#!/bin/sh
if [ -z "$1" ]; then
echo "Content-type: text/html; charset=utf-8"
fi
echo 

confFile='termostat-serial-arduino'
b=$'/tmp/'
strok=$(uci get $confFile.@displ_tip[0].strok)
f=$(uci get port_listen.@displ_tip[0].t_file)
d=$(uci get port_listen.@displ_tip[0].devPort)

a=0
uci_commit=0

while [ "$a" -le 3 ]; do
rm $b$f &> /dev/null
i=1
    if [ "$a" -eq 0 ]; then
	echo r+=setN/ > /dev/tty$d
	test=$(uci get $confFile.@arduino[0].name_snr10 &> /dev/null)
    elif [ "$a" -eq 1 ]; then
	echo r+=setM/ > /dev/tty$d
	test=$(uci get $confFile.@arduino[0].MAC10 &> /dev/null)
    elif [ "$a" -eq 2 ]; then
	echo r+=setD > /dev/tty$d
	test=&(awk -F : '/'Nick':/ {print $1}' $b$f &> /dev/null)
    elif [ "$a" -eq 3 ]; then
	echo r+=setS > /dev/tty$d
	test=&(awk -F : '/'i2c':/ {print $1}' $b$f &> /dev/null)
    fi
#
    we=0
    while [ "$we" -ne 1 ]; do
     cat $b$f &> /dev/null
     if [ $? != 0 ]; then
      we=0
     else
      echo $test
      if [ $? != 1 ]; then
       we=1
      fi
     fi
    done
#
    sed -i 's/[ \t]*$//' $b$f # Удалить все пробелы и символы табуляции в конце каждой строки

  if [ "$a" -eq 0 ] || [ "$a" -eq 1 ]; then
     while [ "$i" -le 10 ]; do
	let "ii = (("$i" - 1))"
	if [ "$a" -eq 0 ]; then
		name_conf=$(uci get $confFile.@arduino[0].name_snr$i)
		name_ardu=$(awk -F : '/'$ii':/ {print $2}' $b$f)
		test=$(uci get $confFile.@arduino[0].name_snr$i &> /dev/null)
	elif [ "$a" -eq 1 ]; then
		mac_conf=$(uci get $confFile.@arduino[0].MAC$i)
		mac_ardu=$(awk -F : '/'$ii':/ {print $2}' $b$f)
		test=$(uci get $confFile.@arduino[0].MAC$i &> /dev/null)
	fi
	echo $test
	if [ $? != 0 ]; then
		: $((uci_commit = $uci_commit + 1))
		if [ "$a" -eq 0 ]; then
			uci set $confFile.@arduino[0].name_snr$i=$name_ardu
		elif [ "$a" -eq 1 ]; then
			uci set $confFile.@arduino[0].MAC$i=$mac_ardu
		fi

	elif [ "$a" -eq 0 ]; then
	    if [ "$name_ardu" != "$name_conf" ]; then
		: $((uci_commit = $uci_commit + 1))
		uci set $confFile.@arduino[0].name_snr$i=$name_ardu
	    fi
	elif [ "$a" -eq 1 ]; then
	    if [ "$mac_ardu" != "$mac_conf" ]; then
		: $((uci_commit = $uci_commit + 1))
		uci set $confFile.@arduino[0].MAC$i=$mac_ardu
	    fi
	fi
     : $((i = $i + 1))
     done
  elif [ "$a" -eq 2 ]; then
# НАЗВАНИЕ
	prbor_conf=$(uci get $confFile.@displ_tip[0].prbor)
	prbor_ardu=$(awk -F : '/'Nick':/ {print $2}' $b$f)
	uci get $confFile.@displ_tip[0].prbor &> /dev/null
	if [ $? != 0 ] || [ "$prbor_ardu" != "$prbor_conf" ]; then
		: $((uci_commit = $uci_commit + 1))
		uci set $confFile.@displ_tip[0].prbor=$prbor_ardu
	fi
  elif [ "$a" -eq 3 ]; then
     while [ "$i" -le 4 ]; do
# alarm
	let "ii = (("$i" - 1))"
	alarm_conf=$(uci get $confFile.@arduino[0].alarm$i)
	alarm_ardu=$(awk -F : '/'T$ii':/ {print $2}' $b$f)
	uci get $confFile.@arduino[0].alarm$i &> /dev/null
	if [ $? != 0 ] || [ "$alarm_ardu" != "$alarm_conf" ]; then
		: $((uci_commit = $uci_commit + 1))
		uci set $confFile.@arduino[0].alarm$i=$alarm_ardu
	fi
# histe
	histe_conf=$(uci get $confFile.@arduino[0].histe$i)
	histe_ardu=$(awk -F : '/'T$ii':/ {print $3}' $b$f)
	uci get $confFile.@arduino[0].histe$i &> /dev/null
	if [ $? != 0 ] || [ "$histe_ardu" != "$histe_conf" ]; then
		: $((uci_commit = $uci_commit + 1))
		uci set $confFile.@arduino[0].histe$i=$histe_ardu
	fi
