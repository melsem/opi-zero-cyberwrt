#!/bin/sh
sleep 2s
rm /www/cgi-bin/modules/i2c_hd44780/index.cgi
rm /www/cgi-bin/modules/i2c_hd44780/install.cgi
rm /www/cgi-bin/modules/i2c_hd44780/pp.cgi
opkg update
opkg install i2c-tools i2c_HD44780 kmod-i2c-gpio-custom kmod-w1 kmod-w1-master-gpio kmod-w1-gpio-custom kmod-w1-slave-therm digitemp digitemp-usb kmod-usb-serial-pl2303
