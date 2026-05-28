#!/usr/bin/env bash
# -----------------------------------------------------------------------------
# setup-ps3-controller.sh
#
# Configures a Raspberry Pi (tested on RPi Zero 2 W, Raspberry Pi OS Bookworm)
# to pair and use a PlayStation 3 (Sixaxis / DualShock 3) controller as input
# for the Hexapodcpp binary.
#
# Two modes are supported:
#   --usb   : pair via USB cable (recommended for genuine PS3 pads).
#             Plug the controller into the Pi over USB before running.
#   --bt    : pair purely over Bluetooth (works for many DS3 clones).
#             Put the controller in pairing mode first (PS + Share until the
#             LEDs blink fast), then pass --mac AA:BB:CC:DD:EE:FF.
#
# Usage examples:
#   sudo ./setup-ps3-controller.sh --usb
#   sudo ./setup-ps3-controller.sh --bt --mac 00:26:43:AA:BB:CC
#   sudo ./setup-ps3-controller.sh --user pi              # default
#   sudo ./setup-ps3-controller.sh --test                 # jstest only
#
# The script is idempotent: re-running it will re-install missing packages,
# re-add the user to the 'input' group, and re-trust the controller.
# -----------------------------------------------------------------------------
set -euo pipefail

# ---------- defaults ---------------------------------------------------------
MODE=""                 # usb | bt
MAC=""                  # required in --bt mode
TARGET_USER="${SUDO_USER:-pi}"
RUN_TEST=0
JOY_DEVICE="/dev/input/js0"

# ---------- helpers ----------------------------------------------------------
log()  { printf "\033[1;34m[setup-ps3]\033[0m %s\n" "$*"; }
warn() { printf "\033[1;33m[setup-ps3] WARN:\033[0m %s\n" "$*"; }
err()  { printf "\033[1;31m[setup-ps3] ERROR:\033[0m %s\n" "$*" >&2; }

die()  { err "$*"; exit 1; }

usage() {
    sed -n '2,25p' "$0"
    exit 1
}

require_root() {
    if [[ $EUID -ne 0 ]]; then
        die "Must be run as root. Try: sudo $0 $*"
    fi
}

# ---------- argument parsing -------------------------------------------------
while [[ $# -gt 0 ]]; do
    case "$1" in
        --usb)        MODE="usb"; shift ;;
        --bt)         MODE="bt"; shift ;;
        --mac)        MAC="$2"; shift 2 ;;
        --user)       TARGET_USER="$2"; shift 2 ;;
        --device)     JOY_DEVICE="$2"; shift 2 ;;
        --test)       RUN_TEST=1; shift ;;
        -h|--help)    usage ;;
        *)            err "Unknown argument: $1"; usage ;;
    esac
done

if [[ -z "$MODE" && $RUN_TEST -eq 0 ]]; then
    err "You must pass --usb, --bt, or --test."
    usage
fi

if [[ "$MODE" == "bt" && -z "$MAC" ]]; then
    die "--bt mode requires --mac AA:BB:CC:DD:EE:FF"
fi

# ---------- 1. Install dependencies -----------------------------------------
install_packages() {
    log "Installing required packages..."
    export DEBIAN_FRONTEND=noninteractive
    apt-get update -qq
    local pkgs=(bluetooth bluez bluez-tools libbluetooth-dev joystick evtest)
    if [[ "$MODE" == "usb" ]]; then
        # 'sixad' was removed from Debian Bookworm, so we build the standalone
        # 'sixpair' helper from source instead.
        pkgs+=(build-essential libusb-dev wget ca-certificates)
    fi
    apt-get install -y --no-install-recommends "${pkgs[@]}"
}

# ---------- 1b. Build sixpair from source (USB mode only) -------------------
ensure_sixpair() {
    if command -v sixpair >/dev/null 2>&1; then
        log "sixpair already installed."
        return
    fi
    log "Building sixpair from source..."
    local tmp
    tmp=$(mktemp -d)
    # Upstream sources for sixpair.c (one-file libusb program).
    local urls=(
        "https://raw.githubusercontent.com/RetroPie/sixad/master/sixpair.c"
        "http://www.pabr.org/sixlinux/sixpair.c"
    )
    local ok=0
    for u in "${urls[@]}"; do
        if wget -q -O "$tmp/sixpair.c" "$u"; then
            ok=1; break
        fi
    done
    [[ $ok -eq 1 ]] || die "Could not download sixpair.c. Check network/proxy."
    gcc -o "$tmp/sixpair" "$tmp/sixpair.c" -lusb \
        || die "Failed to compile sixpair (is libusb-dev installed?)."
    install -m 0755 "$tmp/sixpair" /usr/local/bin/sixpair
    rm -rf "$tmp"
    log "Installed /usr/local/bin/sixpair."
}

