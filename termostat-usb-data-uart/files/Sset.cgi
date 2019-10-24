#!/bin/sh
#Copyright (C) 2013 cyberwrt.ru, carduino.ru, cyber-place.ru, mp3car.ru
echo "Content-type: text/html; charset=utf-8"
echo ""
echo `cat /www/menu.html`
conf_s="ds18b20a_conf"
sdds=$(uci get $conf_s.@Gpio1Wr[0].enabled0)
case "$sdds" in
"" ) Tm_metr=selected;;
"1" ) Mr_zilka=selected;;
esac
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
echo "<head><title>Termostat - Temperature</title></head>"
#____________________________________________________________
#____________________________________________________________
cat /etc/rc.d/S99ds18b20usba_init &> /dev/null
if [ $? = 1 ]; then
ontxtoff2=Желаете_включить_авто-запуск_???
ontxtoff=Желаете_отключить_авто-запуск_???
onoff=staron
sohid=Disable
sotxthid=Enable
kart=$'/modules/termostatUsb/b.jpg'
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
kart=$'/modules/termostatUsb/r.jpg'
cto=ON
fi
#____________________________________________________________
#____________________________________________________________
echo ""
echo "
<style>
  .b1 {
font-size: 15pt;
text-align: center;
padding: 20px 40px;
margin-left: 1px;
cursor: pointer;
border-radius:inherit;
}
  .b2 {
font-size: 15pt;
text-align: center;
padding: 10px 20px;
margin-left: 1px;
cursor: pointer;
border-radius:inherit;
}
</style>
<form method="POST" action="Setup.cgi">"
echo "<table border=0 width=>"
echo "
<tr><td align=center><p style="font-size:38px">Sensor DS18B20 usb-data, uart termostat</p></td></tr>
<br>
<tr><td align=center>First you have to install the packages to get support for digitemp, digitemp-usb, libusb-compat. Init.-Port rs232-UART /dev/ttyATH0 chip 9331,</td></tr>
<tr><td align=center> /dev/ttyS0 chip 9341 - 7241</td></tr>
<tr><td align=center> Init.-Port for USB-UART must be from /dev/ttyUSB0 to /dev/ttyUSB9</td></tr>
<tr><td align=center><a target="_blank" href=http://narodmon.ru> narodmon.ru </a> || <b><a target="_blank" title="Описание" href=http://cyber-place.ru/showpost.php?p=11222&postcount=67>Info</a></b> </td></tr>
<td align=center>
<table border=0 width=250><br>
<li><b>General setup</b>
<td><table border=1 width=650>
<tr><td align=center>Auto - search 1wire sensor</td><td align=center><input type=radio name=search value=0 $nyoo>Off<input type=radio name=search value=1 $yess>On</td></tr>
<tr><td align=center>1Wire enable - <input type=radio name=enbled0 value=0 $noo>Off<input type=radio name=enbled0 value=1 $yss>On</td>
<td align=center>Init 1Wire - port <input type=text name="init_port" size=10 value="
uci get $conf_s.@Gpio1Wr[0].gpio1wr
echo "></td></tr>
<tr><td align=center>interval min</td><td align=center><select name=interval><option $send_off>send off</option><option $pyat>5</option><option $desat>10</option><option $pytna>15</option><option $dvadc>20</option><option $tridc>30</option><option $sest>60</option>
</select></td></tr>
<tr><td align=center>server down <input type=text name="srv_d" size=15 value="
uci get $conf_s.@Gpio1Wr[0].serv_d
echo "></td>
<td align=center>port <input type=text name="srv_port" size=5 value="
uci get $conf_s.@Gpio1Wr[0].serv_port
echo "></td></tr><tr><td align=center>Log -->> Samba-ftp-serv</td><td align=center><input type=radio name="ftp_svr" value="0" $oonoos>Off<input type=radio name="ftp_svr" value="1" $ssysso>On</td></tr></table>"
echo "<br>
<table border=1 width=650>
<caption><li>Additional installation add <b>gpio</b> and on-off start script</caption>
<tr><td align=center><input type="radio" name="allset" $aukul value=0 >table setup sens. - auto</td><td align=center><input type="radio" name="allset" value=10 $ukukl>table setup sensor - all</td></tr>
<tr><td align=center>gp-out I2C <b>SDA-SCL</b> <input type=text name="sda_scl_i2c" size=6 value="
uci get $conf_s.@Gpio1Wr[0].sda_scl
echo "></td>
<td align=center><b>PCF8574</b><input type=radio name="pcf8574" value=0 $otpkl>Off<input type=radio name="pcf8574" value=1 $hkjh>On
</td></tr>
<tr><td align=center> hex adress <b>0x</b><input type=text name="adres_i2c" size=5 value="
uci get $conf_s.@Gpio1Wr[0].hexadr
echo "></td>
<td align=center>PCF8574 add 8 pin gpio <b>"
fgfd=$(uci get $conf_s.@Gpio1Wr[0].hexadr)
sed '/GPIOs/!d; /pcf8574/!d; /0-00'$fgfd'/!d' /sys/kernel/debug/gpio | awk '{print $2}'
echo "<b></td></tr>
</table>
<table border=1 width=650>
<tr><td align=center>$stpd <b><blink>$ENABLED </blink> script - </b>
<input type=text name="imya_skript" size=20 value="
uci get $conf_s.@Gpio1Wr[0].imya_skript
echo "><input type=radio name="pu_Skryp" value=0 $otkl>STOP<input type=radio name="pu_Skryp" value=1 $wkl>RUN <blink>$ENABLED</blink></td></tr>
</table>
<table border=0 width=650><tr><td align=center><button class="b1" name="commit" type=submit title="Сохранить_???">Save</button></td></tr></table>
</td>
</form></td></tr></table></form>"
#____________________________________________________________
echo "<table border=1 width=650>
<form action="Setup.cgi" method="post">
<tr><td align=center><b>Script text:</b><p><textarea rows="10" cols="85" name="text" spellcheck="false" placeholder=Text-print? >"
vcvc=$(uci get $conf_s.@Gpio1Wr[0].imya_skript)
cat /usr/sbin/$vcvc &> /dev/null
if [ $? = 0 ]; then
sed -n '/^.\{1\}/p' /usr/sbin/$vcvc
fi
echo "</textarea>
<input class="b2" type="submit" value="Save-the-changes" name=></p>
</form>
</td></tr></table>"
#_____________________________statUsb_______________________________
echo "
<table border=0 width=650><tr><form action="Setup.cgi" method="post"><td align=center><p style="color:#$clor">Auto Startup - $cto</p></td></tr></table>
<table border=0 width=650><tr><td align=center><button class="b1" $dsd name="$onoff" type=submit title="$ontxtoff"><img src=/modules/termostatUsb/b.jpg alt= style="vertical-align: middle"> "Enable"</button><button class="b1" $dsd2 name="$onoff" type=submit title="$ontxtoff2"><img src=/modules/termostatUsb/r.jpg alt= style="vertical-align: middle"> "Disable"</button></td></tr></table>
</form>"
#____________________________________________________________
gppwr=$(grep gpio1wr /etc/config/ds18b20a_conf | awk '{print $3}'); gppwr1=$(echo "${gppwr} = ${#gppwr}" | awk '{print $3}'); gppwr12=$(echo $(( ${gppwr1} - 10 ))); gp1Wr=$(echo ${gppwr:9:$gppwr12})
conf_DS=$'/tmp/digitemp'
kwod=$(sed -n '/ROM/p' $conf_DS | wc -l)
pidof ds18b20usba &> /dev/null
if [ $? = 1 ]; then
echo "<form method="POST" action="Setup.cgi">"
echo "
<h3 align="center">The program does not work<br><button class="b1" name="cozapusk" type=submit title="Желаете_запусить_модуль_???"><img src="/modules/termostatUsb/b.jpg" alt= style="vertical-align: middle">Termostat START</button></h3>
</form>"
else
echo "<form method="POST" action="Setup.cgi">"
echo "
<h3 align="center">The program works with temperature. Number of found sensors - $kwod pieces.  Init.Port-"$gp1Wr"<br> <button class="b1" name="coostanov" type=submit title="Желаете_остановить_модуль_???"><img src="/modules/termostatUsb/r.jpg" alt= style="vertical-align: middle">Termostat STOP</button></h3>
</form>"
fi
#_____________________________________________________________________________
#_________________________	#  Вторая таблица___________________________________
conf_DS=$'/tmp/digitemp'
if [ "`uci get $conf_s.@Gpio1Wr[0].allset`" = "10" ] ; then
kwod=10
else
kwod=$(sed -n '/ROM/p' $conf_DS | wc -l)
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
trmo=""
trmst=""
kndc=""
morz=""
case "`uci get $conf_s.@ds18b20_s$kolllw[0].tipsensr`" in
"termometr") trmo=selected;;
"termostat") trmst=selected;;
"kondicioner") kndc=selected;;
"morozilka") morz=selected;;
esac
echo "<form method="POST" action="Sens$kolllw.cgi">"
echo "
<body>
<table border="0" cellspacing="0" cellpadding="5" width="250">

