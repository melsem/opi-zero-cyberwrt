#!/bin/sh
#Copyright (C) 2013 cyberwrt.ru, carduino.ru, cyber-place.ru, mp3car.ru

echo "Content-type: text/html; charset=utf-8"
echo ""

if [ -f /www/menu.html ]; then
    cat /www/menu.html
fi

conf_DS="/tmp/digitemp.conf"
cnf_term="/etc/config/i2c_HD44780"
flconf="i2c_HD44780"

kollwo_usb=$(sed -n '/ROM/p' $conf_DS 2>/dev/null | wc -l)
kollwo_gpio=$(cat /sys/devices/w1_bus_master1/w1_master_slave_count 2>/dev/null)

w1_gpio=$(uci -q get $flconf.@ds18b20[0].w1_gpio)
usb_w1_uart=$(uci -q get $flconf.@ds18b20[0].usb_w1_uart)
sda_gpio=$(uci -q get $flconf.@display[0].sda_gpio)
scl_gpio=$(uci -q get $flconf.@display[0].scl_gpio)
lcd_number=$(uci -q get $flconf.@display[0].lcd_number)
gros_simvol=$(uci -q get $flconf.@display[0].gros_simvol)
d=$(uci -q get $flconf.@display[0].device)
a=$(uci -q get $flconf.@display[0].address)
f=$(uci -q get $flconf.@display[0].read_file)

sa=""; sb=""
case $lcd_number in
"16" ) sa="selected";;
"20" ) sb="selected";;
esac

p0=""; p1=""; p2=""; p3=""; p4=""; p5=""; p6=""
case $gros_simvol in
  "0" ) p0="selected";;
  "1" ) p1="selected";;
  "2" ) p2="selected";;
  "3" ) p3="selected";;
  "4" ) p4="selected";;
  "5" ) p5="selected";;
  "6" ) p6="selected";;
esac

if [ "$(uci -q get $flconf.@display[0].info_banner)" = "1" ] ; then
    ok_banner="checked"; no_banner=""
else
    ok_banner=""; no_banner="checked"
fi

if [ "$(uci -q get $flconf.@displ_tip[0].while)" = "1" ] ; then
    ok_while="checked"; no_while=""
else
    ok_while=""; no_while="checked"
fi

clock_work=""
if [ "$(uci -q get $flconf.@displ_tip[0].clock)" = "1" ] ; then
    ok_clock="checked"; no_clock=""; clock_work="Clock ON. "
else
    ok_clock=""; clock_work=""; no_clock="checked"
fi

if [ "$(uci -q get $flconf.@displ_tip[0].led_on)" = "1" ] ; then
    ok_led_on="checked"; no_led_on=""
else
    ok_led_on=""; no_led_on="checked"
fi

if [ "$(uci -q get $flconf.@displ_tip[0].on_read)" = "1" ] ; then
    ok_on_read="checked"; no_on_read=""
else
    ok_on_read=""; no_on_read="checked"
fi

w1gpio=""; digitemp=""; no_ds18=""; sens_or=0; fl_ds=""
on_ds18_val=$(uci -q get $flconf.@displ_tip[0].on_ds18)
if [ "$on_ds18_val" = "0" ] ; then
    w1gpio=""; digitemp=""; fl_ds="<b>Works.</b> LCD monitor. $clock_work"; no_ds18="checked"; sens_or=0
elif [ "$on_ds18_val" = "1" ] ; then
    w1gpio="checked"; digitemp=""; fl_ds="<b>Works.</b> $clock_work Found sensors - <b>$kollwo_gpio</b> pieces. Init gpio-port: <b>$w1_gpio</b>"; no_ds18=""; sens_or=1
elif [ "$on_ds18_val" = "2" ] ; then
    w1gpio=""; digitemp="checked"; fl_ds="<b>Works.</b> $clock_work Found sensors: <b>$kollwo_usb</b> pieces.  Init.Port: <b>$usb_w1_uart</b>"; no_ds18=""; sens_or=2
fi