# ---------- 2. Enable bluetooth service -------------------------------------
enable_bluetooth() {
    log "Enabling Bluetooth service..."
    systemctl enable --now bluetooth
    # Ensure adapter is powered up.
    hciconfig hci0 up 2>/dev/null || true
}

# ---------- 3. Load joystick kernel module ----------------------------------
ensure_joydev() {
    log "Loading joydev kernel module..."
    modprobe joydev || warn "Could not modprobe joydev (already built in?)."
    if ! grep -q '^joydev' /etc/modules 2>/dev/null; then
        echo joydev >> /etc/modules
    fi
}

# ---------- 4. Add target user to 'input' group -----------------------------
grant_input_access() {
    if ! id -u "$TARGET_USER" >/dev/null 2>&1; then
        warn "User '$TARGET_USER' does not exist. Skipping group membership."
        return
    fi
    log "Adding user '$TARGET_USER' to group 'input'..."
    usermod -aG input "$TARGET_USER"
    # Persistent udev rule so /dev/input/js* is always group=input mode 660.
    cat >/etc/udev/rules.d/99-hexapod-joystick.rules <<'EOF'
# Hexapodcpp: make joystick devices readable by the 'input' group.
KERNEL=="js[0-9]*",   MODE="0660", GROUP="input"
KERNEL=="event[0-9]*", MODE="0660", GROUP="input"
EOF
    udevadm control --reload-rules || true
    udevadm trigger --subsystem-match=input || true
}

# ---------- 5a. USB pairing path --------------------------------------------

# Read the Pi's Bluetooth adapter MAC. Works on Bookworm where `hcitool` is
# deprecated. Returns empty string on failure.
get_adapter_mac() {
    local mac
    mac=$(bluetoothctl show 2>/dev/null | awk '/^[[:space:]]*Controller /{print $2; exit}')
    if [[ -z "$mac" ]]; then
        mac=$(hciconfig hci0 2>/dev/null | awk '/BD Address/{print $3; exit}')
    fi
    echo "$mac"
}

