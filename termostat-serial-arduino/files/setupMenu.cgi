#!/bin/sh
echo 
echo "<title>Установки: termostat-serial-arduino</title>"
echo "<table align=center border=0><tr><td>"
echo `cat /www/menu.html`
echo "</td></tr></table>"

confFile="termostat-serial-arduino"
updateRead=$(uci get $confFile.@displ_tip[0].updateRead)
speed=$(uci get port_listen.@displ_tip[0].speed)
devPort=$(uci get port_listen.@displ_tip[0].devPort)
t_file=$(uci get port_listen.@displ_tip[0].t_file)
cfl='termostat-serial-arduino'
strok=$(uci get $cfl.@displ_tip[0].strok)
i2c_adres=$(uci get $cfl.@displ_tip[0].i2c_adres)
simvol=$(uci get $cfl.@displ_tip[0].simvol)
w1_gpio=$(uci get $cfl.@arduino[0].w1_gpio)
prbor=$(uci get $cfl.@displ_tip[0].prbor)
: $((kollv = $strok + 1))
#**********************************************
ctl="#f0f0f0"
echo "<body bgcolor="#f0f0f0">
<style>
 .b0 {
 cursor: pointer;
 font:15px Verdana, Geneva, Tahoma, sans-serif; 
 text-align: center;
 padding: 1px 5px;
 }
 .b1 {
 font:15px Verdana, Geneva, Tahoma, sans-serif; 
 text-align: left;
 padding: 5px 10px;
 margin-left: 1px;
 cursor: pointer;
 border-radius:inherit;
 color:#050808; 
 }
 .b2 {
 font:20px Verdana, Geneva, Tahoma, sans-serif; 
 text-align: center;
 padding: 1px 1px;
 margin-left: 1px;
 border-radius:inherit;
 cursor: pointer;
 color:#050808; 
 }
 .b3 {
 font:20px Verdana, Geneva, Tahoma, sans-serif; 
 text-align: center;
 background-color: #ffffff; /* Цвет фона */
 color: #930; /* Цвет текста */
 cursor: pointer;
 }
 .button{
 text-decoration:none; text-align:center; 
 padding:5px 5px; 
 -webkit-border-radius:20px;
 -moz-border-radius:20px; 
 border-radius: 20px; 
 font:20px Verdana, Geneva, Tahoma, sans-serif; 
 font-weight:bold; 
 -webkit-box-shadow:0px 0px 2px #bababa, inset 0px 0px 1px #ffffff; 
 -moz-box-shadow: 0px 0px 2px #bababa,  inset 0px 0px 1px #ffffff;  
 box-shadow:0px 0px 2px #bababa, inset 0px 0px 1px #ffffff;  
 border:solid 1px #3bbbc7; 
 color:#fcffff; 
 background:#3bbbc7; 
 }
 .button:hover {
 padding:5px 5px; 
 -webkit-border-radius:20px;
 -moz-border-radius:20px; 
 border-radius: 20px; 
 font:20px Verdana, Geneva, Tahoma, sans-serif; 
 font-weight:bold; 
 -webkit-box-shadow:0px 0px 2px #bababa, inset 0px 0px 1px #ffffff; 
 -moz-box-shadow: 0px 0px 2px #bababa,  inset 0px 0px 1px #ffffff;  
 box-shadow:0px 0px 2px #bababa, inset 0px 0px 1px #ffffff;  
 border:solid 1px #3ba4c7; 
 color:#050808; 
 background:#3bbbc7; 
 }
 .button:active{
 padding:5px 5px; 
 -webkit-border-radius:20px;
 -moz-border-radius:20px; 
 border-radius: 20px; 
 font:20px Verdana, Geneva, Tahoma, sans-serif; 
 font-weight:bold; 
 -webkit-box-shadow:0px 0px 2px #bababa, inset 0px 0px 1px #ffffff; 
 -moz-box-shadow: 0px 0px 2px #bababa,  inset 0px 0px 1px #ffffff;  
 box-shadow:0px 0px 2px #bababa, inset 0px 0px 1px #ffffff;  
 border:solid 1px #005072; 
 color:#387575; 
 background:#e4eef2; 
 }
