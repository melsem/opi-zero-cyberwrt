#!/bin/sh
echo "Content-type: text/html; charset=utf-8"
echo 
echo "<title>LCD setup</title>"
echo "<table align=center><tr><td>"
echo `cat /www/menu.html`
echo "</td></tr></table>"
conf_DS=$'/tmp/digitemp.conf'
cnf_term=$'/etc/config/i2c_HD44780'
kollwo_usb=$(sed -n '/ROM/p' $conf_DS | wc -l)
kollwo_gpio=$(ls -l /sys/devices/w1_bus_master1 | sed '/drw/!d; /"*-00*/!d' | awk '{print $9}' | sed -n '$=')
flconf='i2c_HD44780'
w1_gpio=$(uci get $flconf.@ds18b20[0].w1_gpio)
usb_w1_uart=$(uci get $flconf.@ds18b20[0].usb_w1_uart)
sda_gpio=$(uci get $flconf.@display[0].sda_gpio)
scl_gpio=$(uci get $flconf.@display[0].scl_gpio)
lcd_number=$(uci get $flconf.@display[0].lcd_number)
gros_simvol=$(uci get $flconf.@display[0].gros_simvol)
d=$(uci get $flconf.@display[0].device)
a=$(uci get $flconf.@display[0].address)
f=$(uci get $flconf.@display[0].read_file)
info_banner=$(uci get $flconf.@display[0].info_banner)
#*************************************************************

sa=""
sb=""
case $lcd_number in
"16" ) sa="selected";;
"20" ) sb="selected";;
esac
#
p0=""; p1=""; p2=""; p3="";
case $gros_simvol in
  "0" ) p0="selected";;
  "1" ) p1="selected";;
  "2" ) p2="selected";;
  "3" ) p3="selected";;
  "4" ) p4="selected";;
  "5" ) p5="selected";;
  "6" ) p6="selected";;
esac
#