if [ "$(uci -q get $flconf.@displ_tip[0].tip_ds1)" = "1" ] ; then ok_termostat1="checked"; no_termostat1=""; else ok_termostat1=""; no_termostat1="checked"; fi
if [ "$(uci -q get $flconf.@displ_tip[0].tip_ds2)" = "1" ] ; then ok_termostat2="checked"; no_termostat2=""; else ok_termostat2=""; no_termostat2="checked"; fi
if [ "$(uci -q get $flconf.@displ_tip[0].tip_ds3)" = "1" ] ; then ok_termostat3="checked"; no_termostat3=""; else ok_termostat3=""; no_termostat3="checked"; fi
if [ "$(uci -q get $flconf.@displ_tip[0].tip_ds4)" = "1" ] ; then ok_termostat4="checked"; no_termostat4=""; else ok_termostat4=""; no_termostat4="checked"; fi

if [ ! -f /etc/rc.d/S98i2c_HD44780.init ]; then
    ontxtoff2=Желаете_включить_авто-запуск_???; ontxtoff=Желаете_отключить_авто-запуск_???; onoff=staron; sohid=Disable; sotxthid=Enable; kart='/modules/i2c_HD44780/b.jpg'; cto=OFF; clor="ff0000"; dsd=""; dsd2=disabled
else
    dsd=disabled; dsd2=""; clor="0000ff"; ontxtoff2=Желаете_отключить_авто-запуск_???; ontxtoff=Желаете_включить_авто-запуск_???; onoff=staroff; sohid=Enable; sotxthid=Disable; kart='/modules/i2c_HD44780/r.jpg'; cto=ON
fi

sr1=$(uci -q get $flconf.@ds18b20[0].sensor1); sr2=$(uci -q get $flconf.@ds18b20[0].sensor2); sr3=$(uci -q get $flconf.@ds18b20[0].sensor3); sr4=$(uci -q get $flconf.@ds18b20[0].sensor4)
nm1=$(uci -q get $flconf.@ds18b20[0].name_snr1); nm2=$(uci -q get $flconf.@ds18b20[0].name_snr2); nm3=$(uci -q get $flconf.@ds18b20[0].name_snr3); nm4=$(uci -q get $flconf.@ds18b20[0].name_snr4)
nGp1_out=$(uci -q get $flconf.@ds18b20[0].nGp1_out); nGp2_out=$(uci -q get $flconf.@ds18b20[0].nGp2_out); nGp3_out=$(uci -q get $flconf.@ds18b20[0].nGp3_out); nGp4_out=$(uci -q get $flconf.@ds18b20[0].nGp4_out)
alarm1=$(uci -q get $flconf.@ds18b20[0].alarm1); alarm2=$(uci -q get $flconf.@ds18b20[0].alarm2); alarm3=$(uci -q get $flconf.@ds18b20[0].alarm3); alarm4=$(uci -q get $flconf.@ds18b20[0].alarm4)
histe1=$(uci -q get $flconf.@ds18b20[0].histe1); histe2=$(uci -q get $flconf.@ds18b20[0].histe2); histe3=$(uci -q get $flconf.@ds18b20[0].histe3); histe4=$(uci -q get $flconf.@ds18b20[0].histe4)

if [ "$(uci -q get $flconf.@ds18b20[0].invers1)" = "1" ] ; then yss1="checked"; noo1=""; else yss1=""; noo1="checked"; fi
if [ "$(uci -q get $flconf.@ds18b20[0].invers2)" = "1" ] ; then yss2="checked"; noo2=""; else yss2=""; noo2="checked"; fi
if [ "$(uci -q get $flconf.@ds18b20[0].invers3)" = "1" ] ; then yss3="checked"; noo3=""; else yss3=""; noo3="checked"; fi
if [ "$(uci -q get $flconf.@ds18b20[0].invers4)" = "1" ] ; then yss4="checked"; noo4=""; else yss4=""; noo4="checked"; fi

if pidof i2c_HD44780 >/dev/null 2>&1; then
    commit="save_com_restart"; fil_ds=$fl_ds; but_txt="i2c_hd44780 STOP"; dvig="coostanov"; dvig_txt="Желаете_остановить_модуль_???"; dvig_imag="/modules/i2c_hd44780/r.jpg"
