#!/bin/sh
echo "Content-type: text/plain; charset=utf-8"
echo ""

flconf="i2c_HD44780"
on_ds18_val=$(uci -q get $flconf.@displ_tip[0].on_ds18)

if [ "$on_ds18_val" = "1" ]; then
    t1=$(sed -n '1p' /tmp/d_s 2>/dev/null)
    t2=$(sed -n '2p' /tmp/d_s 2>/dev/null)
    t3=$(sed -n '3p' /tmp/d_s 2>/dev/null)
    t4=$(sed -n '4p' /tmp/d_s 2>/dev/null)
    echo "${t1}|${t2}|${t3}|${t4}"

elif [ "$on_ds18_val" = "2" ]; then
    echo "<pre>$(sed -n '1,8p' /tmp/t_h 2>/dev/null)</pre>"

else
    f=$(uci -q get $flconf.@display[0].read_file)
    echo "<pre>$(cat /tmp/$f 2>/dev/null)</pre>"
fi

