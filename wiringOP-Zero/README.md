# Only for OrangePi-Zero. Package from OpenWrt.
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

## Download
    git clone https://github.com/melsem/WiringOP-Zero.git
## Installation
    cd WiringOP-Zero
    chmod +x ./build
    sudo ./build

## Test
```root@orangepi-zero-lts:~# gpio -v
gpio version: 2.60
Copyright (c) 2012-2014 Gordon Henderson
This is free software with ABSOLUTELY NO WARRANTY.
For details type: gpio -warranty

  * Device tree is enabled.
  ** Xunlong Orange Pi Zero Lts **
root@orangepi-zero-lts:~# gpio readall

  +------+-----+----------+  Xunlong Orange Pi Zero Lts    +----------+-----+------+
  | GPIO | wPi |   Name   | Mode | V | Physical | V | Mode | Name     | wPi | GPIO |
  +------+-----+----------+------+---+----++----+---+------+----------+-----+------+
  |      |     |    +3.3v |      |   |  1 || 2  |   |      |      +5v |     |      |
  |   12 |  12 | PA12 sda |   IN | 1 |  3 || 4  |   |      |      +5v |     |      |
  |   11 |  11 | PA11 scl |   IN | 1 |  5 || 6  |   |      |      GND |     |      |
  |    6 |   6 | PA6      |   IN | 0 |  7 || 8  | 0 | IN   |  PG6     | 198 | 198  |
  |      |     |      GND |      |   |  9 || 10 | 0 | IN   |  PG7     | 199 | 199  |
  |    1 |   1 | PA1      |   IN | 0 | 11 || 12 | 0 | IN   |  PA7     | 7   | 7    |
  |    0 |   0 | PA0      |   IN | 0 | 13 || 14 |   |      |      GND |     |      |
  |    3 |   3 | PA3      |   IN | 0 | 15 || 16 | 1 | IN   |  PA19    | 19  | 19   |
  |      |     |    +3.3v |      |   | 17 || 18 | 1 | IN   |  PA18    | 18  | 18   |
  |   15 |  15 | PA15     |   IN | 0 | 19 || 20 |   |      |      GND |     |      |
  |   16 |  16 | PA16     |   IN | 0 | 21 || 22 | 0 | IN   |  PA2     | 2   | 2    |
  |   14 |  14 | PA14     |   IN | 0 | 23 || 24 | 0 | IN   |  PA13    | 13  | 13   |
  |      |     |      GND |      |   | 25 || 26 | 0 | IN   |  PA10    | 10  | 10   |
  +------+-----+----------+------+---+----++----+---+------+----------+-----+------+
  |   17 |  17 | RED-LED  |  OUT | 1 | 27 || 28 | 0 | ALT4 | uart-PWM | 5   | 5    |
  +------+-----+----------+------+---+----++----+---+------+----------+-----+------+
  | GPIO | wPi |   Name   | Mode | V | Physical | V | Mode | Name     | wPi | GPIO |
  +------+-----+----------+  Xunlong Orange Pi Zero Lts    +----------+-----+------+

root@orangepi-zero-lts:~# gpio mode 198 out
root@orangepi-zero-lts:~# gpio mode 199 out
root@orangepi-zero-lts:~# gpio mode 19 in
root@orangepi-zero-lts:~# gpio mode 7 up
root@orangepi-zero-lts:~# gpio readall

  +------+-----+----------+  Xunlong Orange Pi Zero Lts    +----------+-----+------+
  | GPIO | wPi |   Name   | Mode | V | Physical | V | Mode | Name     | wPi | GPIO |
  +------+-----+----------+------+---+----++----+---+------+----------+-----+------+
  |      |     |    +3.3v |      |   |  1 || 2  |   |      |      +5v |     |      |
  |   12 |  12 | PA12 sda |  OFF | 0 |  3 || 4  |   |      |      +5v |     |      |
  |   11 |  11 | PA11 scl |  OFF | 0 |  5 || 6  |   |      |      GND |     |      |
  |    6 |   6 | PA6      |  OFF | 0 |  7 || 8  | 0 | OUT  |  PG6     | 198 | 198  |
  |      |     |      GND |      |   |  9 || 10 | 0 | OUT  |  PG7     | 199 | 199  |
  |    1 |   1 | PA1      |  OFF | 0 | 11 || 12 | 0 | IN   |  PA7     | 7   | 7    |
  |    0 |   0 | PA0      |  OFF | 0 | 13 || 14 |   |      |      GND |     |      |
  |    3 |   3 | PA3      |  OFF | 0 | 15 || 16 | 1 | IN   |  PA19    | 19  | 19   |
  |      |     |    +3.3v |      |   | 17 || 18 | 0 | OFF  |  PA18    | 18  | 18   |
  |   15 |  15 | PA15     |  OFF | 0 | 19 || 20 |   |      |      GND |     |      |
  |   16 |  16 | PA16     |  OFF | 0 | 21 || 22 | 0 | OFF  |  PA2     | 2   | 2    |
  |   14 |  14 | PA14     |  OFF | 0 | 23 || 24 | 0 | OFF  |  PA13    | 13  | 13   |
  |      |     |      GND |      |   | 25 || 26 | 0 | OFF  |  PA10    | 10  | 10   |
  +------+-----+----------+------+---+----++----+---+------+----------+-----+------+
  |   17 |  17 | RED-LED  |  OUT | 1 | 27 || 28 | 0 | ALT4 | uart-PWM | 5   | 5    |
  +------+-----+----------+------+---+----++----+---+------+----------+-----+------+
  | GPIO | wPi |   Name   | Mode | V | Physical | V | Mode | Name     | wPi | GPIO |
  +------+-----+----------+  Xunlong Orange Pi Zero Lts    +----------+-----+------+

root@orangepi-zero-lts:~# gpio mode 7 up
root@orangepi-zero-lts:~# gpio allreadall
  +----------+-----+------+-------+
  |   GPIO   | wPi | Mode | Value |
  +----------+-----+------+-------+
  | gpio-0   | 0   | OFF  | Low   |
  | gpio-1   | 1   | OFF  | Low   |
  | gpio-2   | 2   | OFF  | Low   |
  | gpio-3   | 3   | OFF  | Low   |
  | gpio-5   | 5   | ALT4 | Low   |
  | gpio-6   | 6   | OFF  | Low   |
  | gpio-7   | 7   | IN   | High  |
  | gpio-10  | 10  | OFF  | Low   |
  | gpio-11  | 11  | OFF  | Low   |
  | gpio-12  | 12  | OFF  | Low   |
  | gpio-13  | 13  | OFF  | Low   |
  | gpio-14  | 14  | OFF  | Low   |
  | gpio-15  | 15  | OFF  | Low   |
  | gpio-16  | 16  | OFF  | Low   |
  | gpio-17  | 17  | OUT  | High  |
  | gpio-18  | 18  | OFF  | Low   |
  | gpio-19  | 19  | IN   | High  |
  | gpio-198 | 198 | OUT  | Low   |
  | gpio-199 | 199 | OUT  | Low   |
  | gpio-362 | 362 | IN   | Low   |
  +----------+-----+------+-------+
root@orangepi-zero-lts:~# gpio -h
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

                     mode pin mode
       gpio mode <pin> <in/input>           (pinMode - pin, INPUT)
       gpio mode <pin> <out/output>  (pinMode - pin, OUTPUT)

       gpio mode <pin> <up>             (pullUpDnControl - pin, PUD_UP)
       gpio mode <pin> <down>           (pullUpDnControl - pin, PUD_DOWN)
       gpio mode <pin> <tri/off>        (pullUpDnControl - pin, PUD_OFF)

       gpio mode <pin> <alt0>           (pinModeAlt      (pin, 0b100)
       gpio mode <pin> <alt1>           (pinModeAlt      (pin, 0b101)
       gpio mode <pin> <alt2>           (pinModeAlt      (pin, 0b110)
       gpio mode <pin> <alt3/disable>   (pinModeAlt      (pin, 0b111)
       gpio mode <pin> <alt4/enable>    (sunxi H3 pin-5, pwm-ON...0b011)
       gpio mode <pin> <alt5>           (pinModeAlt      (pin, 0b010)
```

Thanks to zhaolei, who created the base libary for this one!


