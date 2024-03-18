# OrangePi-zero, OrangePi-R1 MMC or spi-flash-16Mb. Stable openwrt-v22.03.6.
 - Images can be collected for "MMS" or for spi-flash.
 - By checking in "make menuconfig" > Target Images > SPI flash size for firmware image.
 - 
- * There is a "boot-config" utility to configure the boot configuration.
- * Web utility "boot-config": 192.168.10.1/boot
![screenshots](https://github.com/melsem/opi-zero-cyberwrt/blob/master/patch/2024-03-01_22-12-37.png)
- * Console:
```
boot-config -h
```
```
BusyBox v1.36.1 (2023-11-14 13:38:11 UTC) built-in shell (ash)

  _______                     ________        __
 |       |.-----.-----.-----.|  |  |  |.----.|  |_
 |   -   ||  _  |  -__|     ||  |  |  ||   _||   _|
 |_______||   __|_____|__|__||________||__|  |____|
          |__| W I R E L E S S   F R E E D O M
 -----------------------------------------------------
 OpenWrt 23.05.2, r23630-842932a63d
 -----------------------------------------------------
root@orangepi-zero:~# boot-config -h

     xunlong,orangepi-zero

 Convert dtb to dts Example:   boot-config dtb-dts
 Convert dts to dtb Example:   boot-config dts-dtb

   ------ Overlay-module names. ------
   mmc0 / mmc1 / mmc2 / ir / pwm / i2c0 / i2c1 / i2c2 / camera / audcodec
   i2s0 / i2s1 / uart0 / uart1 / uart2 / otg / host / peripheral / spdif
   ehci0 / ohci0 / ehci1 / ohci1 / ehci2 / ohci2 / ehci3 / ohci3
   usb0 / usb1 / usb2 / usb3 / spi0 / spi1 / ethernet0
   otg_tipe / host_tipe / peripheral_tipe
   w1 / pps / i2c4 / gpio_keys_user
   rotary or rotary_axis

 Modules that can be controlled by commands: <on / ON / okay> or <off / OFF / disabled>.
   Usage:   boot-config <module> <on / ON / okay>
   Or:      boot-config <module> <off / OFF / disabled>
   Example1: boot-config i2c0 on
   Example2: boot-config usb2 off

     ------------------------------------------------
  Example3: boot-config otg_tipe on
  Or:       boot-config host_tipe on
  Or:       boot-config peripheral_tipe on

  Or all USB0 tips disable (otg, host, peripheral).
  example:  boot-config host_tipe off
     ------------------------------------------------

 In the modules 'w1' you can still change the pins.
   Add or edit single one-wire gpio-contacts.

   Usage:    boot-config -g <module> <gpio number>
   Example4: boot-config -g w1 10

   Usage:    boot-config <module> <gpio name>
   Example5: boot-config w1 PA10
     ------------------------------------------------

 Edit SDA or SCL gpio i2c4.
   sda_i2c4 / sda-i2c4 / scl-i2c4 / scl_i2c4
   Usage:   boot-config <edit gpio i2c4> <gpio name>
   Example6:      boot-config sda_i2c4 PA18
   Example6:      boot-config scl_i2c4 PA19
   Example7 - or: boot-config -g sda_i2c4 18
   Example7 - or: boot-config -g scl_i2c4 19

 Edit gpios or echo-gpios pps.
   gpios_pps / gpios-pps / echo_pps / echo-pps
   Usage:   boot-config <edit gpio pps> <gpio name>
   Example8:      boot-config gpios_pps PA6
   Example8:      boot-config echo_pps PA7
   Example9 - or: boot-config -g gpios_pps 6
   Example9 - or: boot-config -g echo_pps 7

 Add overlays modules.
   Usage:          boot-config overlays <overlays name>
   Example10:      boot-config overlays i2c4
   Example10 - or: boot-config overlays pps

 Erase overlays modules.
   Usage:   boot-config erase <overlays name>
   Usage:   boot-config del <overlays name>
   Usage:   boot-config delete <overlays name>
   Example11:      boot-config erase i2c4
   Example11 - or: boot-config del pps
   Example11 - or: boot-config delete w1

 Convert dtb to dts Example12:   boot-config dtb-dts
 Convert dts to dtb Example12:   boot-config dts-dtb
```
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
![screenshots](https://github.com/melsem/opi-zero-cyberwrt/blob/master/patch/2024-03-01_22-05-06.png)
![screenshots](https://github.com/melsem/opi-zero-cyberwrt/blob/master/patch/2024-03-01_22-05-16.png)
![screenshots](https://github.com/melsem/opi-zero-cyberwrt/blob/master/patch/2024-03-01_22-05-26.png)

###############################

# Old patches..

# An example of using the build system.
- * Wifi: xradio_wlan (xradio-xr819) - okay
- * Soc: i2c0, i2c1, i2c2, spi0-flash, usb0-1-2-3 - okay
- * Soc: usb0-otg g_ether gadget: Ethernet Gadget - okay
- * Soc: H3 Audio Codec, lirc_dev - okay
- 
- Overlay modules - okay
- * input: rotary-encoder rotary_axis rotary_button - okay
- * input: gpio_keys_user - okay
- * 1-wire Dallas network protocol - okay
- * I2C4 (Any of the free pins.) - okay
- 
- 
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


