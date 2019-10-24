#!/bin/sh
#Copyright (C) 2015 cyberwrt.ru, carduino.ru, cyber-place.ru, mp3car.ru
l=`echo "$QUERY_STRING" | sed -n 's/^.*l=\([^&]*\).*$/\1/p'`
for i in `seq 0 $l` ; do
	m=$(echo "$QUERY_STRING" | sed -n "s/^.*mtd$i=\([^&]*\).*$/\1/p")
	if [ "$m" -gt 0 ] ; then
		s=$s"mtd"$i"_"
	fi
done
mtd=mtdblock1
s=$s"`date +"%d_%m_%Y_%H-%M-%S"`.bin"
echo "Content-Type: application/octet-stream"
echo "Content-Disposition: attachment; filename=$s"
echo
for i in `seq 0 $l` ; do
	m=$(echo "$QUERY_STRING" | sed -n "s/^.*mtd$i=\([^&]*\).*$/\1/p")
	if [ "$m" -gt 0 ] ; then
		cat /dev/mtdblock$i
	fi
done