<body bgcolor="#f1f1f1">
<tr><td align=center><b>Sensor - $kolllw</b></td></tr>
<tr><td align=center><div class=l style=background-color:$clr>
<table align=center border=1></td>
<td align=center>ID:</td>
<td align=center><b>"
uci get $conf_s.@Gpio1Wr[0].serv_d
uci get $conf_s.@Gpio1Wr[0].serv_port
echo " - $otprn</b></td>
<td align=center>Device</td>
<td align=center>GPIO/out:</td>
<td align=center>Temperatura:</td>
<td align=center>Histerezis:</td>
<td align=center>DIR:</td>
</tr>
<tr height="10">
<td><input type=text name="id_dev" size=18 value="
uci get $conf_s.@ds18b20_s$kolllw[0].id_sensor
echo "></td>
<td><input $dssd2 type=radio name="narmon$kolllw" value=0 $no>Off<input $dssd type=radio name="narmon$kolllw" value=1 $yes>On</td>
<td><select name=tipsnsr><option $trmo>termometr</option><option $trmst>termostat</option><option $kndc>kondicioner</option><option $morz>morozilka</option></select></td>
<td><input type=text name="pwut" size=5 value="
uci get $conf_s.@ds18b20_s$kolllw[0].pwout
echo "></td>
<td><input type=text name="grdus" size=5 value="
uci get $conf_s.@ds18b20_s$kolllw[0].gradus
echo "></td>
<td><input type=text name="hster" size=5 value="
uci get $conf_s.@ds18b20_s$kolllw[0].hister
echo "></td>
<td><input type=text name="flder" size=18 value="
uci get $conf_s.@ds18b20_s$kolllw[0].folder
echo "></td></tr>
</body></table></body>
<table ><tr><td align=center ><button class="b1" type=submit class=submit name="commit$kolllw" title="Сохранить_???">Save</button></td></tr></table>
</form></table>"
: $((kolllw = $kolllw + 1))
done
