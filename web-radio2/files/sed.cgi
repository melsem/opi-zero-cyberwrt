#!/bin/sh

m3u="/etc/config/web-radio2"
if [[ "$1" == "addnev" ]]; then
echo -e "#EXTINF:-"$2","\"$3\" >> $m3u
elif [[ "$1" == "delet" ]]; then
cat $m3u > /tmp/web-radio2
m3u="/tmp/web-radio2"
 a=$(($2 * 2))
 ab=$(($a + 1))
sed -i ''$a','$ab'd' $m3u

kollwo=$(sed -n '/#EXTINF:-/P' $m3u | sed -n '$=')
zzz=0
sed -i 's/#EXTINF:-\(.*\),/#EXTINF:-0,/' $m3u
b=1
while [ "$kollwo" -ne  "$zzz" ]; do
sed -i '1,/#EXTINF:-0,/{s/#EXTINF:-0,/#EXTINF:-'$b',/}' $m3u
let "b += 1"
let "zzz += 1"
done
cat $m3u > /etc/config/web-radio2
fi
