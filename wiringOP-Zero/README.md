# Only for OrangePi-Zero.
*  When referring to gpio pins, they match the datasheet!
-  From under 'wiringOP', gpios from r_pio (bank-11 'gpio-362') are not managed, Only export and non-export is possible.
-  I've only tested these commands so far:
```
       gpio <toggle/blink> <pin>
       gpio <read/write/mode>
       gpio <export/edge/unexport> <pin>
       gpio readall/nreadall/allreadall/reset
       gpio unexportall/exports
```

-  PWM does not work with 'wiringPI'.
```
       gpio pwm-bal/pwm-ms 
       gpio pwmr <range> 
       gpio pwmc <divider>
```

-  Haven't tested these yet.
```    gpio <aread/awritewb>
       gpio load spi/i2c
       gpio i2cd/i2cdetect
       gpio usbp high/low
````

# ----
# Depends, Mark before assembly:
-    make menuconfig, select Global Build Settings ---> Kernel Build Options:
-    check /dev/mem virtual device support and
-    check /dev/kmem virtual device support

root@orangepi-zero-lts:~# gpio -v
```
gpio version: 2.20
Copyright (c) 2012-2014 Gordon Henderson
This is free software with ABSOLUTELY NO WARRANTY.
For details type: gpio -warranty

  * Device tree is enabled.
  ** Xunlong Orange Pi Zero Lts **
```

- root@orangepi-zero-lts:~# gpio -h
```
gpio: Usage: gpio -v
       gpio -h
       gpio [-g|-1] [-x extension:params] ...
       gpio <toggle/blink> <pin>
       gpio <read/write/aread/awritewb/pwm/mode> ...
       gpio readall/nreadall/allreadall/reset
       gpio unexportall/exports
       gpio export/edge/unexport ...
       gpio pwm-bal/pwm-ms 
       gpio pwmr <range> 
       gpio pwmc <divider> 
       gpio load spi/i2c
       gpio i2cd/i2cdetect
       gpio usbp high/low
```

- root@orangepi-zero-lts:~# gpio readall
```
+-----+-----+-------  Xunlong Orange Pi Zero Lts           --------+-----+-----+
| H2+ | wPi |   Name   | Mode | V | Physical | V | Mode | Name     | wPi | H2+ |
+-----+-----+----------+------+---+----++----+---+------+----------+-----+-----+
|     |     |    +3.3v |      |   |  1 || 2  |   |      |      +5v |     |     |
|  12 |  12 | PA12 sda | ALT3 | 0 |  3 || 4  |   |      |      +5v |     |     |
|  11 |  11 | PA11 scl | ALT3 | 0 |  5 || 6  |   |      |      GND |     |     |
|   6 |   6 | PA6      | ALT3 | 0 |  7 || 8  | 0 | ALT3 |  PG6     | 198 | 198 |
|     |     |      GND |      |   |  9 || 10 | 0 | ALT3 |  PG7     | 199 | 199 |
|   1 |   1 | PA1      | ALT3 | 0 | 11 || 12 | 0 | ALT3 |  PA7     | 7   | 7   |
|   0 |   0 | PA0      | ALT3 | 0 | 13 || 14 |   |      |      GND |     |     |
|   3 |   3 | PA3      | ALT3 | 0 | 15 || 16 | 0 | ALT3 |  PA19    | 19  | 19  |
|     |     |    +3.3v |      |   | 17 || 18 | 0 | ALT3 |  PA18    | 18  | 18  |
|  15 |  15 | PA15     | ALT3 | 0 | 19 || 20 |   |      |      GND |     |     |
|  16 |  16 | PA16     | ALT3 | 0 | 21 || 22 | 0 | ALT3 |  PA2     | 2   | 2   |
|  14 |  14 | PA14     | ALT3 | 0 | 23 || 24 | 0 | ALT3 |  PA13    | 13  | 13  |
|     |     |      GND |      |   | 25 || 26 | 0 | ALT3 |  PA10    | 10  | 10  |
+-----+-----+----------+------+---+---LEDs---+---+------+----------+-----+-----+
|  17 |  17 | RED-LED  |  OUT | 0 | 27 || 28 | 0 | IN   |  PWR-LED | 362 | 362 |
+-----+-----+----------+------+---+-----+----+---+------+----------+-----+-----+
| H2+ | wPi |   Name   | Mode | V | Physical | V | Mode | Name     | wPi | H2+ |
+-----+-----+-------  Xunlong Orange Pi Zero Lts           --------+-----+-----+
```

-  root@orangepi-zero-lts:~# gpio allreadall
```
 +----------+-----+------+-------+
 |   GPIO   | wPi | Mode | Value |
 +----------+-----+------+-------+
 | gpio-0   | 0   | ALT3 | Low   |
 | gpio-1   | 1   | ALT3 | Low   |
 | gpio-2   | 2   | ALT3 | Low   |
 | gpio-3   | 3   | ALT3 | Low   |
 | gpio-6   | 6   | ALT3 | Low   |
 | gpio-7   | 7   | ALT3 | Low   |
 | gpio-10  | 10  | ALT3 | Low   |
 | gpio-11  | 11  | ALT3 | Low   |
 | gpio-12  | 12  | ALT3 | Low   |
 | gpio-13  | 13  | ALT3 | Low   |
 | gpio-14  | 14  | ALT3 | Low   |
 | gpio-15  | 15  | ALT3 | Low   |
 | gpio-16  | 16  | ALT3 | Low   |
 | gpio-17  | 17  | OUT  | Low   |
 | gpio-18  | 18  | ALT3 | Low   |
 | gpio-19  | 19  | ALT3 | Low   |
 | gpio-198 | 198 | ALT3 | Low   |
 | gpio-199 | 199 | ALT3 | Low   |
 | gpio-362 | 362 | IN   | Low   |
 +----------+-----+------+-------+