</style>"
sa=""
sb=""
case $simvol in
"16" ) sa="selected";;
"20" ) sb="selected";;
esac
cls="&deg;C"
echo "
<form method="POST" action="saveApply.cgi">
<table align=center><tr><td bgcolor="#90f1f1" align=center title="Имя_прибора_вписывать_без_пробелов">"
if [ "$updateRead" != "1" ] ; then
echo "
<input class="b2" type=text name="prbor" maxlength=54 size="${#prbor}" value="$prbor">
<input type=text name="tprbor" hidden value="$prbor">
<input type=text name="fPort" hidden value="$devPort">"
else
echo "Первый вход в web установок. Нету связи с Arduino. 
 http://cyber-place.ru/showthread.php?t=2752 
Прописать ком-порт, если отличается, и скорость порта прописаная в скетче. 
 Нажать ''Синхронизировать и перезагрузить'' обязательно.
Должна отобразится температура. 
И все, настройки arduino и модуля в роутере синхронизируются, 
и теперь можно настраивать под себя в ''Настройка_arduino''."
fi
echo "</td></tr></table>
<table align=center><tr><td align=center>"
if [ "$updateRead" != "1" ] ; then
echo "<button name="save_prbor" class="button" type=submit title="Сохранить_???">"Сохранить"</button>"
fi
echo "</td></tr></table>
</form>

<form method="POST" action="saveApply.cgi">
<table align=center><caption><b>Установки</b></caption><tr><td>

<table align=center border="1">







<tr><td bgcolor="$ctl" align=center>"Скорость"</td><td align="center" bgcolor="$ctl"> "byt-rate "
<select name="speed" class="b2">"
ss=4800
while [ "$ss" -le "230400" ]; do
if  [ "$ss" -eq "76800" ]; then
: $((ss = $ss - 19200))
fi
sds=""
case $speed in
$ss ) sds="selected";;
esac
echo "<option value=$ss label="$ss'.'" $sds>"$ss "</option>"
: $((ss = $ss * 2))
done
echo "</select><input type=text name="tspeed" hidden value="$speed"></td></tr>
<tr><td bgcolor="$ctl" align=center>"com port"</td><td align="center" bgcolor="$ctl">"/dev/tty"
<input class="b2" required="" type=text maxlength=4 name="devPort" size="${#devPort}" value="$devPort">
<input type=text name="tdevPort" hidden value="$devPort"></td></tr>"


if [ "$updateRead" != "1" ] ; then
echo "
<tr><td bgcolor="$ctl" align=center title="для_хранения_принятых_данных_от_аrduino">"temp фаил"</td>
<td align="center" bgcolor="$ctl"  title="имя_файла_без_пробелов!!!"><input class="b3" type=text name="t_file" value="$t_file">
<input type=text name="tt_file" hidden value="$t_file"></td></tr>
<tr><td align=center bgcolor="$ctl">i2c-LCD</td><td align="center" bgcolor="$ctl">"Адрес i2c"
<select title="Адрес_LCD_на_шине_i2c" name="i2c_adres" class="b2">"
ss=20
while [ "$ss" -ne "40" ]; do
if  [ "$ss" -eq "28" ]; then
: $((ss = $ss + 4))
fi
sds=""
case $i2c_adres in
$ss ) sds="selected";;
esac
echo "<option value=$ss label="'0x'$ss'.'" $sds>"'0x'$ss "</option>"
: $((ss = $ss + 1))
done
echo "</select>".hex"<input type=text name="ti2c_adres" hidden value="$i2c_adres"></td></tr>
<tr><td align=center bgcolor="$ctl">LCD</td><td align="center" bgcolor="$ctl">"Колличество символов"
<select title="Колличество_символов_в_строке_на_LCD" name="simvol" class="b2">
<option value="16" label="16." $sa>"16."</option><option value="20" label="20." $sb>"20."</option>
</select><input type=text name="tsimvol" hidden value="$simvol"></td></tr>