if [ "`uci get $flconf.@display[0].info_banner`" == "1" ] ; then
ok_banner="checked"
no_banner=""
else
ok_banner=""
no_banner="checked"
fi
if [ "`uci get $flconf.@displ_tip[0].while`" == "1" ] ; then
ok_while="checked"
no_while=""
else
ok_while=""
no_while="checked"
fi
if [ "`uci get $flconf.@displ_tip[0].clock`" == "1" ] ; then
ok_clock="checked"
no_clock=""
clock_work="Clock ON. "
else
ok_clock=""
clock_work=""
no_clock="checked"
fi
if [ "`uci get $flconf.@displ_tip[0].led_on`" == "1" ] ; then
ok_led_on="checked"
no_led_on=""
else
ok_led_on=""
no_led_on="checked"
fi
if [ "`uci get $flconf.@displ_tip[0].on_read`" == "1" ] ; then
ok_on_read="checked"
no_on_read=""
else
ok_on_read=""
no_on_read="checked"
fi
if [ "`uci get $flconf.@displ_tip[0].on_ds18`" == "0" ] ; then
w1gpio=""
digitemp=""
fl_ds="<b>Works.</b> LCD monitor. $clock_work"
no_ds18="checked"
sens_or=0
elif [ "`uci get $flconf.@displ_tip[0].on_ds18`" == "1" ] ; then
w1gpio="checked"
digitemp=""
fl_ds="<b>Works.</b> $clock_work Found sensors - <b>$kollwo_gpio</b> pieces. Init gpio-port: <b>"$w1_gpio"</b>"
no_ds18=""
sens_or=1
elif [ "`uci get $flconf.@displ_tip[0].on_ds18`" == "2" ] ; then
w1gpio=""
digitemp="checked"
fl_ds="<b>Works.</b> $clock_work Found sensors: <b>$kollwo_usb</b> pieces.  Init.Port: <b>"$usb_w1_uart"</b>"
no_ds18=""
sens_or=2
fi
if [ "`uci get $flconf.@displ_tip[0].tip_ds1`" == "1" ] ; then
ok_termostat1="checked"
no_termostat1=""
else
ok_termostat1=""
no_termostat1="checked"
fi
if [ "`uci get $flconf.@displ_tip[0].tip_ds2`" == "1" ] ; then
ok_termostat2="checked"
no_termostat2=""
else
ok_termostat2=""
no_termostat2="checked"
fi
if [ "`uci get $flconf.@displ_tip[0].tip_ds3`" == "1" ] ; then
ok_termostat3="checked"
no_termostat3=""
else
ok_termostat3=""
no_termostat3="checked"
fi
if [ "`uci get $flconf.@displ_tip[0].tip_ds4`" == "1" ] ; then
ok_termostat4="checked"
no_termostat4=""
else
ok_termostat4=""
no_termostat4="checked"
fi
cat /etc/rc.d/S98i2c_HD44780.init &> /dev/null
if [ $? = 1 ]; then
ontxtoff2=Желаете_включить_авто-запуск_???
ontxtoff=Желаете_отключить_авто-запуск_???
onoff=staron
sohid=Disable
sotxthid=Enable
kart=$'/modules/i2c_HD44780/b.jpg'
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
kart=$'/modules/i2c_HD44780/r.jpg'
cto=ON
fi
sr1=$(uci get $flconf.@ds18b20[0].sensor1)
sr2=$(uci get $flconf.@ds18b20[0].sensor2)
sr3=$(uci get $flconf.@ds18b20[0].sensor3)
sr4=$(uci get $flconf.@ds18b20[0].sensor4)
nm1=$(uci get $flconf.@ds18b20[0].name_snr1)
nm2=$(uci get $flconf.@ds18b20[0].name_snr2)
nm3=$(uci get $flconf.@ds18b20[0].name_snr3)
nm4=$(uci get $flconf.@ds18b20[0].name_snr4)
nGp1_out=$(uci get $flconf.@ds18b20[0].nGp1_out)
nGp2_out=$(uci get $flconf.@ds18b20[0].nGp2_out)
nGp3_out=$(uci get $flconf.@ds18b20[0].nGp3_out)
nGp4_out=$(uci get $flconf.@ds18b20[0].nGp4_out)
alarm1=$(uci get $flconf.@ds18b20[0].alarm1)
alarm2=$(uci get $flconf.@ds18b20[0].alarm2)
alarm3=$(uci get $flconf.@ds18b20[0].alarm3)
alarm4=$(uci get $flconf.@ds18b20[0].alarm4)
histe1=$(uci get $flconf.@ds18b20[0].histe1)
histe2=$(uci get $flconf.@ds18b20[0].histe2)
histe3=$(uci get $flconf.@ds18b20[0].histe3)
histe4=$(uci get $flconf.@ds18b20[0].histe4)
####  invers  ###
if [ "`uci get $flconf.@ds18b20[0].invers1`" = "1" ] ; then
yss1="checked"
noo1=""
else
yss1=""
noo1="checked"
fi
if [ "`uci get $flconf.@ds18b20[0].invers2`" = "1" ] ; then
yss2="checked"
noo2=""
else
yss2=""
noo2="checked"
fi
if [ "`uci get $flconf.@ds18b20[0].invers3`" = "1" ] ; then
yss3="checked"
noo3=""
else
yss3=""
noo3="checked"
fi
if [ "`uci get $flconf.@ds18b20[0].invers4`" = "1" ] ; then
yss4="checked"
noo4=""
else
yss4=""
noo4="checked"
fi

pidof i2c_HD44780 &> /dev/null
if [ $? = 1 ]; then
commit="save_commit"
but_txt="  i2c_hd44780 START"
fil_ds="Not Work"
dvig="cozapusk"
dvig_txt="Желаете_запусить_модуль_???"
dvig_imag="/modules/i2c_hd44780/b.jpg"
else
commit="save_com_restart"
fil_ds=$fl_ds
but_txt="   i2c_hd44780 STOP"
dvig="coostanov"
dvig_txt="Желаете_остановить_модуль_???"
dvig_imag="/modules/i2c_hd44780/r.jpg"
fi
if [ "`uci get $flconf.@displ_tip[0].on_ds18`" == "2" ] || [ "`uci get $flconf.@displ_tip[0].on_ds18`" == "0" ]; then
hidden="hidden"
else
hidden=""
fi
if [ "`uci get $flconf.@displ_tip[0].on_ds18`" == "0" ]; then
hien=""
else
hien="hidden"
fi
#*************************************************************
echo "<body bgcolor="#f0f0f0">
<style>
  .b0 {
cursor: pointer;
}
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
padding: 1px 1px;
margin-left: 1px;
border-radius:inherit;
}
</style><form method="POST" action="Save.cgi"><table align=center><br><caption><b>General setup i2c_HD44780</b></caption><tr><td><table align=center><tr><td align="right"><b>"display"</b></td><td><b>" setup"</b></td></tr><tr><td align=center bgcolor="#90f1f1">"auto restart "("loop-while")""</td><td class="b0" align=center bgcolor="#90f1f1"><input type="radio" name="while" value="0" $no_while>Off<input type="radio" name="while" value="1" $ok_while>ON</td></tr><tr><td align=center bgcolor="#90f1f1">i2c-Dev</td><td align=center bgcolor="#90f1f1"> "Dev . "<input type=text name="device" size=2 value="
echo $d
echo "></td></tr><tr><td align=center bgcolor="#90f1f1">I2C_slave-adres</td><td align=center bgcolor="#90f1f1"> "hex . "<input type=text name="address" size=3 value="
echo $a
echo "></td></tr><tr><td align=center bgcolor="#90f1f1">LCD_GPIO-SDA_SCL</td><td align=center bgcolor="#90f1f1"> "sda-scl "<input type=text name="sda_gpio" size=2 value="
echo $sda_gpio
echo "><input type=text name="scl_gpio" size=2 value="
echo $scl_gpio
echo "></td></tr>


