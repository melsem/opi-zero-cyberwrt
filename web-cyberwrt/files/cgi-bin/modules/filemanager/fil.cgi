#!/bin/sh
#Copyright (C) 2013 cyberwrt.ru, carduino.ru, cyber-place.ru, mp3car.ru
temp=$(echo $QUERY_STRING | tr '/' '\n' | tail -n1)
echo "Content-type: application/octet-stream; charset=utf-8"
#echo "Content-type: text/html; charset=utf-8"
echo "Content-Disposition: Attachment; filename=$temp"
echo
#echo $# $2
#echo $QUERY_STRING
cat $QUERY_STRING

#wget -q -U "Mozilla/5.0" --post-file /etc/banner --header="Content-Type: application/octet-stream" -O -
#echo `cat /etc/banner`
#echo `wget -O - etc/banner`
#`awk -f /etc/banner >>`
