[![Make](https://github.com/berryerlouis/Hexapodcpp/actions/workflows/build.yaml/badge.svg)](https://github.com/berryerlouis/Hexapodcpp/actions/workflows/build.yaml)

# Hexapod

![Hmi](images/HMI2.png)

# Install IDE and Tools

The following tools are used:

- Install VS-code or CLion
- Install nodeJs and npm

# Prepare RPI

``` shell
sudo apt update
sudo apt install build-essential gcc g++ cmake git libssl-dev
```

## Install WiringPi

``` shell
git clone https://github.com/WiringPi/WiringPi.git
cd WiringPi
./build
```

## Enable I2C

``` shell
sudo nano /boot/firmware/config.txt
#uncomment line dtparam=i2c_arm=on
sudo modprobe i2c-dev
```

``` shell
sudo nano /etc/modules
```

``` shell
# /etc/modules: kernel modules to load at boot time.
#
# This file contains the names of kernel modules that should be loaded
# at boot time, one per line. Lines beginning with "#" are ignored.
# Parameters can be specified after the module name.
i2c-dev
```

## Create Systemd service

``` shell
sudo nano /etc/systemd/system/hexabot.service
```

### Add the following content (change user and working directory as needed)

``` shell
[Unit]
Description=Hexabot service
After=network-online.target
Wants=network-online.target

[Service]
Type=simple
User=hexabot
WorkingDirectory=/hexabot/louis
ExecStart=/home/hexabot/Hexapodcpp
Restart=on-failure
RestartSec=3s

[Install]
WantedBy=multi-user.target
```

### Enable service

``` shell
sudo systemctl daemon-reload
sudo systemctl enable --now hexabot.service
```

### verify service

``` shell
systemctl status hexabot.service
journalctl -u hexabot.service -e
```

# Prepare host system

``` shell
sudo apt install build-essential gcc g++ cmake git nodejs npm
```

# Configure and Compile App

## Using script

``` shell
bin/dev/build.sh X64 sources RELEASE
```

## Configuration For X64

``` shell
cmake -DCMAKE_BUILD_TYPE=Debug -DTARGET=X64 -Wno-dev -G "Unix Makefiles" -S . -B ./build/gcc-debug
```

## Configuration For RPI

``` shell
cmake -DCMAKE_BUILD_TYPE=Debug -DTARGET=RPI -Wno-dev -G "Unix Makefiles" -S . -B ./build/gcc-debug'
```

## Compile

``` shell
cmake --build ./build/gcc-debug --target Hexapodcpp -- -j 16
```

# Configure and Compile Google Test

## Using script

``` shell
bin/dev/test.sh all
```

## Configuration (only for X64)

``` shell
cmake -DCMAKE_BUILD_TYPE=Debug -DGTEST=1 -Wno-dev -G "Unix Makefiles" -S . -B ./build/hexapodTest
```

## Compile

``` shell
cmake --build ./build/hexapodTest --target HexapodcppTest -- -j 16
```

# Release

``` shell
bin/prod/build.sh
```

# CrossCompilation using Docker

``` shell
# Release build (default)
./bin/dev/docker-build-rpi.sh

# Debug build
./bin/dev/docker-build-rpi.sh debug

# Release with clean
./bin/dev/docker-build-rpi.sh release clean

# Debug with clean
./bin/dev/docker-build-rpi.sh debug clean
```

# Architecture

![Architecture](images/architecture.svg)

## Render as svg

``` shell
docker run --rm -v $PWD:/ws -w /ws plantuml/plantuml -tsvg images/architecture.puml
```

# Using Cmake

## Compile

 ``` shell
 # Arguments:
 # 1: target (X64, RPI)
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

## HMI

Go to HMI folder

 ``` shell
 cd HMI
 ```

and run

 ``` shell
 npm run dev
 ```

- http://localhost:5173/

## Websocket

- port: 8080
