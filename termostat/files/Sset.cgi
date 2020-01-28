#!/bin/sh
echo "Content-type: text/html; charset=utf-8"
echo ""
conf_s="ds18b20"
sdds=$(uci get $conf_s.@Gpio1Wr[0].enabled0)
case "$sdds" in
"" ) Tm_metr=selected;;
"1" ) Mr_zilka=selected;;
esac
invertOut () {
if [ "`uci get $conf_s.@Gpio1Wr[0].oInvert`" = "0" ] ; then
aktiv="HIGH"
else
aktiv="LOW"
fi
}
if [ "`uci get $conf_s.@Gpio1Wr[0].search`" = "1" ] ; then
yess="checked"
nyoo=""
else
yess=""
nyoo="checked"
fi
if [ "`uci get $conf_s.@Gpio1Wr[0].enabled0`" = "1" ] ; then
yss="checked"
noo=""
else
yss=""
noo="checked"
fi
if [ "`uci get $conf_s.@Gpio1Wr[0].ftp_svr`" = "1" ] ; then
ssysso="checked"
oonoos=""
else
ssysso=""
oonoos="checked"
fi
if [ "`uci get $conf_s.@Gpio1Wr[0].pu_Skryp`" = "1" ] ; then
ENABLED="Start enabled"
stpd="RUN"
wkl="checked"
otkl=""
else
ENABLED="stopped !!!"
#stpd="stopped"
wkl=""
otkl="checked"
fi
if [ "`uci get $conf_s.@Gpio1Wr[0].pcf8574`" = "1" ] ; then
hkjh="checked"
otpkl=""
else
hkjh=""
otpkl="checked"
fi
if [ "`uci get $conf_s.@Gpio1Wr[0].allset`" = "10" ] ; then
ukukl="checked"
aukul=""
else
ukukl=""
aukul="checked"
fi
if [ "`uci get $conf_s.@Gpio1Wr[0].oInvert`" = "1" ] ; then
ou_tInvert="checked"
no_tInvert=""
else
ou_tInvert=""
no_tInvert="checked"
fi
send_off=""; dva=""; pyat=""; desat=""; pytna=""; dvadc=""; tridc=""; sest=""
case "`uci get $conf_s.@Gpio1Wr[0].times`" in
"send off") send_off=selected;;
"5") pyat=selected;;
"10") desat=selected;;
"15") pytna=selected;;
"20") dvadc=selected;;
"30") tridc=selected;;
"60") sest=selected;;
esac
echo "<head><title>TermostatGpio</title></head>"
#
cat /etc/rc.d/S99ds18b20_init &> /dev/null
if [ $? = 1 ]; then
ontxtoff2=Желаете_включить_авто-запуск_???
ontxtoff=Желаете_отключить_авто-запуск_???
onoff=staron
sohid=Disable
sotxthid=Enable
kart=$'/modules/termostatGpio/b.jpg'
cto=OFF
clor="ff0000"
dsd=""
dsd2=disabled
else
dsd=disabled
dsd2=""
clor="0000ff"
ontxtoff2=Желаете_отключить_авто-запуск_???
ontxtoff=Желаете_включить_авто-запуск_???
onoff=staroff
sohid=Enable
sotxthid=Disable
kart=$'/modules/termostatGpio/r.jpg'
cto=ON
fi
#
echo ""
echo "
<body bgcolor="#f0f0f0">
<style>
  .b1 {
font-size: 15pt;
text-align: center;
padding: 20px 40px;
margin-left: 1px;
cursor: pointer;
border-radius:inherit;
}
</style><table align=center border=0 width=><tr><td align=center><p style="font-size:38px">Sensor DS18B20 Gpio-1Wire termostat</p></td></tr><br><tr><td>First you have to install the packages to get support for kmod-w1, kmod-w1-master-gpio, kmod-w1-gpio-custom, kmod-w1-slave-therm.</td></tr>
<tr><td><br><a href=/cgi-bin/modules/termostatGpio/index.cgi><input type="submit" class="b1" value="Exit-setup" name=></a><a target="_blank" href="http://narodmon.ru"><input type="submit" class="b1" value="narodmon.ru" name=></a> || <b><a target="_blank" title="Описание" href=http://cyber-place.ru/showpost.php?p=11222&postcount=67>Info</a></b> </td></tr>
<form method="POST" action="Setup.cgi"><table align=center border=0 width=550><br><caption><li><b>General setup</b></caption><tr><td><table bgcolor="#ffffff" align=center border=0 width=550><tr><td>1Wire enable - </td><td><input type=radio name=enbled0 value=0 $noo>Off<input type=radio name=enbled0 value=1 $yss>On</td></tr><tr><td>Init 1Wire - port - </td><td><input type=text name="init_port" size=10 value="
uci get $conf_s.@Gpio1Wr[0].gpio1wr
echo "></td></tr><tr><td title="состояние_ON_Только_при_сканировании!!!!!_После_первого_запуска_модуля_переключить_в_состояние_OFF">Auto - search 1wire sensor </td><td title="состояние_ON_Только_при_сканировании!!!!!_После_первого_запуска_модуля_переключить_в_состояние_OFF"><input type=radio name=search value=0 $nyoo >Off<input type=radio name=search value=1 $yess title="состояние_ON_Только_при_сканировании!!!!!_После_первого_запуска_модуля_переключить_в_состояние_OFF">On</td></tr><tr><td>interval min - </td><td><select name=interval><option $send_off>send off</option><option $pyat>5</option><option $desat>10</option><option $pytna>15</option><option $dvadc>20</option><option $tridc>30</option><option $sest>60</option></select></td></tr><tr><td>server down - </td><td><input type=text name="srv_d" size=15 value="
uci get $conf_s.@Gpio1Wr[0].serv_d
echo "></td></tr><tr><td>port - </td><td><input type=text name="srv_port" size=5 value="
uci get $conf_s.@Gpio1Wr[0].serv_port
echo "></td></tr><tr><td>Log -->> Samba-ftp-serv</td><td><input type=radio name=ftp_svr value=0 $oonoos>Off<input type=radio name=ftp_svr value=1 $ssysso>On</td></tr><tr><td></td><td>.</td></tr><tr><th bgcolor="#90f1f1" align=centr>Additional installation add gpio</th><td bgcolor="#90f1f1"></td></tr><tr><td bgcolor="#90f1f1">table setup sensor - </td><td bgcolor="#90f1f1"><input type="radio" name="allset" $aukul value=2>auto<input type="radio" name="allset" value=10 $ukukl>all</td></tr><tr><td bgcolor="#90f1f1" title="Инвертировать уровень выходов">Invert the output level - aktiv:"
invertOut
echo " <b>$aktiv</></td><td bgcolor="#90f1f1"><input type="radio" name="oInvert" value="0" $no_tInvert>No-Invert<input type="radio" name="oInvert" value="1" $ou_tInvert>Invert</td></tr><tr><td bgcolor="#90f1f1" align=centr title="необходимо_устанивить_пакеты__i2c-tools__и__kmod-i2c-gpio-custom"><b>PCF8574</b></td><td bgcolor="#90f1f1" title="необходимо_устанивить_пакеты__i2c-tools__и__kmod-i2c-gpio-custom"><input type=radio name="pcf8574" value=5 $otpkl title="необходимо_устанивить_пакеты__i2c-tools__и__kmod-i2c-gpio-custom">Off. <input type=radio name="pcf8574" value=1 $hkjh title="необходимо_устанивить_пакеты__i2c-tools__и__kmod-i2c-gpio-custom">On</td></tr><tr><td bgcolor="#90f1f1">gpio-pin I2C <b>SDA-SCL</b></td><td bgcolor="#90f1f1"><input type=text name="sda_scl_i2c" size=6 value="
uci get $conf_s.@Gpio1Wr[0].sda_scl
echo "></td></tr><tr><td bgcolor="#90f1f1"> hex adress </td><td bgcolor="#90f1f1">0x<input type=text name="adres_i2c" size=3 value="
uci get $conf_s.@Gpio1Wr[0].hexadr
echo "></td></tr><tr><td bgcolor="#90f1f1">PCF8574 add 8 pin gpio .</td><td bgcolor="#90f1f1"> "View gpio out ."<input type=text name="innf" size=5 readonly value="
fgfd=$(uci get $conf_s.@Gpio1Wr[0].hexadr)
sed '/GPIOs/!d; /pcf8574/!d; /0-00'$fgfd'/!d' /sys/kernel/debug/gpio | awk '{print $2}'
echo "></td></tr><tr><td bgcolor="#90f1f1" align=center><b><blink>$ENABLED </blink>script -</b><input type=radio name="pu_Skryp" value=0 $otkl>STOP <input type=radio name="pu_Skryp" value=1 $wkl>RUN</td><td bgcolor="#90f1f1"><input type=text name="imya_skript" size=20 readonly value="
uci get $conf_s.@Gpio1Wr[0].imya_skript
echo "></td></tr></table></td></tr></table><table align=center border=0 width=550><tr><td align=center><br><button name="commit" class="b1" type=submit title="Сохранить_???">Save</button></td></tr></table></table></form><br><br>"
echo "<table align=center border=0 width=550><form action="Setup.cgi" method="post"><tr><td align=center><b>Script text:</b> <blink>$ENABLED</blink><p><textarea rows="3" cols="64" name="text" spellcheck="false" placeholder=Text-print? >"
vcvc=$(uci get $conf_s.@Gpio1Wr[0].imya_skript)
cat /usr/sbin/$vcvc &> /dev/null
if [ $? = 0 ]; then
sed -n '/^.\{1\}/p' /usr/sbin/$vcvc
fi
echo "</textarea></td></tr><tr><td align=center><br><input type="submit" class="b1" value="Save-Script-text" name=></p></td></tr></form></table><br><br><table align=center border=0 width=550><tr><td align=center><p style="color:#$clor">Auto Startup - $cto</p></td></tr></table><table align=center border=0 width=550><form action="Setup.cgi" method="post"><tr><td align=center>"
if [ "$onoff" == "staron" ]; then
echo "<button $dsd name="$onoff" type=submit class="b1" title="$ontxtoff"><img src=/modules/termostatGpio/b.jpg alt= style="vertical-align: middle"> "Enable"</button>"
else
echo "<button $dsd2 name="$onoff" type=submit class="b1" title="$ontxtoff2"><img src=/modules/termostatGpio/r.jpg alt= style="vertical-align: middle"> "Disable"</button>"
fi
#
echo "</td></tr></form></table><br><br><table align=center border=0 width=550><form method="POST" action="Setup.cgi"><tr><td align=center>"
gppwr=$(grep gpio1wr /etc/config/ds18b20 | awk '{print $3}'); gppwr1=$(echo "${gppwr} = ${#gppwr}" | awk '{print $3}'); gppwr12=$(echo $(( ${gppwr1} - 2 ))); gp1Wr=$(echo ${gppwr:1:$gppwr12})
kwod=$(ls -l /sys/devices/w1_bus_master1 | sed '/drw/!d; /"*28-*/!d' | awk '{print $9}' | sed -n '$=')
ps > /tmp/1.ds18
grep -q ds18b20 /tmp/1.ds18
if [ $? -eq 1 ]; then
echo "<h3 align="center">The program does not work<br><button name="cozapusk" class="b1" type=submit title="Желаете_запусить_модуль_???"><img src="/modules/termostatGpio/b.jpg" alt= style="vertical-align: middle">Termostat START</button></h3>"
else
echo "<h3 align="center">The program works with temperature. Number of found sensors - $kwod pieces.  Init.Port-"$gp1Wr"<br> <button name="coostanov" class="b1" type=submit title="Желаете_остановить_модуль_???"><img src="/modules/termostatGpio/r.jpg" alt= style="vertical-align: middle">Termostat STOP</button></h3>"
fi
rm /tmp/1.ds18
echo "<td><tr></form></table><br><br>"
# Вторая таблица
conf_s="ds18b20"
if [ "`uci get $conf_s.@Gpio1Wr[0].allset`" = "10" ] ; then
kwod=10
else
kwod=$(ls -l /sys/devices/w1_bus_master1 | sed '/drw/!d; /"*28-*/!d' | awk '{print $9}' | sed -n '$=')
fi
kolllw=0
while [ "$kolllw" -ne  "$kwod" ]; do
if [ "`uci get $conf_s.@ds18b20_s$kolllw[0].enab_nar`" = "1" ] ; then
clr="f1e1f1"
yes="checked"
no=""
otprn=ON
dssd=disabled
dssd2=""
else
dssd=""
dssd2=disabled
otprn=OFF
clr="ffffff"
yes=""
no="checked"
fi
trmo=""; trmst=""; kndc=""; morz=""
case "`uci get $conf_s.@ds18b20_s$kolllw[0].tipsensr`" in
"termometr") trmo=selected;;
"termostat") trmst=selected;;
"kondicioner") kndc=selected;;
"morozilka") morz=selected;;
esac
logns=$(uci get $conf_s.@ds18b20_s$kolllw[0].logns)
echo " <form method="POST" action="Sens$kolllw.cgi"><table align=center border=0 width=><tr><td align=center><p style="font-size:25px"><b> $logns </b><input type=text name="logns" size=18 value="
uci get $conf_s.@ds18b20_s$kolllw[0].logns
echo "> Sensor:$kolllw <b>"
uci get $conf_s.@ds18b20_s$kolllw[0].tipsensr
echo "</b></p></td></tr><table bgcolor="$clr" align=center border=1 width=><tr><td align=center>ID:</td><td align=center><b>"
uci get $conf_s.@Gpio1Wr[0].serv_d
echo "</b></td><td align=center title="Тип_устройства">Device</td><td align=center title="Номер_GPIO_вывода._Для_управления_включением/отключением_нагревательных_элементов_Термо-регулятора____или_Для_управления_включением/отключением_двигателей_компрессора_холодильника/кондиционера.">gpio</td><td></td><td align=center>Hist:</td><td align=center>DIR:</td></tr><tr height="10"><td><input type=text name="id_dev" size=17 value="
uci get $conf_s.@ds18b20_s$kolllw[0].id_sensor
echo "></td><td align=center><input $dssd2 type=radio name="narmon$kolllw" title="Отключить_отправку_показаний_с_датчика" value=0 $no>Off<input $dssd type=radio name="narmon$kolllw" title="Включить_отправку_показаний_с_датчика" value=1 $yes>On</td><td align=center><select name=tipsensr><option $trmo>termometr</option><option $trmst>termostat</option><option $kndc>kondicioner</option><option $morz>morozilka</option></select></td><td align=center title="Номер_GPIO_вывода._Для_управления_включением/отключением_нагревательных_элементов_Термо-регулятора____или_Для_управления_включением/отключением_двигателей_компрессора_холодильника/кондиционера."><input type=text name="pwut" size=3 maxlength="3" value="
uci get $conf_s.@ds18b20_s$kolllw[0].pwout
echo " ></td><td></td><td align=center><input type=text name="hster" size=2 value="
uci get $conf_s.@ds18b20_s$kolllw[0].hister
echo ">&deg;C</td><td align=center><input type=text name="flder" size=18 value="
uci get $conf_s.@ds18b20_s$kolllw[0].folder
echo "></td></tr></table><table align=center border=0 width=><tr><td>--------------</td></tr></table><table bgcolor="$clr" align=center border=1 width=><tr><td></td><td width=150 bgcolor="#90f1f1" align="center" title="День_недели">"The day of the week"</td><td></td><td width=135 align=center bgcolor="#8891f1" title="Температура_отключения_c_00:00_до_Time1">temperature with 00:00 to Time1</td><td width=100 align=center bgcolor="#90f1f1" title="Таймер_№1">Time1</td><td width=135 align=center bgcolor="#8891f1" title="Температура_отключения_№1">Temperature 1:</td><td></td><td width=100 align=center bgcolor="#90f1f1" title="Таймер_№2">Time2</td><td width=135 align=center bgcolor="#8891f1" title="Температура_отключения_№2">Temperature 2:</td><td></td><td width=100 align=center bgcolor="#90f1f1" title="Таймер_№3">Time3</td><td width=135 align=center bgcolor="#8891f1" title="Температура_отключения_№3">Temperature 3:</td><td></td></tr>"
# Таймеры на 7 дней
nedel=1
while [ "$nedel" -ne "8" ]; do
if [ "$nedel" = "1" ]; then
dennedel="Monday"; den_comm="Понедельник"; m1ime="m1ime"; t1ime="t1ime"; gradus="gradus"; t2ime="t2ime"; m2ime="m2ime"; gra2dus="gra2dus"; t3ime="t3ime"; m3ime="m3ime"; gra3dus="gra3dus"; gra0dus="gra0dus"
elif [ "$nedel" = "2" ]; then
dennedel="Tuesday"; den_comm="Вторник"; m1ime="Tue_m1ime"; t1ime="Tue_t1ime"; gradus="Tue_gradus"; t2ime="Tue_t2ime"; m2ime="Tue_m2ime"; gra2dus="Tue_gra2dus"; t3ime="Tue_t3ime"; m3ime="Tue_m3ime"; gra3dus="Tue_gra3dus"; gra0dus="Tue_gra0dus"
elif [ "$nedel" = "3" ]; then
dennedel="Wednesday"; den_comm="Среда"; m1ime="Wed_m1ime"; t1ime="Wed_t1ime"; gradus="Wed_gradus"; t2ime="Wed_t2ime"; m2ime="Wed_m2ime"; gra2dus="Wed_gra2dus"; t3ime="Wed_t3ime"; m3ime="Wed_m3ime"; gra3dus="Wed_gra3dus"; gra0dus="Wed_gra0dus"
elif [ "$nedel" = "4" ]; then
dennedel="Thursday"; den_comm="Четверг"; m1ime="Thu_m1ime"; t1ime="Thu_t1ime"; gradus="Thu_gradus"; t2ime="Thu_t2ime"; m2ime="Thu_m2ime"; gra2dus="Thu_gra2dus"; t3ime="Thu_t3ime"; m3ime="Thu_m3ime"; gra3dus="Thu_gra3dus"; gra0dus="Thu_gra0dus"
elif [ "$nedel" = "5" ]; then
dennedel="Friday"; den_comm="Пятница"; m1ime="Fri_m1ime"; t1ime="Fri_t1ime"; gradus="Fri_gradus"; t2ime="Fri_t2ime"; m2ime="Fri_m2ime"; gra2dus="Fri_gra2dus"; t3ime="Fri_t3ime"; m3ime="Fri_m3ime"; gra3dus="Fri_gra3dus"; gra0dus="Fri_gra0dus"
elif [ "$nedel" = "6" ]; then
dennedel="Saturday"; den_comm="Суббота"; m1ime="Sat_m1ime"; t1ime="Sat_t1ime"; gradus="Sat_gradus"; t2ime="Sat_t2ime"; m2ime="Sat_m2ime"; gra2dus="Sat_gra2dus"; t3ime="Sat_t3ime"; m3ime="Sat_m3ime"; gra3dus="Sat_gra3dus"; gra0dus="Sat_gra0dus"
elif [ "$nedel" = "7" ]; then
dennedel="Sunday"; den_comm="Воскресенье"; m1ime="Sun_m1ime"; t1ime="Sun_t1ime"; gradus="Sun_gradus"; t2ime="Sun_t2ime"; m2ime="Sun_m2ime"; gra2dus="Sun_gra2dus"; t3ime="Sun_t3ime"; m3ime="Sun_m3ime"; gra3dus="Sun_gra3dus"; gra0dus="Sun_gra0dus"
fi
m0=""; m1=""; m2=""; m3=""; m4=""; m5=""
case "`uci get $conf_s.@ds18b20_s$kolllw[0].$m1ime`" in
"00") m0=selected;;
"10") m1=selected;;
"20") m2=selected;;
"30") m3=selected;;
"40") m4=selected;;
"50") m5=selected;;
esac
mm0=""; mm1=""; mm2=""; mm3=""; mm4=""; mm5=""
case "`uci get $conf_s.@ds18b20_s$kolllw[0].$m2ime`" in
"00") mm0=selected;;
"10") mm1=selected;;
"20") mm2=selected;;
"30") mm3=selected;;
"40") mm4=selected;;
"50") mm5=selected;;
esac
mmm0=""; mmm1=""; mmm2=""; mmm3=""; mmm4=""; mmm5=""
case "`uci get $conf_s.@ds18b20_s$kolllw[0].$m3ime`" in
"00") mmm0=selected;;
"10") mmm1=selected;;
"20") mmm2=selected;;
"30") mmm3=selected;;
"40") mmm4=selected;;
"50") mmm5=selected;;
esac
echo "<tr><td></td><td width=100 align=center bgcolor="#90f1f1" title="$den_comm">$dennedel</td><td></td><td align=center bgcolor="#8891f1" title="Температура_отключения_c_00:00_до_Tme1_$den_comm"><input type=text name="$gra0dus" size=3 value="
uci get $conf_s.@ds18b20_s$kolllw[0].$gra0dus
echo ">&deg;C</td><td align=center bgcolor="#90f1f1" title="$den_comm"><input type="text" name=$t1ime size=2 title="Часы" align="right" maxlength="2" value="
uci get $conf_s.@ds18b20_s$kolllw[0].$t1ime
echo ">:<select name="$m1ime" title="Минуты"><option $m0>00</option><option $m1>10</option><option $m2>20</option><option $m3>30</option><option $m4>40</option><option $m5>50</option></select></td><td align=center bgcolor="#8891f1" title="Температура_отключения_№1_$den_comm"><input type=text name="$gradus" size=3 value="
uci get $conf_s.@ds18b20_s$kolllw[0].$gradus
echo ">&deg;C</td><td></td><td align=center bgcolor="#90f1f1" title="$den_comm"><input type=text name="$t2ime" size=2 title="Часы" align="right" maxlength="2" value="
uci get $conf_s.@ds18b20_s$kolllw[0].$t2ime
echo ">:<select name=$m2ime title="Минуты"><option $mm0>00</option><option $mm1>10</option><option $mm2>20</option><option $mm3>30</option><option $mm4>40</option><option $mm5>50</option></select></td><td align=center bgcolor="#8891f1" title="Температура_отключения_№2_$den_comm"><input type=text name="$gra2dus" size=3 value="
uci get $conf_s.@ds18b20_s$kolllw[0].$gra2dus
echo ">&deg;C</td><td></td><td align=center bgcolor="#90f1f1" title="$den_comm"><input type=text name="$t3ime" size=2 title="Часы" align="right" maxlength="2" value="
uci get $conf_s.@ds18b20_s$kolllw[0].$t3ime
echo ">:<select name=$m3ime title="Минуты"><option $mmm0>00</option><option $mmm1>10</option><option $mmm2>20</option><option $mmm3>30</option><option $mmm4>40</option><option $mmm5>50</option></select></td><td align=center bgcolor="#8891f1" title="Температура_отключения_№3_$den_comm"><input type=text name="$gra3dus" size=3 value="
uci get $conf_s.@ds18b20_s$kolllw[0].$gra3dus
echo ">&deg;C</td><td></td></tr>"
: $((nedel = $nedel + 1))
done
echo " </table><table align=center border=0 width=><tr><td align=center><button type=submit class="b1" name="commit$kolllw" title="Сохранить_настройки_к_Датчику_№$kolllw ">"Save Sensor:"<b>$kolllw</b></button></td></tr></table></form><br>"
: $((kolllw = $kolllw + 1))
done
# Конец второй таблицы
echo "</body>"


