#!/bin/sh
#Copyright (C) 2013 cyberwrt.ru, carduino.ru, cyber-place.ru, mp3car.ru
echo "Content-type: text/html; charset=utf-8"
echo
echo "<title>О системе</title>"
echo `cat /www/menu.html`
board=$(cat /tmp/sysinfo/board_name)
boardname="${board##*,}"
df -h | grep overlayfs &> /dev/null
if [ $? != 1 ]; then root='overlayfs'; else root='root'; fi
if [ "$boardname" == "orangepi-zero-lts" ]; then nn='500'; else nn='1000'; fi
echo "<br><br><b>$boardname</b><br>Free mem:<b> `df -h | sed '/'$root'/!d' | awk '{print $4}'`</b><br>"
echo "Themperature: ""<b>`awk '{printf "%.01f°C\n", $1/'$nn'}' /sys/devices/virtual/thermal/thermal_zone0/temp`</b><br>"
echo "Cpufreq: ""<b>`awk '{printf "%.03f GHz\n", $1/1000000}' /sys/bus/cpu/devices/cpu0/cpufreq/scaling_cur_freq`</b><br>"
echo "<h3>Текущая Дата и время</h3><pre>`date`</pre>"
echo "<h3>Версия прошивки</h3><pre>`cat /proc/version`</pre>"
echo "<h3>Информация о CPU</h3><pre>`cat /proc/cpuinfo`</pre>"
echo "<h3>Информация о RAM</h3><pre>`cat /proc/meminfo`</pre>"
echo "<h3>Информация об используемой и свободной ОЗУ и Swap</h3><pre>`free`</pre>"
echo "<h3>Свободное и используемое место в разделах</h3><pre>`df -h`</pre>"
echo "<h3>Информация о сетевых устройствах</h3><pre>`ifconfig`</pre>"
echo "<h3>Настройки WiFi адаптера</h3><pre>`cat /etc/config/wireless`</pre>"
echo "<h3>Настройки сети</h3><pre>`cat /etc/config/network`</pre>"
echo "<h3>Список установленных пакетов</h3><pre>`opkg list-installed`</pre>"
echo "<h3>сообщения ядра</h3><pre>`dmesg`</pre>"
echo "<h3>Глобальные переменые</h3>"
echo "<pre>"
echo SERVER_SOFTWARE = $SERVER_SOFTWARE
echo SERVER_NAME = $SERVER_NAME
echo GATEWAY_INTERFACE = $GATEWAY_INTERFACE
echo SERVER_PROTOCOL = $SERVER_PROTOCOL
echo SERVER_PORT = $SERVER_PORT
echo SERVER_ADMIN = $SERVER_ADMIN
echo SERVER_SIGNATURE = $SERVER_SIGNATURE
echo REQUEST_URI = $REQUEST_URI
echo REQUEST_METHOD = $REQUEST_METHOD
echo QUERY_STRING = $QUERY_STRING
echo REMOTE_HOST = $REMOTE_HOST
echo REMOTE_ADDR = $REMOTE_ADDR
echo HTTP_ACCEPT = $HTTP_ACCEPT
echo HTTP_HOST = $HTTP_HOST
echo HTTP_USER_AGENT = $HTTP_USER_AGENT
echo SCRIPT_NAME = $SCRIPT_NAME
echo SCRIPT_FILENAME = $SCRIPT_FILENAME
echo PATH = $PATH
echo PATH_INFO = $PATH_INFO
echo PATH_TRANSLATED = $PATH_TRANSLATED
echo REMOTE_USER = $REMOTE_USER
echo AUTH_TYPE = $AUTH_TYPE
echo CONTENT_TYPE = $CONTENT_TYPE
echo CONTENT_LENGTH = $CONTENT_LENGTH
echo DOCUMENT_ROOT = $DOCUMENT_ROOT
echo "<pre>"

