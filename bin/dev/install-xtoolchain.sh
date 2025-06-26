#!/bin/sh

echo "Install xtoolchain"

mkdir -p  ~/xtool/usr/lib/rpi-arm-linux-gnueabihf/
mkdir -p  ~/xtool/usr/rpi-arm-linux-gnueabihf/include/

 #0x00000001 (NEEDED)                     Shared library: [libwiringPi.so]
 #0x00000001 (NEEDED)                     Shared library: [libssl.so.3]
 #0x00000001 (NEEDED)                     Shared library: [libcrypto.so.3]
 #0x00000001 (NEEDED)                     Shared library: [libstdc++.so.6]
 #0x00000001 (NEEDED)                     Shared library: [libm.so.6]
 #0x00000001 (NEEDED)                     Shared library: [libgcc_s.so.1]
 #0x00000001 (NEEDED)                     Shared library: [libc.so.6]
 #0x00000001 (NEEDED)                     Shared library: [ld-linux-armhf.so.3]

#echo "copy all libs"
#rsync -rl hexabot:/lib/arm-linux-gnueabihf/lib* ~/xtool/usr/lib/rpi-arm-linux-gnueabihf/

echo "copy libthread_db"
rsync -rl hexabot:/lib/arm-linux-gnueabihf/libthread_db* ~/xtool/usr/lib/rpi-arm-linux-gnueabihf/

echo "copy libssl"
rsync -rl hexabot:/lib/arm-linux-gnueabihf/libssl* ~/xtool/usr/lib/rpi-arm-linux-gnueabihf/

echo "copy libcrypto"
rsync -rl hexabot:/lib/arm-linux-gnueabihf/libcrypt* ~/xtool/usr/lib/rpi-arm-linux-gnueabihf/
#sudo ln -sf libcrypt.so.1 /usr/lib/arm-linux-gnueabihf/libcrypt.so

echo "copy libstdc++"
rsync -rl hexabot:/lib/arm-linux-gnueabihf/libstdc++* ~/xtool/usr/lib/rpi-arm-linux-gnueabihf/

echo "copy libm"
rsync -rl hexabot:/lib/arm-linux-gnueabihf/libm* ~/xtool/usr/lib/rpi-arm-linux-gnueabihf/

echo "copy libgcc"
rsync -rl hexabot:/lib/arm-linux-gnueabihf/libgcc_s* ~/xtool/usr/lib/rpi-arm-linux-gnueabihf/

echo "copy libc"
rsync -rl hexabot:/lib/arm-linux-gnueabihf/libc.* ~/xtool/usr/lib/rpi-arm-linux-gnueabihf/
rsync -r hexabot:/lib/arm-linux-gnueabihf/libc_nonshared.a ~/xtool/usr/lib/rpi-arm-linux-gnueabihf/
rsync -r hexabot:/lib/arm-linux-gnueabihf/libarmmem-v6l.so ~/xtool/usr/lib/rpi-arm-linux-gnueabihf/

echo "copy ld-linux-armhf.so.3"
rsync -r hexabot:/usr/lib/arm-linux-gnueabihf/ld-linux-armhf.so.3 ~/xtool/usr/lib/rpi-arm-linux-gnueabihf/

echo "copy includes"
rsync -r hexabot:/usr/include/*  ~/xtool/usr/rpi-arm-linux-gnueabihf/include/
rsync -r hexabot:/usr/include/arm-linux-gnueabihf/*  ~/xtool/usr/rpi-arm-linux-gnueabihf/include/

echo "copy wiring pi"
rsync -rl hexabot:/usr/local/lib/libwiringPi* ~/xtool/usr/lib/rpi-arm-linux-gnueabihf/

echo "copy to destination folders"
sudo cp -r ~/xtool/usr/lib/rpi-arm-linux-gnueabihf/* /usr/lib/rpi-arm-linux-gnueabihf/
sudo cp -r ~/xtool/usr/rpi-arm-linux-gnueabihf/include/* /usr/rpi-arm-linux-gnueabihf/include/
sudo cp -r /usr/lib/rpi-arm-linux-gnueabihf/* /usr/rpi-arm-linux-gnueabihf/lib


##toolchain
#sudo apt-get -y autoremove gcc-arm-linux-gnueabihf
#sudo apt-get -y autoremove g++-arm-linux-gnueabihf
#sudo apt-get -y autoremove libc6-armhf-cross
#sudo apt-get -y install gcc-arm-linux-gnueabihf
#sudo apt-get -y install g++-arm-linux-gnueabihf
#sudo apt-get -y install libc6-armhf-cross
