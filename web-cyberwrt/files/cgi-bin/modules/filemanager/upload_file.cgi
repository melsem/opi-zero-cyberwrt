#!/bin/sh
#Copyright (C) 2013 cyberwrt.ru, carduino.ru, cyber-place.ru, mp3car.ru
echo -e "Content-type: text/html; charset=utf-8\n"
TMP_FILE="/tmp/_$$"
cat > $TMP_FILE
File=`head -n2 $TMP_FILE | grep "filename=" | sed -e "s/.*filename=\"\(.*\)\".*/\1/"`
path=`head -n2 $TMP_FILE | grep 'name="&&&' | sed -e 's/.*name="&&&//' -e 's/".*//'`
#echo $path$File
#echo "<br>"
#echo `cat $TMP_FILE`
NLINES=`wc -l $TMP_FILE | awk '{print $1}'`
head -n$(($NLINES-1)) $TMP_FILE | tail -n$((NLINES-5)) > /$path$File
rm -f $TMP_FILE
echo "<script>window.location.href='index.cgi?$path';</script>"