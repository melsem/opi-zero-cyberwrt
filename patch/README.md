
# OrangePi-zero, OrangePi-R1 MMC or spi-flash-16Mb. Stable openwrt-v22.03.6
- * There is a "boot-config" utility to configure the boot configuration.
- * Web utility "boot-config": 192.168.10.1/boot
- * Console:
```
boot-config -h
```
![screenshots](https://github.com/melsem/opi-zero-cyberwrt/blob/master/patch/2024-03-01_22-12-37.png)
# An example of using the build system.
- * Wifi: xradio_wlan (xradio-xr819) - okay
- * Soc: i2c0, i2c1, i2c2, spi0-flash, usb0-1-2-3 - okay
- * Soc: usb0-otg g_ether gadget: Ethernet Gadget - okay
- * Soc: H3 Audio Codec, lirc_dev - okay
- 
- Overlay add modules - okay
- * input: rotary-encoder rotary_axis rotary_button - okay
- * pps: gpios, echo-gpios - okay
- * input: gpio_keys_user - okay
- * 1-wire Dallas network protocol - okay
- * I2C4 (Any of the free gpios.) - okay
-
- * Overlays folder:
```
ls -l /etc/dt-overlays
```
- * Using ready-made samples, you can add your overlays to the folder: /etc/dt-overlays
- 
- 
![screenshots](https://github.com/melsem/opi-zero-cyberwrt/blob/master/patch/2024-03-01_22-05-06.png)
![screenshots](https://github.com/melsem/opi-zero-cyberwrt/blob/master/patch/2024-03-01_22-05-16.png)
![screenshots](https://github.com/melsem/opi-zero-cyberwrt/blob/master/patch/2024-03-01_22-05-26.png)
- [1.] Download sources for **[old-stable openwrt-v22.03.6](https://github.com/openwrt/openwrt/archive/refs/tags/v22.03.6.tar.gz)**
* And unpack:
```
wget https://github.com/openwrt/openwrt/archive/refs/tags/v22.03.6.tar.gz
tar -xvf v22.03.6.tar.gz
cd openwrt-22.03.6
```
- [2.] Download patches **[1-old-stable openwrt-v22.03.6](https://github.com/melsem/opi-zero-cyberwrt/raw/master/patch/All_openwrt-22.03.6.patch)**
 and **[2-old-stable openwrt-v22.03.6](https://github.com/melsem/opi-zero-cyberwrt/raw/master/patch/036-.config_openwrt-22.03.6.patch)**
* Download two patches to the root folder of openwrt-22.03.6:
```
wget https://github.com/melsem/opi-zero-cyberwrt/raw/master/patch/All_openwrt-22.03.6.patch
wget https://github.com/melsem/opi-zero-cyberwrt/raw/master/patch/036-.config_openwrt-22.03.6.patch
./scripts/feeds update -a
patch -p1 < All_openwrt-22.03.6.patch
./scripts/feeds update opicyberwrt
./scripts/feeds update diskman
./scripts/feeds install -a
patch -p1 < 036-.config_openwrt-22.03.6.patch
make menuconfig
make -j1 V=s
```