```

- root@orangepi-zero-lts:~# gpio mode 3 in
- root@orangepi-zero-lts:~# gpio mode 198 out
- root@orangepi-zero-lts:~# gpio write 198 1
- root@orangepi-zero-lts:~# gpio allreadall
```
 +----------+-----+------+-------+
 |   GPIO   | wPi | Mode | Value |
 +----------+-----+------+-------+
 | gpio-0   | 0   | ALT3 | Low   |
 | gpio-1   | 1   | ALT3 | Low   |
 | gpio-2   | 2   | ALT3 | Low   |
 | gpio-3   | 3   | IN   | Low   |
 | gpio-6   | 6   | ALT3 | Low   |
 | gpio-7   | 7   | ALT3 | Low   |
 | gpio-10  | 10  | ALT3 | Low   |
 | gpio-11  | 11  | ALT3 | Low   |
 | gpio-12  | 12  | ALT3 | Low   |
 | gpio-13  | 13  | ALT3 | Low   |
 | gpio-14  | 14  | ALT3 | Low   |
 | gpio-15  | 15  | ALT3 | Low   |
 | gpio-16  | 16  | ALT3 | Low   |
 | gpio-17  | 17  | OUT  | Low   |
 | gpio-18  | 18  | ALT3 | Low   |
 | gpio-19  | 19  | ALT3 | Low   |
 | gpio-198 | 198 | OUT  | High  |
 | gpio-199 | 199 | ALT3 | Low   |
 | gpio-362 | 362 | IN   | Low   |
 +----------+-----+------+-------+
root@orangepi-zero-lts:~# gpio readall

+-----+-----+-------  Xunlong Orange Pi Zero Lts           --------+-----+-----+
| H2+ | wPi |   Name   | Mode | V | Physical | V | Mode | Name     | wPi | H2+ |
+-----+-----+----------+------+---+----++----+---+------+----------+-----+-----+
|     |     |    +3.3v |      |   |  1 || 2  |   |      |      +5v |     |     |
|  12 |  12 | PA12 sda | ALT3 | 0 |  3 || 4  |   |      |      +5v |     |     |
|  11 |  11 | PA11 scl | ALT3 | 0 |  5 || 6  |   |      |      GND |     |     |
|   6 |   6 | PA6      | ALT3 | 0 |  7 || 8  | 1 | OUT  |  PG6     | 198 | 198 |
|     |     |      GND |      |   |  9 || 10 | 0 | ALT3 |  PG7     | 199 | 199 |
|   1 |   1 | PA1      | ALT3 | 0 | 11 || 12 | 0 | ALT3 |  PA7     | 7   | 7   |
|   0 |   0 | PA0      | ALT3 | 0 | 13 || 14 |   |      |      GND |     |     |
|   3 |   3 | PA3      |   IN | 0 | 15 || 16 | 0 | ALT3 |  PA19    | 19  | 19  |
|     |     |    +3.3v |      |   | 17 || 18 | 0 | ALT3 |  PA18    | 18  | 18  |
|  15 |  15 | PA15     | ALT3 | 0 | 19 || 20 |   |      |      GND |     |     |
|  16 |  16 | PA16     | ALT3 | 0 | 21 || 22 | 0 | ALT3 |  PA2     | 2   | 2   |
|  14 |  14 | PA14     | ALT3 | 0 | 23 || 24 | 0 | ALT3 |  PA13    | 13  | 13  |
|     |     |      GND |      |   | 25 || 26 | 0 | ALT3 |  PA10    | 10  | 10  |
+-----+-----+----------+------+---+---LEDs---+---+------+----------+-----+-----+
|  17 |  17 | RED-LED  |  OUT | 0 | 27 || 28 | 0 | IN   |  PWR-LED | 362 | 362 |
+-----+-----+----------+------+---+-----+----+---+------+----------+-----+-----+
| H2+ | wPi |   Name   | Mode | V | Physical | V | Mode | Name     | wPi | H2+ |
+-----+-----+-------  Xunlong Orange Pi Zero Lts           --------+-----+-----+
```

