#!/bin/sh
if [ "$1" != 0 ]; then
echo "Content-type: text/html; charset=utf-8"
fi
echo 
echo "<title>termostat-serial-arduino</title>"
confFile="termostat-serial-arduino"
w1_gpio=$(uci get $confFile.@arduino[0].w1_gpio)
d=$(uci get port_listen.@displ_tip[0].devPort)
f=$(uci get port_listen.@displ_tip[0].t_file)
b=$'/tmp/'
if [ "`uci get $confFile.@displ_tip[0].updateRead`" == "1" ] ; then
/www/cgi-bin/modules/termostat-serial-arduino/setupMenu.cgi
else
echo "<table align=center border=0><tr><td>"
echo `cat /www/menu.html`
echo "</td></tr></table>"
echo "
<body bgcolor="#f0f0f0">
<style>
  .b1 {
 text-decoration:none; text-align:center; 
 padding:5px 5px; 
 -webkit-border-radius:20px;
 -moz-border-radius:20px; 
 border-radius: 20px; 
 font:18px Verdana, Geneva, Tahoma, sans-serif; 
 font-weight:bold; 
 -webkit-box-shadow:0px 0px 2px #bababa, inset 0px 0px 1px #ffffff; 
 -moz-box-shadow: 0px 0px 2px #bababa,  inset 0px 0px 1px #ffffff;  
 box-shadow:0px 0px 2px #bababa, inset 0px 0px 1px #ffffff;  

 border:solid 1px #3bbbc7; 
 color:#fcffff; 
 background:#3bbbc7; 
 }
 h1 {
    font-family: Georgia, 'Times New Roman', Times, monospace;
   }
 .b1:hover {
 padding:5px 5px; 
 -webkit-border-radius:20px;
 -moz-border-radius:20px; 
 border-radius: 20px; 
 font:18px Verdana, Geneva, Tahoma, sans-serif; 
 font-weight:bold; 
 -webkit-box-shadow:0px 0px 2px #bababa, inset 0px 0px 1px #ffffff; 
 -moz-box-shadow: 0px 0px 2px #bababa,  inset 0px 0px 1px #ffffff;  
 box-shadow:0px 0px 2px #bababa, inset 0px 0px 1px #ffffff;  

 border:solid 1px #3ba4c7; 
 color:#050808; 
 background:#3bbbc7; 
  
 }
  .b1:active{
 padding:5px 5px; 
 -webkit-border-radius:20px;
 -moz-border-radius:20px; 
 border-radius: 20px; 
 font:18px Verdana, Geneva, Tahoma, sans-serif; 
 font-weight:bold; 
 -webkit-box-shadow:0px 0px 2px #bababa, inset 0px 0px 1px #ffffff; 
 -moz-box-shadow: 0px 0px 2px #bababa,  inset 0px 0px 1px #ffffff;  
 box-shadow:0px 0px 2px #bababa, inset 0px 0px 1px #ffffff;  

 border:solid 1px #005072; 
 color:#387575; 
 background:#e4eef2; 
 }
</style>"
rm $b$f &> /dev/null
#if [ "$1" == 0 ]; then
#sleep 2
#fi
echo r+=setT/ > /dev/tty$d
pidof port_listen &> /dev/null
if [ $? = 1 ]; then
 :
else
  a=1
  while [ 0 -ne "$a" ]; do
    cat $b$f &> /dev/null
    if [ $? = 0 ]; then
      let "a = 0"
    fi
  done
fi

echo "<table align="center" border=0 width=><tr><td>"
  cat /proc/cpuinfo | awk '{print $4}' | sed -n '1p'
echo "</span></td><tr></table><table align="center" border=0 width=><tr><td style='text-align: left; ' title="Имя_прибора_вписывать_без_пробелов"><span style='font-size: 20px;'>"
  uci get $confFile.@displ_tip[0].prbor
echo "</span></td><tr></table><table align="center" border=0 width=>"

cat /tmp/$f &> /dev/null
if [ $? != 1 ]; then
  kollw_str=$(sed -n '$=' /tmp/$f)
  echo "<br>"
  i=0
  while [ "$i" -ne "$kollw_str" ]; do
    : $((i = $i + 1))
    echo "<tr><td style='text-align: left; '><span style='color:#383838;'><span style='font-size: 30px;'><span style='font-family: Georgia, 'Times New Roman', Times, monospace; '>"
    uci get $confFile.@arduino[0].name_snr$i
    echo "": "</span></span></span></td>"
    echo "<td style='text-align: center; '><span style='color:#66cc00;'><span style='font-size: 50px;'><span style='font-family: Georgia, 'Times New Roman', Times, monospace; '>"
    sed -n ''$i'p' /tmp/$f
    echo "&deg;C</span></span></span></td></tr>"
  done
fi

echo "</table><table align="center" border=0 width=><tr><td><a href=/cgi-bin/modules/termostat-serial-arduino/sync.cgi><input title="Желаете_зайти_в_установки ???" class="b1" type=submit value="Настройка_arduino" name=></a></td></tr><br></table>"

fi
#
#if [ "$1" != 0 ]; then
# pidof port_listen &> /dev/null
# if [ $? = 1 ]; then
#  :
# else
#    cat $b$f &> /dev/null
#    if [ $? = 1 ]; then
#     :
#    else
#      echo "<meta http-equiv="refresh" content="3">"
#    fi
# fi
#fi
#
rm $b$f &> /dev/null





