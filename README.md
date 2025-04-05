[![Make](https://github.com/berryerlouis/Hexapodcpp/actions/workflows/build.yaml/badge.svg)](https://github.com/berryerlouis/Hexapodcpp/actions/workflows/build.yaml)

# Hexapod

![Hmi](images/HMI2.png)

# Install

The following tools are used:

- Install VS-code or CLion
- Install nodeJs and npm

# Yocto

Install kas

``` shell
sudo pip install kas
```

or

``` shell
sudo apt install kas
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
