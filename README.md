# Патчи для включения поддержки разного рода оборудования.
Список:
```
/target/linux/sunxi/config-4.xx
/target/linux/sunxi/modules.mk
/target/linux/sunxi/patches-4.xx/202-add0-pinctrl_wifi-xr819-opi-zero.patch
/target/linux/sunxi/patches-4.xx/203-add-enable-soc_audio-routing-opi-zero.patch
/target/linux/sunxi/patches-4.xx/204-add-IR-pinctrl-opi-zero.patch
/target/linux/sunxi/patches-4.xx/205-add-w1-gpio_pinctrl-opi-zero.patch
/target/linux/sunxi/patches-4.xx/206-add-PA5-pwm_pin-opi-zero.patch
/target/linux/sunxi/patches-4.xx/207-add0-cpu-freg-ths.patch
/target/linux/sunxi/patches-4.xx/207-add1-thermal_sensor-opi-zero.patch
/target/linux/sunxi/patches-4.xx/207-add2-thermal_zone-opi-zero.patch
/target/linux/sunxi/patches-4.xx/207-add3-sunxi-ths_linux.patch
/target/linux/sunxi/patches-4.xx/207-add4-sunxi-iio_gpadc_linux.patch
/target/linux/sunxi/patches-4.xx/207-add5-sunxi-iio_gpadc_linux.patch
/target/linux/sunxi/patches-4.xx/208-add-vdd-cpux-regulator-opi-zero.patch
/target/linux/sunxi/patches-4.xx/209-dts-add-orangepi-zero-lts.patch
/target/linux/sunxi/patches-4.xx/210-add-spdif-PA17-red_led-opi-zero.patch
/target/linux/sunxi/patches-4.xx/211-add-button-opi-zero.patch
/target/linux/sunxi/image/cortex-a7.mk # Добавит профиль orangepi-zero и orangepi-zero-lts
/target/linux/sunxi/base-files/etc/rc.local
/package/boot/uboot-sunxi/patches/001-add-orangepi-zero-lts.patch
/package/boot/uboot-sunxi/Makefile
/package/kernel/mac80211/files/lib/wifi
```
**[Для openwrt-18.06.8](https://github.com/openwrt/openwrt/archive/v18.06.8.tar.gz)**
```
 все в одном
patch -p1 < All_openwrt-18.06.8.patch
```
**[Для openwrt-19.07.2](https://github.com/openwrt/openwrt/archive/v19.07.2.tar.gz)**
```
 все в одном
patch -p1 < All_openwrt-19.07.2.patch
```
* ------------------------------------- 

# package
* Для xradio (к *openwrt-19.07 kernel-4.14, openwrt-master kernel-4.19*) в *feeds.conf.default* прописать строку:
```
src-git opicyberwrt https://github.com/melsem/opi-zero-cyberwrt.git
```
* А для xradio к *openwrt-18.06* kernel-4.14 в *feeds.conf.default* прописать строку:
```
src-git opicyberwrt https://github.com/melsem/opi-zero-cyberwrt.git^2d7ef9a0408dfc5cb74235e9abd8d18b8437db2e
```
* -------------------------------------------------------------------------------------
* И в исходниках.
```
./scripts/feeds update -a
./scripts/feeds install -a
```
* -------------------------------------------------------------------------------------
- **[Модуль-прога "boot-config". Редактор dtb фала uboot](https://github.com/melsem/opi-zero-cyberwrt/blob/master/boot-config/boot-config2.png)**
```
http://192.168.10.1/boot

    Проверялось в openwrt-18.06, openwrt-19.07 и openwrt-master
    На orange-pi-zero, orange-pi-zero-lts, orange-pi-R1
```
* -------------------------------------------------------------------------------------
- **[Модуль-прога "termostat-serial-arduino". Управление платой Arduino-termostat](http://cyber-place.ru/showthread.php?t=2752)**
* -------------------------------------------------------------------------------------
- **[Модуль-прога "web-radio2"](http://cyber-place.ru/showthread.php?t=2731)**
* -------------------------------------------------------------------------------------
- **[Модуль-прога "termostat"-датчики к GPIO и "termostat-usb-data-uart"-датчики к usb или к data-uart](http://cyber-place.ru/showthread.php?t=1214)**
* -------------------------------------------------------------------------------------
- **[Модуль-прога "i2c_HD44780" модуль на расширителе PCF8574. И можно как терморегуляторы, часы. датчики к GPIO  к usb или к data-uart.](http://cyber-place.ru/showthread.php?t=2164)**
* -------------------------------------------------------------------------------------
- **[Модуль-прога "web-cyberwrt" мой только скин..](http://cyber-place.ru/forumdisplay.php?f=44)**
```
http://192.168.10.1/cyberwrt
```
* -------------------------------------------------------------------------------------
- **[Модуль-прога "web-ir-remote". Упрощается настройка IR пульта](https://www.sigmdel.ca/michel/ha/opi/ir_03_en.html)**
```
http://192.168.10.1/irc
```
* -------------------------------------------------------------------------------------
- **[Пакет для сборки драйвера rtl8189es к orange-pi-R1](https://github.com/melsem/opi-zero-cyberwrt/tree/master/rtl8189es)**
- **[Подправленные исходники rtl8189es](https://github.com/melsem/rtl8189es)**
* Для rtl8189es (к *openwrt-19.07* kernel-4.14 и *openwrt-18.06* kernel-4.14) в *feeds.conf.default* надо, что бы была
 прописана строка:
```
src-git opicyberwrt https://github.com/melsem/opi-zero-cyberwrt.git
```
или
```
src-git opicyberwrt https://github.com/melsem/opi-zero-cyberwrt.git^2d7ef9a0408dfc5cb74235e9abd8d18b8437db2e
```
```
    Как AP работает нормально. Проверялось на openwrt-18.06 и openwrt-19.07 (kernel-4.14)
    С openwrt-master (kernel-4.19) не работает. При включении wifi, kernel panic, и уходит в ребут.

    При сканировании "iw" вызывает, kernel panic, и уходит в ребут.
    А попытки вручную создать "sta" ни к чему не привели.
    Не коннектится.

    Включить/выключить wifi в:
    http://192.168.10.1/boot
```
* -------------------------------------------------------------------------------------