else
    commit="save_commit"; but_txt="i2c_hd44780 START"; fil_ds="Not Work"; dvig="cozapusk"; dvig_txt="Желаете_запусить_модуль_???"; dvig_imag="/modules/i2c_hd44780/b.jpg"
fi

if [ "$on_ds18_val" = "2" ] || [ "$on_ds18_val" = "0" ]; then hidden="hidden"; else hidden=""; fi
if [ "$on_ds18_val" = "0" ]; then hien=""; else hien="hidden"; fi

echo "<style>
  body { font-family: 'Segoe UI', Arial, sans-serif; background-color: #f4f6f9; color: #333; margin: 0; padding: 20px; }
  table { border-collapse: separate; border-spacing: 0; background: #ffffff; border-radius: 12px; box-shadow: 0 4px 12px rgba(0,0,0,0.05); margin: 15px auto; padding: 15px; width: 100%; max-width: 900px; }
  caption { font-size: 18px; font-weight: bold; color: #2c3e50; margin-bottom: 12px; }
  td { padding: 10px 15px; border-bottom: 1px solid #edf2f7; }
  tr:last-child td { border-bottom: none; }
  td[bgcolor='#90f1f1'] { background-color: #f8fafc !important; font-weight: 500; color: #4a5568; }
  input[type='text'], select { border: 1px solid #cbd5e0; border-radius: 6px; padding: 6px 10px; font-size: 14px; color: #4a5568; outline: none; }
  .b1 { font-size: 14px; font-weight: bold; padding: 12px 28px; background-color: #34495e; color: #fff; cursor: pointer; border: none; border-radius: 6px; transition: all 0.15s ease; text-transform: uppercase; }
  .b1:hover { background-color: #2c3e50; }
  
  .switch-container { display: inline-flex; background: #edf2f7; padding: 3px; border-radius: 6px; gap: 2px; }
  .switch-container input[type='radio'] { display: none; }
  .switch-container label { padding: 4px 12px; font-size: 13px; font-weight: bold; color: #718096; cursor: pointer; border-radius: 4px; transition: all 0.15s ease; user-select: none; }
  .switch-container input[type='radio']:checked + label { background: #ffffff; color: #2d3748; box-shadow: 0 2px 4px rgba(0,0,0,0.08); }
  .switch-container input[type='radio'][value='1']:checked + label { color: #2ecc71; }
  .switch-container input[type='radio'][value='2']:checked + label { color: #3498db; }
  .btn-green { background-color: #2ecc71 !important; color: white; }
  .btn-green:hover { background-color: #27ae60 !important; }
  .btn-red { background-color: #e74c3c !important; color: white; }
  .btn-red:hover { background-color: #c0392b !important; }
  .btn-disabled { background-color: #bdc3c7 !important; color: #7f8c8d !important; cursor: not-allowed; }
</style>"

echo "<form method='POST' action='Save.cgi'>"
echo "<table>"
echo "<caption><b>General setup i2c_HD44780</b></caption>"
echo "<tr><td align='right'><b>display</b></td><td><b>setup</b></td></tr>"

echo "<tr><td bgcolor='#90f1f1'>auto restart (loop-while)</td><td>"
echo "  <div class='switch-container'>"
echo "    <input type='radio' id='w_0' name='while' value='0' $no_while><label for='w_0'>Off</label>"
echo "    <input type='radio' id='w_1' name='while' value='1' $ok_while><label for='w_1'>ON</label>"
echo "  </div>"
echo "</td></tr>"

echo "<tr><td bgcolor='#90f1f1'>i2c-Dev</td><td>Dev . <input type='text' name='device' size='2' value='$d'></td></tr>"
echo "<tr><td bgcolor='#90f1f1'>I2C_slave-adres</td><td>hex . <input type='text' name='address' size='3' value='$a'></td></tr>"
echo "<tr><td bgcolor='#90f1f1'>LCD_GPIO-SDA_SCL</td><td>sda-scl <input type='text' name='sda_gpio' size='2' value='$sda_gpio'><input type='text' name='scl_gpio' size='2' value='$scl_gpio'></td></tr>"

echo "<tr><td bgcolor='#90f1f1'>String length_LCD</td><td>lcd . "
echo "  <select name='lcd_number'>"
echo "    <option value='16' $sa>16.</option>"
echo "    <option value='20' $sb>20.</option>"
echo "  </select>"
echo "</td></tr>"

echo "<tr><td bgcolor='#90f1f1'>WIDE</td><td>"
echo "  <select name='gros_simvol'>"
echo "    <option value='0' $p0>OK по одной строке</option>"
echo "    <option value='1' $p1>OK в две строки</option>"
echo "    <option value='2' $p2>OK в две строки нижний демо</option>"
echo "    <option value='3' $p3>OK Clock в четыре строки</option>"
echo "    <option value='4' $p4>OK в четыре строки демо</option>"
echo "    <option value='5' $p5>пусто1</option>"
echo "    <option value='6' $p6>пусто2</option>"
echo "  </select>"
echo "</td></tr>"

echo "<tr $hien><td bgcolor='#90f1f1'>file name:</td><td>/tmp/<input type='text' name='read_file' size='20' value='$f'></td></tr>"

echo "<tr $hien><td bgcolor='#90f1f1'>LCD: info banner</td><td>"
echo "  <div class='switch-container'>"
echo "    <input type='radio' id='b_0' name='info_banner' value='0' $no_banner><label for='b_0'>Off</label>"
echo "    <input type='radio' id='b_1' name='info_banner' value='1' $ok_banner><label for='b_1'>ON</label>"
echo "  </div>"
echo "</td></tr>"

echo "<tr $hien><td bgcolor='#90f1f1'>file /temp/<b>$f</b> (read)</td><td>"
echo "  <div class='switch-container'>"
echo "    <input type='radio' id='r_0' name='on_read' value='0' $no_on_read><label for='r_0'>Off</label>"
echo "    <input type='radio' id='r_1' name='on_read' value='1' $ok_on_read><label for='r_1'>ON</label>"
echo "  </div>"
echo "</td></tr>"

echo "<tr><td bgcolor='#90f1f1'>clock</td><td>"
echo "  <div class='switch-container'>"
echo "    <input type='radio' id='c_0' name='clock' value='0' $no_clock><label for='c_0'>Off</label>"
echo "    <input type='radio' id='c_1' name='clock' value='1' $ok_clock><label for='c_1'>ON</label>"
echo "  </div>"
echo "</td></tr>"

echo "<tr><td bgcolor='#90f1f1'>led - </td><td>"
echo "  <div class='switch-container'>"
echo "    <input type='radio' id='l_0' name='led_on' value='0' $no_led_on><label for='l_0'>Off</label>"
echo "    <input type='radio' id='l_1' name='led_on' value='1' $ok_led_on><label for='l_1'>ON</label>"
echo "  </div>"
echo "</td></tr>"

echo "<tr><td align='right'><b>sensor</b></td><td><b>setup</b></td></tr>"
echo "<tr><td bgcolor='#90f1f1'>sensor ds18</td><td>"
echo "  <div class='switch-container'>"
echo "    <input type='radio' id='d_0' name='on_ds18' value='0' $no_ds18><label for='d_0'>Off</label>"
echo "    <input type='radio' id='d_1' name='on_ds18' value='1' $w1gpio><label for='d_1'>w1-gpio</label>"
echo "    <input type='radio' id='d_2' name='on_ds18' value='2' $digitemp><label for='d_2'>usb-uart</label>"
echo "  </div>"
echo "</td></tr>"

if [ "$sens_or" = "0" ] ; then
    echo "<tr><td bgcolor='#90f1f1'>sensor ds18 Off</td><td>sensor ds18 Off</td></tr>"
elif [ "$sens_or" = "1" ] ; then
    echo "<tr><td bgcolor='#90f1f1'>w1_gpio ds1820</td><td>gpio-pin . <input type='text' name='w1_gpio' size='5' value='$w1_gpio'></td></tr>"
elif [ "$sens_or" = "2" ] ; then
    echo "<tr><td bgcolor='#90f1f1'>usb-uart ds1820</td><td>/dev/tty<input type='text' name='usb_w1_uart' size='5' value='$usb_w1_uart'></td></tr>"
fi
echo "</table>"

echo "<table>"
echo "<tr><td></td><td></td><td></td><td align='right'><b>Thermostat</b></td><td><b>setup</b></td><td></td><td></td></tr>"
echo "<tr>"
echo "  <td bgcolor='#90f1f1' $hidden>Thermostat</td>"
echo "  <td bgcolor='#90f1f1' $hidden>Pin out</td>"
echo "  <td bgcolor='#90f1f1' $hidden>Invers-out-gpio</td>"
echo "  <td bgcolor='#90f1f1' $hidden>Power-OFF</td>"
echo "  <td bgcolor='#90f1f1' $hidden>Power-ON</td>"
echo "  <td bgcolor='#90f1f1'>Nickname sensor</td>"
echo "  <td bgcolor='#90f1f1'>W1 ID-sensor</td>"
echo "</tr>"

for i in 1 2 3 4; do
    eval ok_term=\$ok_termostat$i
    eval no_term=\$no_termostat$i
    eval inv_0=\$noo$i
    eval inv_1=\$yss$i
    eval nGp=\$nGp${i}_out
    eval alarm=\$alarm$i
    eval histe=\$histe$i
    eval nm=\$nm$i
    eval sr=\$sr$i

    echo "<tr>"
    echo "  <td $hidden><div class='switch-container'><input type='radio' id='t0_$i' name='tip_ds$i' value='0' $no_term><label for='t0_$i'>Off</label><input type='radio' id='t1_$i' name='tip_ds$i' value='1' $ok_term><label for='t1_$i'>ON</label></div></td>"
    echo "  <td $hidden style='white-space: nowrap;'>Gpio-<input type='text' name='nGp${i}_out' style='width: 30px; text-align: center; padding: 4px;' value='$nGp'></td>"
    echo "  <td $hidden><div class='switch-container'><input type='radio' id='i0_$i' name='invers$i' value='0' $inv_0><label for='i0_$i'>0</label><input type='radio' id='i1_$i' name='invers$i' value='1' $inv_1><label for='i1_$i'>1</label></div></td>"
    echo "  <td $hidden style='white-space: nowrap;'><input type='text' name='alarm$i' style='width: 50px; text-align: center; padding: 4px;' value='$alarm'> <b>&deg;C</b></td>"
    echo "  <td $hidden style='white-space: nowrap;'><input type='text' name='histe$i' style='width: 50px; text-align: center; padding: 4px;' value='$histe'> <b>&deg;C</b></td>"
    echo "  <td><input type='text' name='name_snr$i' size='8' value='$nm'></td>"
    echo "  <td><input type='text' name='sensor$i' size='16' value='$sr'></td>"
    echo "</tr>"
done
echo "</table>"

echo "<table align='center'><tr><td align='right'><button name='$commit' class='b1 btn-green' type='submit'>Save</button></td></tr></table>"
echo "</form>"

echo "<form method='POST' action='Save.cgi'>"
echo "<table align='center'>"
echo "  <tr><td align='right'><p style='color:#$clor; margin: 0;'>Auto Startup - <b>$cto</b></p></td></tr>"
echo "  <tr><td align='right'>"
echo "    <div style='display: flex; justify-content: flex-end; gap: 10px;'>"

if [ -z "$dsd" ]; then
    echo "      <button name='$onoff' type='submit' class='b1 btn-green'>Enable</button>"
else
    echo "      <button disabled class='b1 btn-disabled'>Enable</button>"
fi

if [ -z "$dsd2" ]; then
    echo "      <button name='$onoff' type='submit' class='b1 btn-red'>Disable</button>"
else
    echo "      <button disabled class='b1 btn-disabled'>Disable</button>"
fi

echo "    </div>"
echo "  </td></tr>"
echo "</table>"
echo "</form>"

echo "<form method='POST' action='Save.cgi'>"
echo "<table align='center'>"
echo "  <tr><td align='right'>$fil_ds</td></tr>"
echo "  <tr><td align='right'>"

if [ "$commit" = "save_com_restart" ]; then
    echo "    <button class='b1 btn-red' name='$dvig' type='submit'>$but_txt</button>"
else
    echo "    <button class='b1 btn-green' name='$dvig' type='submit'>$but_txt</button>"
fi

echo "  </td></tr>"
echo "</table>"
echo "</form>"

echo "</body></html>"

