# package
* Для xradio (к openwrt-19.07 kernel-4.14, openwrt-master kernel-4.19) в feeds.conf.default прописать строку:
```
src-git opicyberwrt https://github.com/melsem/opi-zero-cyberwrt.git
```
* А для xradio к openwrt-18.06 kernel-4.14 в feeds.conf.default прописать строку:
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