<tr><td align=center bgcolor="$ctl">ds18b20</td><td align="center" bgcolor="$ctl"> "Kолличество датчиков"
<select title="Колличество_подключенных_датчиков" name="strok" class="b2">"
ss=1
while [ "$ss" -ne "11" ]; do
sds=""
case $strok in
$ss ) sds="selected";;
esac
echo "<option value=$ss label="$ss'.'" $sds>"$ss"</option>"
: $((ss = $ss + 1))
done
echo "</select><input type=text name="tstrok" hidden value="$strok"></td></tr>

<tr><td align=center bgcolor="$ctl">1Wr</td><td align="center" bgcolor="$ctl"> "w1-gpio-pin "
<input class="b2" type=text name="w1_gpio" size=2 value="$w1_gpio">
<input type=text name="tw1_gpio" hidden value="$w1_gpio"></td></tr>"
fi
echo "</table><table align=center border=0>

<tr><td align=center>"
if [ "$updateRead" == "1" ] ; then
	echo "<button name="save_commit_synk" class="button" type=submit title="Синхронизировать_???">"Синхронизировать и перезагрузить"</button>"
else
	echo "<button name="save_commit" class="button" type=submit title="Сохранить_???">"Сохранить и перезагрузить"</button>"
fi
echo "</td></tr></table></td></tr></table></form>"





if [ "$updateRead" != "1" ] ; then
echo "<table align=center border=0>"
echo "<tr><td></td><td></td><td></td>
<td align=center>"Название"</td><td></td><td></td>
<td align=center>Серийный номер</td><td></td><td></td>
<td align=center class="b0" title="включить_или_отключить_терморегулятор">"Терморегулятор"</td><td></td><td></td>
<td align=center title="Номер_вывода_arduino_для_управления_силовыми_ключами">"GPIO Выводы"</td><td></td><td></td>
<td align=center class="b0" title="Инвертировать_активный_уровень_gpio-выхода_для_управления_силовыми_ключами">"Инвертировать"</td><td></td><td></td>
<td align=center class="b0" title="Температура_отключения">t*Выкл-вкл</td><td></td><td></td>
<td align=center class="b0" title="Гистерезис_Bключения">"  Гистерезис  "</td><td></td><td></td>
<td></td><td></td><td></td></tr>"

i=1
while [ "$i" -ne "$kollv" ]; do

if [ "`uci get $cfl.@arduino[0].invers$i`" = "1" ] ; then
  yes="checked"
  nou=""
else
  yes=""
  nou="checked"
fi
#
p0=""; p1=""; p2=""; p3="";
tip_ds=$(uci get $cfl.@arduino[0].tip_ds$i)
case $tip_ds in
  "0" ) p0="selected";;
  "1" ) p1="selected";;
  "2" ) p2="selected";;
  "3" ) p3="selected";;
esac
#
if [ "$i" -le "4" ]; then
  hid=" "
  onn="ON"
  off="OFF"
  Gpio="Gpio: "
  cels="&deg;C"
  LOW="LOW"
  HIGH="HIGH"
  requi_red="required"
#  cvet="#90f1f1"
  cvet="#f0f0f0"
else
  hid="hidden"
  onn=""
  off=""
  Gpio=""
  cels=""
  LOW=""
  HIGH=""
  requi_red=""
  cvet="#f0f0f0"
fi

outOff () {
  echo $gradus | awk ' {print $1/10} '
}
name_snr=$(uci get $cfl.@arduino[0].name_snr$i)
MAC=$(uci get $cfl.@arduino[0].MAC$i)
nGp=$(uci get $cfl.@arduino[0].nGp$i'_out')
invers=$(uci get $cfl.@arduino[0].invers$i)