# Tipe
	let "ii = (("$i" + 1))"
	tip_ds_conf=$(uci get $confFile.@arduino[0].tip_ds$i)
	tip_ds_ardu=$(awk -F : '/'P':/ {print $'$ii'}' $b$f)
	uci get $confFile.@arduino[0].tip_ds$i &> /dev/null
	if [ $? != 0 ] || [ "$tip_ds_ardu" != "$tip_ds_conf" ]; then
		: $((uci_commit = $uci_commit + 1))
		uci set $confFile.@arduino[0].tip_ds$i=$tip_ds_ardu
	fi
# nGp3_out
	nGp_out_conf=$(uci get $confFile.@arduino[0].nGp"$i"_out)
	nGp_out_ardu=$(awk -F : '/'Gpi':/ {print $'$ii'}' $b$f)
	uci get $confFile.@arduino[0].nGp"$i"_out &> /dev/null
	if [ $? != 0 ] || [ "$nGp_out_ardu" != "$nGp_out_conf" ]; then
		: $((uci_commit = $uci_commit + 1))
		uci set $confFile.@arduino[0].nGp"$i"_out=$nGp_out_ardu
	fi
# invers
	invers_conf=$(uci get $confFile.@arduino[0].invers$i)
	invers_ardu=$(awk -F : '/'Inv':/ {print $'$ii'}' $b$f)
	uci get $confFile.@arduino[0].invers$i &> /dev/null
	if [ $? != 0 ] || [ "$invers_ardu" != "$invers_conf" ]; then
		: $((uci_commit = $uci_commit + 1))
		uci set $confFile.@arduino[0].invers$i=$invers_ardu
	fi
     : $((i = $i + 1))
     done
  fi
: $((a = $a + 1))
done
# displ_tip-simvol
	simvol_conf=$(uci get $confFile.@displ_tip[0].simvol)
	simvol_ardu=$(awk -F : '/'LCD':/ {print $2}' $b$f)
	uci get $confFile.@displ_tip[0].simvol &> /dev/null
	if [ $? != 0 ] || [ "$simvol_ardu" != "$simvol_conf" ]; then
		: $((uci_commit = $uci_commit + 1))
		uci set $confFile.@displ_tip[0].simvol=$simvol_ardu
	fi
# displ_tip-strok
	strok_conf=$(uci get $confFile.@displ_tip[0].strok)
	strok_ardu=$(awk -F : '/'LCD':/ {print $3}' $b$f)
	uci get $confFile.@displ_tip[0].strok &> /dev/null
	if [ $? != 0 ] || [ "$strok_ardu" != "$strok_conf" ]; then
		: $((uci_commit = $uci_commit + 1))
		uci set $confFile.@displ_tip[0].strok=$strok_ardu
	fi
# w1_gpio
	w1_gpio_conf=$(uci get $confFile.@arduino[0].w1_gpio)
	w1_gpio_ardu=$(awk -F : '/'1Wr':/ {print $2}' $b$f)
	uci get $confFile.@arduino[0].w1_gpio &> /dev/null
	if [ $? != 0 ] || [ "$w1_gpio_ardu" != "$w1_gpio_conf" ]; then
		: $((uci_commit = $uci_commit + 1))
		uci set $confFile.@arduino[0].w1_gpio=$w1_gpio_ardu
	fi
# i2c_adres-LCD
	i2c_adres_conf=$(uci get $confFile.@displ_tip[0].i2c_adres)
	i2c_adres_ardu=$(awk -F : '/'i2c':/ {print $2}' $b$f)
	uci get $confFile.@displ_tip[0].i2c_adres &> /dev/null
	if [ $? != 0 ] || [ "$i2c_adres_ardu" != "$i2c_adres_conf" ]; then
		: $((uci_commit = $uci_commit + 1))
		uci set $confFile.@displ_tip[0].i2c_adres=$i2c_adres_ardu
	fi

if [ "$uci_commit" -ne 0 ]; then
uci commit $confFile
fi
rm $b$f &> /dev/null

if [ -z "$1"]; then
 /www/cgi-bin/modules/termostat-serial-arduino/setupMenu.cgi
elif [ "$1" == 1 ]; then
# devPort=$(uci get port_listen.@displ_tip[0].devPort)
# echo e+=end/ > /dev/tty$devPort
# /www/cgi-bin/modules/termostat-serial-arduino/setupMenu.cgi
# /etc/init.d/port_listen.init restart

 /www/cgi-bin/modules/termostat-serial-arduino/index.cgi 0
else
 /www/cgi-bin/modules/termostat-serial-arduino/setupMenu.cgi
fi