<tr><td align=center bgcolor="#90f1f1">String length_LCD</td><td align="center" bgcolor="#90f1f1">"lcd . "
<select title="Колличество_символов_в_строке_на_LCD" name="lcd_number">
<option value="16" label="16." $sa>"16."</option><option value="20" label="20." $sb>"20."</option>
</select></td></tr>


<tr><td align=center bgcolor="#90f1f1">WIDE</td><td align=center bgcolor="#90f1f1">
<select title="Выбор_высоты_символа" name="gros_simvol">
<option value="0" label="OK_по_одной_строке" $p0>"OK по одной строке"</option>
<option value="1" label="OK_в_две_строки" $p1>"OK в две строки"</option>
<option value="2" label="OK_в_две_строки_нижний_демо" $p2>"OK в две строки нижний демо"</option>
<option value="3" label="OK_Clock_в_четыре_строки" $p3>"OK Clock в четыре строки"</option>
<option value="4" label="OK_в_четыре_строки_демо." $p4>"OK в четыре строки демо"</option>
<option value="5" label="пусто1" $p5>"пусто1"</option>
<option value="6" label="пусто2" $p6>"пусто2"</option>
</select></td></tr>




<tr><td align=center bgcolor="#90f1f1" $hien> file name:</td><td align=center bgcolor="#90f1f1" $hien> "/tmp/"<input type=text name="read_file" size=20 value="
echo $f
echo "></td></tr><tr><td align=center bgcolor="#90f1f1" $hien>LCD: info banner</td><td class="b0" align=center bgcolor="#90f1f1" $hien><input type="radio" name="info_banner" value="0" $no_banner>Off<input type="radio" name="info_banner" value="1" $ok_banner>ON</td></tr><tr><td align=center bgcolor="#90f1f1" $hien>file /temp/<b>$f</b> (read)</td><td class="b0" align=center bgcolor="#90f1f1" $hien><input type="radio" name="on_read" value="0" $no_on_read>Off<input type="radio" name="on_read" value="1" $ok_on_read>ON</td></tr><tr><td align=center bgcolor="#90f1f1">clock</td><td class="b0" align=center bgcolor="#90f1f1"><input type="radio" name="clock" value="0" $no_clock>Off<input type="radio" name="clock" value="1" $ok_clock>ON</td></tr><tr><td align=center bgcolor="#90f1f1">"led - "</td><td class="b0" align=center bgcolor="#90f1f1"><input type="radio" name="led_on" value="0" $no_led_on>Off<input type="radio" name="led_on" value="1" $ok_led_on>ON</td></tr><tr><td align="right"><b>sensor</b></td><td><b>setup</b></td></tr><tr><td align=center bgcolor="#90f1f1">"sensor ds18"</td><td class="b0" align=center bgcolor="#90f1f1"><input type="radio" name="on_ds18" value="0" $no_ds18>Off<input type="radio" name="on_ds18" value="1" $w1gpio>w1-gpio<input type="radio" name="on_ds18" value="2" $digitemp>usb-uart</td></tr>"
if [ "$sens_or" == "0" ] ; then
echo "<tr><td class="b2" align=center bgcolor="#90f1f1">"sensor ds18 Off"</td><td class="b2" align=center bgcolor="#90f1f1">"sensor ds18 Off"</td></tr>"
elif [ "$sens_or" == "1" ] ; then
echo "<tr><td align=center bgcolor="#90f1f1">w1_gpio ds1820</td><td align=center bgcolor="#90f1f1"> "gpio-pin . "<input class="b2" type=text name="w1_gpio" size=5 value="
echo $w1_gpio
echo "></td></tr>"
elif [ "$sens_or" == "2" ] ; then
echo "<tr><td align=center bgcolor="#90f1f1">usb-uart ds1820</td><td align=center bgcolor="#90f1f1"> "/dev/tty"<input class="b2" type=text name="usb_w1_uart" size=5 value="
echo $usb_w1_uart
echo "></td></tr>"
fi
echo "
<tr><td align="right"><b>Thermostat </b></td><td><b>setup</b></td></tr>
</table>
<table>"
echo "
<tr>
<td align=center bgcolor="#90f1f1" $hidden class="b0" title="включить_или_отключить_терморегулятор">Thermostat</td><td></td><td></td>
<td align=center bgcolor="#90f1f1" $hidden>Pin out</td><td></td><td></td>
<td align=center bgcolor="#90f1f1" $hidden class="b0" title="Инвертировать_активный_уровень_gpio-выхода_для_управления_силовыми_ключами">Invers-out-gpio</td><td></td><td></td>
<td align=center bgcolor="#90f1f1" $hidden class="b0" title="Температура_отключения">Power-OFF</td><td></td><td></td>
<td align=center bgcolor="#90f1f1" $hidden class="b0" title="Температура_Bключения">Power-ON</td><td></td><td></td>
<td align=center bgcolor="#90f1f1">Nickname sensor</td><td></td><td></td>
<td align=center bgcolor="#90f1f1">W1 ID-sensor</td>
</tr>
<tr>
<td class="b0" align=center bgcolor="#90f1f1" $hidden><input type="radio" name="tip_ds1" value="0" $no_termostat1 class="b0" title="выключить_терморегулятор">Off
<input type="radio" name="tip_ds1" value="1" $ok_termostat1 class="b0" title="включить_терморегулятор">ON</td><td></td><td></td>
<td align=center bgcolor="#90f1f1" $hidden>Gpio-<input class="b2" type=text name="nGp1_out" size=2 value="$nGp1_out"></td><td></td><td></td>
<td align=center bgcolor="#90f1f1" align=center $hidden class="b0"><input type=radio name=invers1 value=0 $noo1 title="Включить_активный_уровень_0_gpio-выхода"><b>0</b><input type=radio name=invers1 value=1 $yss1 title="Включить_активный_уровень_1_gpio-выхода"><b>1</b></td><td></td><td></td>
<td align=center bgcolor="#90f1f1" $hidden><input class="b2" title="Температура_отключения" type=text name="alarm1" size=4 value="$alarm1"><b>&deg;C</b></td><td></td><td></td>
<td align=center bgcolor="#90f1f1" $hidden><input class="b2" title="Температура_Bключения" type=text name="histe1" size=4 value="$histe1"><b>&deg;C</b></td><td></td><td></td>
<td align=center bgcolor="#90f1f1"><input class="b2" type=text name="name_snr1" size=8 value="$nm1"></td><td></td><td></td>
<td align=center bgcolor="#90f1f1"><input class="b2" type=text name="sensor1" size=16 value="$sr1"></td>
</tr>
<tr>
<td class="b0" align=center bgcolor="#90f1f1" $hidden><input type="radio" name="tip_ds2" value="0" $no_termostat2 class="b0" title="выключить_терморегулятор">Off
<input type="radio" name="tip_ds2" value="1" $ok_termostat2 class="b0" title="включить_терморегулятор">ON</td><td></td><td></td>
<td align=center bgcolor="#90f1f1" $hidden>Gpio-<input class="b2" type=text name="nGp2_out" size=2 value="$nGp2_out"></td><td></td><td></td>
<td align=center bgcolor="#90f1f1" align=center $hidden class="b0"><input type=radio name=invers2 value=0 $noo2 title="Включить_активный_уровень_0_gpio-выхода"><b>0</b><input type=radio name=invers2 value=1 $yss2 title="Включить_активный_уровень_1_gpio-выхода"><b>1</b></td><td></td><td></td>
<td align=center bgcolor="#90f1f1" $hidden><input class="b2" title="Температура_отключения" type=text name="alarm2" size=4 value="$alarm2"><b>&deg;C</b></td><td></td><td></td>
<td align=center bgcolor="#90f1f1" $hidden><input class="b2" title="Температура_Bключения" type=text name="histe2" size=4 value="$histe2"><b>&deg;C</b></td><td></td><td></td>
<td align=center bgcolor="#90f1f1"><input class="b2" type=text name="name_snr2" size=8 value="$nm2"></td><td></td><td></td>
<td align=center bgcolor="#90f1f1"><input class="b2" type=text name="sensor2" size=16 value="$sr2"></td>
</tr>
<tr>
<td class="b0" align=center bgcolor="#90f1f1" $hidden><input type="radio" name="tip_ds3" value="0" $no_termostat3 class="b0" title="выключить_терморегулятор">Off
<input type="radio" name="tip_ds3" value="1" $ok_termostat3 class="b0" title="включить_терморегулятор">ON</td><td></td><td></td>
<td align=center bgcolor="#90f1f1" $hidden>Gpio-<input class="b2" type=text name="nGp3_out" size=2 value="$nGp3_out"></td><td></td><td></td>
<td align=center bgcolor="#90f1f1" align=center $hidden class="b0"><input type=radio name=invers3 value=0 $noo3 title="Включить_активный_уровень_0_gpio-выхода"><b>0</b><input type=radio name=invers3 value=1 $yss3 title="Включить_активный_уровень_1_gpio-выхода"><b>1</b></td><td></td><td></td>
<td align=center bgcolor="#90f1f1" $hidden><input class="b2" title="Температура_отключения" type=text name="alarm3" size=4 value="$alarm3"><b>&deg;C</b></td><td></td><td></td>
<td align=center bgcolor="#90f1f1" $hidden><input class="b2" title="Температура_Bключения" type=text name="histe3" size=4 value="$histe3"><b>&deg;C</b></td><td></td><td></td>
<td align=center bgcolor="#90f1f1"><input class="b2" type=text name="name_snr3" size=8 value="$nm3"></td><td></td><td></td>
<td align=center bgcolor="#90f1f1"><input class="b2" type=text name="sensor3" size=16 value="$sr3"></td>
</tr>
<tr>
<td class="b0" align=center bgcolor="#90f1f1" $hidden><input type="radio" name="tip_ds4" value="0" $no_termostat4 class="b0" title="выключить_терморегулятор">Off
<input type="radio" name="tip_ds4" value="1" $ok_termostat4 class="b0" title="включить_терморегулятор">ON</td><td></td><td></td>
<td align=center bgcolor="#90f1f1" $hidden>Gpio-<input class="b2" type=text name="nGp4_out" size=2 value="$nGp4_out"></td><td></td><td></td>
<td align=center bgcolor="#90f1f1" align=center $hidden class="b0"><input type=radio name=invers4 value=0 $noo4 title="Включить_активный_уровень_0_gpio-выхода"><b>0</b><input type=radio name=invers4 value=1 $yss4 title="Включить_активный_уровень_1_gpio-выхода"><b>1</b></td><td></td><td></td>
<td align=center bgcolor="#90f1f1" $hidden><input class="b2" title="Температура_отключения" type=text name="alarm4" size=4 value="$alarm4"><b>&deg;C</b></td><td></td><td></td>
<td align=center bgcolor="#90f1f1" $hidden><input class="b2" title="Температура_Bключения" type=text name="histe4" size=4 value="$histe4"><b>&deg;C</b></td><td></td><td></td>
<td align=center bgcolor="#90f1f1"><input class="b2" type=text name="name_snr4" size=8 value="$nm4"></td><td></td><td></td>
<td align=center bgcolor="#90f1f1"><input class="b2" type=text name="sensor4" size=16 value="$sr4"></td>
</tr>
</table>
<table align=center border=0><tr><td align=center><button name="$commit" class="b1" type=submit title="Сохранить_???">Save</button></td></tr></table></table></form>"
echo "<form method="POST" action="Save.cgi"><table align=center border=0><tr><td><tr><td align=center><p style="color:#$clor">Auto Startup - <b>$cto</b></p></td></tr><tr><td align=center><button $dsd name="$onoff" type=submit title="$ontxtoff" class="b1"><img src=/modules/i2c_hd44780/b.jpg alt= style="vertical-align: middle"> "Enable"</button><button $dsd2 name="$onoff" type=submit title="$ontxtoff2" class="b1"><img src=/modules/i2c_hd44780/r.jpg alt= style="vertical-align: middle"> "Disable"</button></td></tr></td></tr></table></form>"
echo "<form method="POST" action="Save.cgi"><table align=center border=0><tr><td align=center>$fil_ds</td></tr><tr><td align=center><button class="b1" name="$dvig" type=submit title="$dvig_txt"><img src="$dvig_imag" alt= style="vertical-align: middle">"$but_txt"</button></td></tr></table></form>"




