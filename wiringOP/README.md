
# Depends:
-    make menuconfig, select Global Build Settings ---> Kernel Build Options:
-    check /dev/mem virtual device support and
-    check /dev/kmem virtual device support

-    Select your board from the list below and enter it into the file /patches/001-add-board_config.patch:

```
	orangepione-h3
	orangepilite-h3
	orangepipc-h3
	orangepiplus-h3
	orangepipcplus-h3
	orangepiplus2e-h3
	orangepi2giot
	orangepipc2-h5
	orangepiprime-h5
	orangepizeroplus-h5
	orangepiwin-a64
	orangepiwinplus-a64
	orangepizero-h2
	orangepir1-h2
	orangepioneplus-h6
	orangepilite2-h6
	orangepi3-h6
	orangepizero2-h616
	orangepizeroplus2h3
	orangepizeroplus2h5
	orangepirk3399
	orangepi4
	orangepi800
	orangepir1plus-rk3328
```

-    Example for orangepizero-h2:
```
--- a/BOARD
+++ b/BOARD
@@ -0,0 +1,1 @@
+orangepizero-h2
```
root@orangepi-zero-lts:~# gpio -v
```
gpio version: 2.46
Copyright (c) 2012-2018 Gordon Henderson
This is free software with ABSOLUTELY NO WARRANTY.
For details type: gpio -warranty

  * Device tree is enabled.
  *--> Xunlong Orange Pi Zero Lts
```
- root@orangepi-zero-lts:~# gpio -h
```
gpio: Usage: gpio -v
       gpio -h
       gpio [-g|-1] ...
       gpio <mode/qmode/read/write/pwm> ...
       gpio <toggle/blink> <pin>
       gpio readall
       gpio unexportall/exports
       gpio export/edge/unexport ...
       gpio pwm-bal/pwm-ms 
       gpio i2cd/i2cdetect port
       gpio serial port
       gpio rbx/rbd
       gpio wb <value>
```
- root@orangepi-zero-lts:~# gpio readall
```
 +------+-----+----------+- Xunlong Orange Pi Zero Lts -+----------+-----+------+
 | GPIO | wPi |   Name   | Mode | V | Physical | V | Mode | Name     | wPi | GPIO |
 +------+-----+----------+------+---+----++----+---+------+----------+-----+------+
 |      |     |  +3.3v   |      |   |  1 || 2  |   |      |   +5v    |     |      |
 |   12 |   0 | PA12 sda |  OFF | 0 |  3 || 4  |   |      |   +5v    |     |      |
 |   11 |   1 | PA11 scl |  OFF | 0 |  5 || 6  |   |      |    GND   |     |      |
 |    6 |   2 |     PA6  |  OFF | 0 |  7 || 8  | 0 | OFF  |  PG6     | 3   | 198  |
 |      |     |   GND    |      |   |  9 || 10 | 0 | OFF  |  PG7     | 4   | 199  |
 |    1 |   5 |     PA1  |  OFF | 0 | 11 || 12 | 0 | OFF  |  PA7     | 6   | 7    |
 |    0 |   7 |     PA0  |  OFF | 0 | 13 || 14 |   |      |    GND   |     |      |
 |    3 |   8 |     PA3  |  OFF | 0 | 15 || 16 | 0 | OFF  |  PA19    | 9   | 19   |
 |      |     |  +3.3v   |      |   | 17 || 18 | 0 | OFF  |  PA18    | 10  | 18   |
 |   15 |  11 |     PA15 |  OFF | 0 | 19 || 20 |   |      |    GND   |     |      |
 |   16 |  12 |     PA16 |  OFF | 0 | 21 || 22 | 0 | OFF  |  PA2     | 13  | 2    |
 |   14 |  14 |     PA14 |  OFF | 0 | 23 || 24 | 0 | OFF  |  PA13    | 15  | 13   |
 |      |     |   GND    |      |   | 25 || 26 | 0 | OFF  |  PA10    | 16  | 10   |
 +------+-----+----------+------+---+-- LEDs --+---+------+----------+-----+------+
 |   17 |  17 |  RED-LED |  OUT | 1 | 27 || 28 |   |      |  PWR-LED |     |      |
 +------+-----+----------+------+---+----++----+---+------+----------+-----+------+
 | GPIO | wPi |   Name   | Mode | V | Physical | V | Mode | Name     | wPi | GPIO |
 +------+-----+----------+- Xunlong Orange Pi Zero Lts -+----------+-----+------+
```
- root@orangepi-zero-lts:~# gpio mode 3 in
- root@orangepi-zero-lts:~# gpio mode 4 out
- root@orangepi-zero-lts:~# gpio write 4 1
- root@orangepi-zero-lts:~# gpio readall
```
 +------+-----+----------+- Xunlong Orange Pi Zero Lts -+----------+-----+------+
 | GPIO | wPi |   Name   | Mode | V | Physical | V | Mode | Name     | wPi | GPIO |
 +------+-----+----------+------+---+----++----+---+------+----------+-----+------+
 |      |     |  +3.3v   |      |   |  1 || 2  |   |      |   +5v    |     |      |
 |   12 |   0 | PA12 sda |  OFF | 0 |  3 || 4  |   |      |   +5v    |     |      |
 |   11 |   1 | PA11 scl |  OFF | 0 |  5 || 6  |   |      |    GND   |     |      |
 |    6 |   2 |     PA6  |  OFF | 0 |  7 || 8  | 0 | IN   |  PG6     | 3   | 198  |
 |      |     |   GND    |      |   |  9 || 10 | 1 | OUT  |  PG7     | 4   | 199  |
 |    1 |   5 |     PA1  |  OFF | 0 | 11 || 12 | 0 | OFF  |  PA7     | 6   | 7    |
 |    0 |   7 |     PA0  |  OFF | 0 | 13 || 14 |   |      |    GND   |     |      |
 |    3 |   8 |     PA3  |  OFF | 0 | 15 || 16 | 0 | OFF  |  PA19    | 9   | 19   |
 |      |     |  +3.3v   |      |   | 17 || 18 | 0 | OFF  |  PA18    | 10  | 18   |
 |   15 |  11 |     PA15 |  OFF | 0 | 19 || 20 |   |      |    GND   |     |      |
 |   16 |  12 |     PA16 |  OFF | 0 | 21 || 22 | 0 | OFF  |  PA2     | 13  | 2    |
 |   14 |  14 |     PA14 |  OFF | 0 | 23 || 24 | 0 | OFF  |  PA13    | 15  | 13   |
 |      |     |   GND    |      |   | 25 || 26 | 0 | OFF  |  PA10    | 16  | 10   |
 +------+-----+----------+------+---+-- LEDs --+---+------+----------+-----+------+
 |   17 |  17 |  RED-LED |  OUT | 1 | 27 || 28 |   |      |  PWR-LED |     |      |
 +------+-----+----------+------+---+----++----+---+------+----------+-----+------+
 | GPIO | wPi |   Name   | Mode | V | Physical | V | Mode | Name     | wPi | GPIO |
 +------+-----+----------+- Xunlong Orange Pi Zero Lts -+----------+-----+------+
```