# Temporarily unbind the controller from the kernel HID driver so sixpair can
# claim the USB interface. Echoes the (driver,id) pair so we can rebind.
unbind_sony_driver() {
    local devpath
    for devpath in /sys/bus/hid/devices/*054C:0268*; do
        [[ -e "$devpath" ]] || continue
        local id
        id=$(basename "$devpath")
        local drv
        drv=$(readlink -f "$devpath/driver" 2>/dev/null || true)
        if [[ -n "$drv" ]]; then
            echo "$id" > "$drv/unbind" 2>/dev/null || true
            echo "${drv}|${id}"
        fi
    done
}

rebind_sony_driver() {
    local entry="$1"
    [[ -z "$entry" ]] && return
    local drv="${entry%%|*}"
    local id="${entry##*|}"
    echo "$id" > "$drv/bind" 2>/dev/null || true
}

pair_usb() {
    log "USB pairing mode: plug the PS3 controller into the Pi via USB now."
    log "Waiting up to 30 s for the controller to appear..."
    local found=0
    for i in $(seq 1 30); do
        if lsusb -d 054c:0268 >/dev/null 2>&1; then
            found=1; break
        fi
        sleep 1
    done
    [[ $found -eq 1 ]] || die "PS3 controller not detected on USB (vendor 054c:0268)."

    # Give udev / the sony kernel driver a moment to do its own pairing.
    sleep 2

    local adapter
    adapter=$(get_adapter_mac)
    if [[ -z "$adapter" ]]; then
        die "Could not read Bluetooth adapter MAC. Is 'bluetooth' service running? Try: systemctl status bluetooth"
    fi
    log "Bluetooth adapter MAC: $adapter"

    # The DS3 often re-enumerates rapidly when its battery is flat or the Pi
    # cannot supply enough power. Use a tight poll loop (no fixed sleep) so we
    # have a chance of catching the device while it is actually enumerated.
    log "Running sixpair $adapter (will retry for up to 30 s)..."
    local paired=0
    local rebind_entry=""
    local seen_count=0
    local end=$(( $(date +%s) + 30 ))
    while [[ $(date +%s) -lt $end ]]; do
        if ! lsusb -d 054c:0268 >/dev/null 2>&1; then
            sleep 0.1
            continue
        fi
        seen_count=$((seen_count + 1))
        # Unbind kernel sony driver so libusb (sixpair) can claim the device.
        rebind_entry=$(unbind_sony_driver | head -n1)
        if sixpair "$adapter" >/tmp/sixpair.log 2>&1; then
            paired=1
            rebind_sony_driver "$rebind_entry"
            break
        fi
        rebind_sony_driver "$rebind_entry"
        sleep 0.1
    done

    if [[ $paired -ne 1 ]]; then
        warn "sixpair never completed cleanly. Last output:"
        sed 's/^/    /' /tmp/sixpair.log >&2 || true
        if [[ $seen_count -eq 0 ]]; then
            warn "The controller never appeared on USB during the retry window."
            warn "It is plugged in but is re-enumerating too fast to be caught."
        else
            warn "Controller was seen $seen_count times but sixpair could not talk to it."
        fi
        warn "Root cause is almost always insufficient power. Fix it by:"
        warn "  1. charging the controller from a wall/PC for ~30 minutes,"
        warn "  2. using a POWERED USB hub between the Pi and the controller,"
        warn "  3. using a thicker/shorter USB cable,"
        warn "  4. checking 'vcgencmd get_throttled' (non-zero = undervoltage),"
        warn "     and 'dmesg | grep -i under' for undervoltage warnings."
        warn ""
        warn "On Raspberry Pi OS Bookworm the kernel sony driver may pair the"
        warn "controller on its own. Unplug the cable now and press PS; if"
        warn "/dev/input/js0 appears, sixpair was not needed."
    fi

    log "Unplug the USB cable, then press the PS button on the controller."
    log "The controller will now connect over Bluetooth automatically."

    # Discover the controller's MAC for trust.
    local detected=""
    for i in $(seq 1 30); do
        detected=$(bluetoothctl devices | awk '/PLAYSTATION|Wireless Controller|Sixaxis/ {print $2; exit}')
        [[ -n "$detected" ]] && break
        sleep 1
    done
    if [[ -n "$detected" ]]; then
        log "Trusting controller $detected ..."
        bluetoothctl -- trust "$detected" || true
    else
        warn "Could not auto-detect controller MAC. Run 'bluetoothctl devices' later and 'trust <MAC>'."
    fi
}

# ---------- 5b. Bluetooth pairing path --------------------------------------
pair_bt() {
    log "Bluetooth pairing mode for $MAC ..."
    bluetoothctl -- power on
    bluetoothctl -- agent on
    bluetoothctl -- default-agent
    log "Scanning for 10 s (make sure controller is in pairing mode)..."
    timeout 10 bluetoothctl -- scan on || true
    bluetoothctl -- pair    "$MAC" || warn "pair failed (may already be paired)"
    bluetoothctl -- trust   "$MAC"
    bluetoothctl -- connect "$MAC" || warn "connect failed; press the PS button"
}

# ---------- 6. Sanity check --------------------------------------------------
verify_device() {
    log "Waiting for $JOY_DEVICE ..."
    for i in $(seq 1 15); do
        [[ -e "$JOY_DEVICE" ]] && break
        sleep 1
    done
    if [[ ! -e "$JOY_DEVICE" ]]; then
        warn "$JOY_DEVICE not present. Press the PS button on the controller."
        return 1
    fi
    log "OK: $(ls -l "$JOY_DEVICE")"
    log "Identity: $(udevadm info --query=property --name="$JOY_DEVICE" | grep ID_INPUT_JOYSTICK || true)"
}

# ---------- 7. Optional interactive test ------------------------------------
run_jstest() {
    [[ -e "$JOY_DEVICE" ]] || die "$JOY_DEVICE not found. Pair the controller first."
    log "Running jstest. Move sticks and press buttons. Ctrl-C to quit."
    jstest "$JOY_DEVICE"
}

# ---------- main -------------------------------------------------------------
require_root

if [[ $RUN_TEST -eq 1 && -z "$MODE" ]]; then
    run_jstest
    exit 0
fi

#install_packages
enable_bluetooth
ensure_joydev
grant_input_access

case "$MODE" in
    usb) ensure_sixpair; pair_usb ;;
    bt)  pair_bt  ;;
esac

verify_device || true

log "Done."
log "If '$TARGET_USER' was newly added to the 'input' group, log out and back in"
log "(or reboot) so the Hexapodcpp process can open $JOY_DEVICE."
log ""
log "Quick test:  sudo $0 --test"
log "Or:          jstest $JOY_DEVICE"
