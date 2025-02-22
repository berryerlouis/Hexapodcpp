[![Make](https://github.com/berryerlouis/Hexapodcpp/actions/workflows/build.yaml/badge.svg)](https://github.com/berryerlouis/Hexapodcpp/actions/workflows/build.yaml)

# Hexapod

![Hmi](images/HMI.png)

# Install

The following tools are used:

- Install VS-code or CLion
- If you are using VS-code install serial-monitor extension

# Yocto

Install kas

``` shell
sudo pip install kas
```

Clone repo and build sdk

``` shell
git clone git@github.com:berryerlouis/yocto-rpiw.git
cd yocto-rpiw
kas build meta-raspberrypi/kas-poky-rpi.yml -c populate_sdk
cd build/tmp/deploy/sdk/
./poky-glibc-x86_64-core-image-base-arm1176jzfshf-vfp-raspberrypi0-wifi-toolchain-5.1.sh
```

# Architecture

![Architecture](images/architecture.png)

# Using Cmake

## Compile

 ``` shell
 # Arguments:
 # 1: target (AVR, X64, RPI)
 # 2: source or test
 # 3: DEBUG, RELEASE, or CLEAN
 # 4: RPI install wiring PI (optional)
 bin/dev/build.sh RPI source RELEASE install
 ```

## Google Test

 ``` shell
 bin/dev/test.sh all
 ```

# Communication

## Websocket

- port: 8080
