
# Как всем этим воспользоваться.
- [1.] Скачать исходники к **[openwrt-18.06.8](https://github.com/openwrt/openwrt/archive/v18.06.8.tar.gz)**
 или **[openwrt-19.07.7](https://github.com/openwrt/openwrt/archive/v19.07.7.tar.gz)** . Распаковать..
- [2.] Скачать патч к **[openwrt-18.06.8](https://github.com/melsem/opi-zero-cyberwrt/raw/master/patch/All_openwrt-18.06.8.patch.zip)**
 или **[openwrt-19.07.7](https://github.com/melsem/opi-zero-cyberwrt/raw/master/patch/All_openwrt-19.07.6.MMC.zip)**
- [3.] Распаковать патч с архива в openwrt-18.06.8 или openwrt-19.07.7. И применить
```
patch -p1 < All_openwrt-18.06.8.patch
```
Или
```
patch -p1 < All_openwrt-19.07.6.patch
```
- [4.] Прописать строку в *feeds.conf.default*:
 openwrt-18.06.8
```
src-git opicyberwrt https://github.com/melsem/opi-zero-cyberwrt.git^b18e40dea7b0f330c0b5398d772167c30732e67a
```
 openwrt-19.07.7
```
src-git opicyberwrt https://github.com/melsem/opi-zero-cyberwrt.git
```
- [5.] Обновить сырцы пакетов и установить ярлыки.
```
./scripts/feeds update -a
./scripts/feeds install -a
make menuconfig
```
- Run "make menuconfig" to select your preferred configuration for the toolchain, target system & firmware packages. 
- Be sure to select web-cyberwrt and boot-config-mmc in utils. The rest as needed.
- Запустите «make menuconfig», чтобы выбрать предпочитаемую конфигурацию для цепочки инструментов, целевой системы и пакетов прошивки.
- Выберите boot-config в utils. Остальное по мере необходимости.
- Утилита boot-config должна редактировать dtb на всех sun8i-h3 и sun8i-h2-plus:
```
bananapi-m2-zero, libretech-all-h3-cc, orangepi-r1, orangepi-zero, orangepi-zero-lts, 
bananapi-m2-plus, bananapi-m2-plus-v1.2, beelink-x2, emlid-neutis-n5h3-devboard, 
libretech-all-h3-cc, mapleboard-mp130, nanopi-duo2, nanopi-m1, nanopi-m1-plus, nanopi-neo, 
nanopi-neo-air, nanopi-r1, orangepi-2, orangepi-lite, orangepi-one, orangepi-pc, 
orangepi-pc-plus, orangepi-plus, orangepi-plus2e, orangepi-zero-plus2, rervision-dvk, zeropi
```
Тестировал на orangepi-r1, orangepi-zero, orangepi-zero-lts и orangepi-pc
```
make
```
- После сборки, записи образа на флэшку.
- При первой загрузке Orange Pi надо войти в утилиту boot-config для настройки конфигурации начальной загрузки
```
http://192.168.10.1/boot
```
-  Или с вебки Luci
```
http://192.168.10.1/cgi-bin/luci/admin/system/bootconfig
```

* ---------------------------------------

* ---------------------------------------
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
- **[Для openwrt-18.06.8](https://github.com/openwrt/openwrt/archive/v18.06.8.tar.gz)**
- **[patch openwrt-18.06.8](https://github.com/melsem/opi-zero-cyberwrt/raw/master/patch/All_openwrt-18.06.8.patch.zip)**
-  все в одном
-  Распаковать в корень сырцов openwrt-18.06.8 и:
```
patch -p1 < All_openwrt-18.06.8.patch
```
- **[Для openwrt-19.07.7](https://github.com/openwrt/openwrt/archive/v19.07.7.tar.gz)**
- **[patch openwrt-19.07.7](https://github.com/melsem/opi-zero-cyberwrt/raw/master/patch/All_openwrt-19.07.6.MMC.zip)**
-  все в одном
-  Распаковать в корень сырцов openwrt-19.07.7 и:
```
patch -p1 < All_openwrt-19.07.6.patch
```
-  
-  Или для загрузки со SPI-флэшки, перепаянной под 16мб.
-  Если в MMC и USB будут отсутствовать образы.
- **[Для openwrt-19.07.7](https://github.com/openwrt/openwrt/archive/v19.07.7.tar.gz)**
- **[patch SPI-флэш openwrt-19.07.7](https://github.com/melsem/opi-zero-cyberwrt/raw/master/patch/All_openwrt-19.07.7-SPI-flash-firmware-16mb.zip)**
-  все в одном
-  Распаковать в корень сырцов openwrt-19.07.7 и:
```
patch -p1 < All_openwrt-19.07.7-SPI-flash-firmware-16mb.patch
```
* ------------------------------------- 

# package
* Для xradio (к *openwrt-19.07 kernel-4.14, openwrt-master kernel-5.4.*) в *feeds.conf.default* прописать строку:
```
src-git opicyberwrt https://github.com/melsem/opi-zero-cyberwrt.git
```
* А для xradio к *openwrt-18.06* kernel-4.14 в *feeds.conf.default* прописать строку:
```
src-git opicyberwrt https://github.com/melsem/opi-zero-cyberwrt.git^b18e40dea7b0f330c0b5398d772167c30732e67a
```
* -------------------------------------------------------------------------------------
* И в исходниках.
```
./scripts/feeds update -a
./scripts/feeds install -a
```
* -------------------------------------------------------------------------------------
- **[Модуль-прога "boot-config". Редактор dtb фала uboot](https://github.com/melsem/opi-zero-cyberwrt/blob/master/boot-config/boot-config2.png)**
-    Проверялось в openwrt-18.06, openwrt-19.07 и openwrt-master-21.02. 
-   На orange-pi-zero, orange-pi-zero-lts, orange-pi-R1 и orangepi-pc
```
http://192.168.10.1/boot
```
-  Или с вебки Luci
```
http://192.168.10.1/cgi-bin/luci/admin/system/bootconfig
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
- **[Подправленные исходники rtl8189es](https://github.com/melsem/rtl8189ES_linux)**
* Для rtl8189es (к *openwrt-19.07* kernel-4.14, *openwrt-master* kernel-5.4.*) в *feeds.conf.default* надо, что бы была
 прописана строка:
```
src-git opicyberwrt https://github.com/melsem/opi-zero-cyberwrt.git
```
или к *openwrt-18.06* kernel-4.14
```
src-git opicyberwrt https://github.com/melsem/opi-zero-cyberwrt.git^b18e40dea7b0f330c0b5398d772167c30732e67a
```
-    Как AP работает нормально. Проверялось на openwrt-18.06 и openwrt-19.07 (kernel-4.14)
-    Клиент не работает!!!
-    При сканировании, "iw" вызывает kernel panic, и уходит в ребут.
-    А попытки вручную создать "sta", без сканирования, ни к чему не привели.
-    Не коннектится.
-
-    С openwrt-master (kernel-4.19) не работает!!! При включении wifi, kernel panic, и уходит в ребут.
-    В *openwrt-master* kernel-5.4.* не проверял..
-
    Включить/выключить wifi в:
```
    http://192.168.10.1/boot
```
* -------------------------------------------------------------------------------------


