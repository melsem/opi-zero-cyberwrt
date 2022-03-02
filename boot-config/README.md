
# Как всем этим воспользоваться и проверил работает на Sunxi Allwiner sun8i-h3 и sun50i-h5..
- Sunxi Allwiner sun8i-h3 и sun50i-h5 адреса регистров (используемых в boot-config) одинаковые.. Различия только по самим особенностям самой платы.
- Включить в список загрузки аудио-кодек
- root@orangepi-zero-lts:~# boot-config overlays codec
- ======================
- Сами overlays в сыцах: /boot-config/src/dt-overlays
- в orangepi будут /etc/dt-overlays
- Можно добавить и свои overlays в /dt-overlays.

- B 'aliases' и 'moduls' находятся псевдонимы и их имена регистров, по которым и привязываются оверлэи.

root@orangepi-zero-lts:~# boot-config -h
```

 ------------------->  Xunlong Orange Pi Zero Lts           <-------------------
 Convert dtb file to dts:
   Example: boot-config dtb-dts-mmc
 Convert dtb file to dts:
   Example: boot-config dts-dtb-mmc

 Modules that can be controlled by commands: <on/ON/okay> or <off/OFF/disabled>.

   mmc0/mmc1/mmc2/ir/pwm/i2c0/i2c1/i2c2/camera/audcodec
   i2s0/i2s1/uart0/uart1/uart2/otg/host/peripheral/spdif
   ehci0/ohci0/ehci1/ohci1/ehci2/ohci2/ehci3/ohci3
   usb0/usb1/usb2/usb3/spi0/spi1/ethernet0
   otg_tipe/host_tipe/peripheral_tipe
   w1/pps/i2c4/gpio-keys-user/rotary/rotary_button/rotary_axis

   Usage:   boot-config <module> <on/ON/okay>
   Or:      boot-config <module> <off/OFF/disabled>
   Example1: boot-config i2c0 on
   Example2: boot-config usb2 off

     ------------------------------------------------
       Example3: boot-config otg_tipe on
       Or:       boot-config host_tipe on
       Or:       boot-config peripheral_tipe on

     Or all USB0 tips disable (otg, host, peripheral).
       example:  boot-config host_tipe off
     ------------------------------------------------

 In the modules 'w1', 'pps' you can still change the pins.
   Add or edit single one-wire, pps contacts.

   Usage:   boot-config -g <module> <gpio number>
   Example4: boot-config -g w1 10

   Usage:   boot-config <module> <gpio name>
   Example5: boot-config w1 PA10

 -------------------------------------------------------------------------------
```

