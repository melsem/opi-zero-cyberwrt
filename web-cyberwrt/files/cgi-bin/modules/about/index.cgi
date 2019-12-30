#!/bin/sh
#Copyright (C) 2013 cyberwrt.ru, carduino.ru, cyber-place.ru, mp3car.ru
echo "Content-type: text/html; charset=utf-8"
echo
echo "<title>О системе</title>"
echo `cat /www/menu.html`
board=$(cat /tmp/sysinfo/board_name)
boardname="${board##*,}"
echo "<br><br><b>$boardname</b><br>Free mem:<b> `df -h | sed '/root/!d' | awk '{print $4}'`</b><br>"
if [ "$boardname" == "orangepi-zero-lts" ]; then
echo "<b>`awk '{printf "Themperature: " "%.01f°C\n", $1/500}' /sys/devices/virtual/thermal/thermal_zone0/temp`</b><br>"
else
echo "<b>`awk '{printf "Themperature: " "%.01f°C\n", $1/1000}' /sys/devices/virtual/thermal/thermal_zone0/temp`</b><br>"
fi
echo "<h3>`awk '{printf "Cpufreq: " "%.03f GHz\n", $1/1000000}' /sys/bus/cpu/devices/cpu0/cpufreq/scaling_cur_freq`</h3>"
echo "<h1>Текущая Дата и время</h1><pre>`date`</pre>"
echo "<h1>Версия прошивки</h1><pre>`cat /proc/version`</pre>"
echo "<h1>Информация о CPU</h1><pre>`cat /proc/cpuinfo`</pre>"
echo "<h1>Информация о RAM</h1><pre>`cat /proc/meminfo`</pre>"
echo "<h1>Информация об используемой и свободной ОЗУ и Swap</h1><pre>`free`</pre>"
echo "<h1>Свободное и используемое место в разделах</h1><pre>`df -h`</pre>"
echo "<h1>Информация о сетевых устройствах</h1><pre>`ifconfig`</pre>"
echo "<h1>Настройки WiFi адаптера</h1><pre>`cat /etc/config/wireless`</pre>"
echo "<h1>Настройки сети</h1><pre>`cat /etc/config/network`</pre>"
echo "<h1>Список установленных пакетов</h1><pre>`opkg list-installed`</pre>"

echo "<h1>сообщения ядра</h1><pre>`dmesg`</pre>"

echo "<h1>Глобальные переменые</h1>"
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
