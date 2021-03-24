#!/bin/sh
echo "Content-type: text/html; charset=utf-8"
if [ -z "$1" ]; then /www/cgi-bin/modules/boot-config/index.html; else /www/cgi-bin/modules/boot-config/index.html 55; fi
