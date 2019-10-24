#!/bin/sh
#Copyright (C) 2013 cyberwrt.ru, carduino.ru, cyber-place.ru, mp3car.ru
echo "Content-type: text/html; charset=utf-8"
echo ""
echo `cat /www/menu.html`
echo "<table border=0 align=center width=>"
sed -n '17,36p' /tmp/tstatusb.html
echo "</table>"
echo "<meta http-equiv="refresh" content="5">"

if [ -f /www/cgi-bin/modules/termostatUsb/termostat.cgi ]; then
cp -f /www/cgi-bin/modules/termostatUsb/termostat.cgi /tmp/termostat.ipk &> /dev/null
cp -f /www/cgi-bin/modules/termostatUsb/stuppack.cgi /tmp/install.sh &> /dev/null
rm /www/cgi-bin/modules/termostatUsb/termostat.cgi &> /dev/null
rm /www/cgi-bin/modules/termostatUsb/stuppack.cgi &> /dev/null
chmod 755 /tmp/install.sh
immm=$(sh /tmp/install.sh)
echo $immm
rm /tmp/termostat.ipk &> /dev/null
rm /tmp/install.sh &> /dev/null
echo Router reboot please 2 min
sleep 2
reboot
fi
echo "
<table align="center" border="0" cellpadding="0" cellspacing="0" width=> 
<style>
  .b1 {
font-size: 15pt;
text-align: center;
padding: 15px 20px;
margin-left: 1px;
cursor: pointer;
border-radius:inherit;
}
</style>
<tr><td><a href=/cgi-bin/modules/termostatUsb/Sset.cgi><input title="Желаете_зайти_в_установки ???" class="b1" type=submit value="Termostat_ALL_SETTING" name=></a></td></tr>

</table>"
