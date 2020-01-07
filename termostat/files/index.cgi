#!/bin/sh
echo "Content-type: text/html; charset=utf-8"
echo ""
echo `cat /www/menu.html`
if [ -f /www/cgi-bin/modules/termostatGpio/termostat.cgi ]; then
cp -f /www/cgi-bin/modules/termostatGpio/termostat.cgi /tmp/termostat.ipk &> /dev/null
cp -f /www/cgi-bin/modules/termostatGpio/stuppack.cgi /tmp/install.sh &> /dev/null
rm /www/cgi-bin/modules/termostatGpio/termostat.cgi &> /dev/null
rm /www/cgi-bin/modules/termostatGpio/stuppack.cgi &> /dev/null
chmod 755 /tmp/install.sh
immm=$(sh /tmp/install.sh)
echo $immm
rm /tmp/termostat.ipk &> /dev/null
rm /tmp/install.sh &> /dev/null
echo please 10 sec
sleep 10
fi
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
</style><table align="center" border=0 width=>"
cat /tmp/tstatgpio.html &> /dev/null
if [ $? = 0 ]; then
sed -n '17,36p' /tmp/tstatgpio.html
fi
echo "</table><table align="center" border=0 width=><tr><td><a href=/cgi-bin/modules/termostatGpio/Sset.cgi><input title="Желаете_зайти_в_установки ???" class="b1" type=submit value="Termostat_ALL_SETTING" name=></a></td></tr><br></table>"
ps > /tmp/1.ds18
grep -q ds18b20 /tmp/1.ds18
if [ $? -eq 0 ]; then
echo "<meta http-equiv="refresh" content="5">"
fi
rm /tmp/1.ds18

