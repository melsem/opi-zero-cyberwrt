#!/bin/sh
sleep 2s
rm /www/cgi-bin/modules/termostat-serial-arduino/index.cgi
rm /www/cgi-bin/modules/termostat-serial-arduino/install.cgi
rm /www/cgi-bin/modules/termostat-serial-arduino/pp.cgi
opkg update
opkg install port_listen kmod-usb-serial-ch341 kmod-usb-serial-pl2303