echo "<form method="POST" action="saveApply.cgi"><tr><td align=center>$i: <input type=text name="poryadk" hidden value="
echo $i
echo "><input type=text name="dsPort" hidden value="$devPort"></td><td></td>

<td></td><td>
<input title="Hазвание_датчика_вписывать_от_1_до_13-символов_БЕЗ_ПРОБЕЛОВ_И_БЕЗ_ДВОЕТОЧИЯ!!!" class="b2" required="" type=text name="name_snr$i" maxlength=13 size=8 value="$name_snr">
<input type=text name="tname_snr$i" hidden value="$name_snr">
</td><td></td>

<td></td><td>
<input title="К_символам_НЕ_КРИТИЧНО!!_Заглавные_или_НЕТ..." class="b2" required="" type=text name="MAC$i" maxlength=16 size="${#MAC}" value="$MAC">
<input type=text name="tMAC$i" hidden value="$MAC">
</td><td></td>


<td></td><td class="b0" align=center bgcolor="$cvet">"
if [ "$i" -le "4" ]; then
echo "<select title="Выбор_прибора_по_типу_регулировки_температуры" name="tip_ds$i" class="b1">
<option value="0" label="Tермометр" $p0>"Tермометр"</option>
<option value="1" label="Термо-стат" $p1>"Термо-стат"</option>
<option value="2" label="Кондиционер" $p2>"Кондиционер"</option>
<option value="3" label="Насос_котла" $p3>"Насос_котла"</option>
<input type=text name="ttip_ds$i" hidden value="$tip_ds">
</select>"
else
echo Tермометр.
fi
echo "</td><td></td>


<td></td><td bgcolor="$cvet" align="center" class="b0">"
if [ "$i" -le "4" ]; then
echo "$Gpio<input class="b2" title="Номер_вывода_arduino_для_управления_силовыми_ключами" type=text name="nGp$i'_out'" $hid maxlength=2 size=2 value="$nGp">
<input type=text name="tnGp$i'_out'" hidden value="$nGp">"
fi
echo "</td><td></td>


<td></td><td bgcolor="$cvet" align="center" class="b0">"
if [ "$i" -le "4" ]; then
echo "<input title="Включить_активный_уровень_0_gpio-выхода" type=radio name="invers$i" value=0 $hid $nou>"$LOW"
<input title="Включить_активный_уровень_1_gpio-выхода" type=radio name="invers$i" value=1 $hid $yes>"$HIGH"
<input type=text name="tinvers$i" hidden value="$invers">"
fi
echo "</td><td></td>


<td></td><td bgcolor="$cvet" align="center" class="b0">"
if [ "$i" -le "4" ]; then
echo "<input class="b2" title="Максимум-99.9" $requi_red type=text name="alarm$i" $hid maxlength=4 size=4 value="
  gradus=$(uci get $cfl.@arduino[0].alarm$i)
  outOff
echo "><b>$cels</b><input type=text name="talarm$i" hidden value="$gradus">"
fi
echo "</td><td></td>


<td></td><td bgcolor="$cvet" align="center" class="b0">"
if [ "$i" -le "4" ]; then
echo "<input class="b2" title="Максимум-9.9" $requi_red type=text name="histe$i" $hid maxlength=4 size=4 value="
  gradus=$(uci get $cfl.@arduino[0].histe$i)
  outOff
echo "><b>$cels</b><input type=text name="thiste$i" hidden value="$gradus">"
fi
echo "</td><td></td>




<td></td><td align=center><button class="button" name="save_commit_sensor" type=submit title="Сохранить_$i" value="
echo $i
echo ">"Сохранить $i"</button></td></td><td></td><td></td></tr>"
echo "</form>"
: $((i = $i + 1))
done
echo "</table>"
fi
echo "<br><br>"

