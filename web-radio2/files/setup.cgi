#!/bin/sh
#Copyright (C) 2013 cyberwrt.ru, carduino.ru, cyber-place.ru, mp3car.ru
echo 
echo `cat /www/menu.html`
echo "<script type="text/javascript">function startTime(){ var tm=new Date(); var h=tm.getHours(); var m=tm.getMinutes(); var s=tm.getSeconds(); m=checkTime(m); s=checkTime(s);document.getElementById('txt').innerHTML=h+":"+m+":"+s; t=setTimeout('startTime()',500); }
function checkTime(i){ if (i<10) { i="0" + i;} return i; } </script>"
echo "<title>WEB-RADIO-2</title>"

#flconf='i2c_HD44780'
#tfile=$(uci get $flconf.@display[0].read_file)

playlist=/etc/config/web-radio2
tfile=/tmp/play_name
bitwebradio2=0

cat /etc/rc.d/S99web-radio2.init &> /dev/null
if [ $? == 1 ]; then
ontxtoff2=Желаете_включить_авто-запуск_???
ontxtoff=Желаете_отключить_авто-запуск_???
onoff=autostarton
cto=OFF
clor="ff0000"
dsd=""
dsd2=disabled
star_tup=1
else
star_tup=0
dsd=disabled
dsd2=""
clor="0000ff"
ontxtoff2=Желаете_отключить_авто-запуск_???
ontxtoff=Желаете_включить_авто-запуск_???
onoff=autostartoff
cto=ON
fi
#********************************************8
echo "
<body bgcolor="#f0f0f0">
<style>
  .b0 {
cursor: pointer;
}
  .b1 {
font-size: 15pt;
text-align: center;
padding: 5px 30px;
margin-left: 1px;
cursor: pointer;
border-radius:inherit;
}
  .b2 {
font-size: 12pt;
text-align: left;
padding: 2px 10px;
margin-left: 5px;
cursor: pointer;
border-radius:inherit;
}
  .b3 {
font-size: 12pt;
text-align: center;
padding: 2px 50px;
margin-left: 5px;
cursor: pointer;
border-radius:inherit;
}
   textarea {
    resize: none; /* Запрещаем изменять размер */
   } 
</style>"
#____________________________________________________________
echo "<table><tr><td>

<table><tr><td>
<div class="header"><span id="txt">"
pidof web-radio2 &> /dev/null
if [ $? == 1 ]; then
bitwebradio2=0
   pidof madplay &> /dev/null
   if [ $? == 1 ]; then
	echo "Off"
   else
	echo "URL: <b>"
	cat $tfile
	echo "</b>"
   fi

else
bitwebradio2=1
   pidof curl &> /dev/null
   if [ $? == 1 ]; then
	echo "URL: <b>"
	cat $tfile
	echo "</b>"
   else
	echo "Radio: <b>"
	cat $tfile
	echo "</b>"
   fi
fi
echo "<td><tr></table>
</span></div>"


#____________________________________________________________
#____________________________________________________________
echo "
<table width="100" border="0">
<form method="POST" action="save.cgi"><tr><td valign="vertical">
<input class="b0" size="70" name="URL" required placeholder="http://chanson.hostingradio.ru:8041/chanson-uncensored128.mp3" title="URL-адрес_аудио_потока"></td><td>
<input class="b0" type="submit" value="Play-URL">
</td></tr></form>
<form method="POST" action="save.cgi"><tr><td valign="vertical">
<input class="b0" size="70" name="file" required placeholder="/home/Шансон/044.Очаровала.mp3" title="Полный_путь_к_аудио_файлу"></td><td>
<input class="b0" type="submit" value=" Play-file ">
</td></tr></form>
<form method="POST" action="save.cgi"><tr><td valign="vertical">
<input class="b0" size="70" name="folder" required placeholder="/home/Шансон" title="Имя_папки_с_аудио_файлами"></td><td>
<input class="b0" type="submit" value="Play-folder">
</td></tr></form>
</table>"
#===================================================================================
#===================================================================================

c=$(sed -n '$=' $playlist)
let "b = (("$c" / 2))"
a=1

echo "
<table width="100"><tr><th></th><th>
<table align="left"><tr><td><form method="POST" action="save.cgi">"
pidof web-radio2 &> /dev/null
if [ $? == 1 ]; then
   pidof madplay &> /dev/null
   if [ $? == 1 ]; then
	echo "<button class="b1" name="zapusk" type=submit title="Желаете_запусить_модуль_???"><img src="/modules/web-radio2/b.jpg" alt= style="vertical-align: middle">"    RADIO START"</button>"
   else
	echo "<button class="b1" name="ostanov" type=submit title="Желаете_остановить_модуль_???"><img src="/modules/web-radio2/r.jpg" alt= style="vertical-align: middle">"    madplay STOP"</button>"
   fi
else
   pidof curl &> /dev/null
   if [ $? == 1 ]; then
	echo "<button class="b1" name="ostanov" type=submit title="Желаете_остановить_модуль_???"><img src="/modules/web-radio2/r.jpg" alt= style="vertical-align: middle">"    madplay STOP"</button>"
   else
	echo "<button class="b1" name="ostanov" type=submit title="Желаете_остановить_модуль_???"><img src="/modules/web-radio2/r.jpg" alt= style="vertical-align: middle">"    RADIO STOP"</button>"
   fi
fi
echo "</form></td></tr></table>

<table align="centr"><tr><td><form method="POST" action="save.cgi">
<button name="next" type="submit" title="Следующий_канал" class="b1"> "  Next chennel"</button>
</form></td></tr></table></th><th></th></tr>"

while [ "$a" -le "$b" ]; do
name=$(cat $playlist | awk -F , '/#EXTINF:'-$a',/ {print $2}')
url=$(sed -n '/#EXTINF:'-$a'/{n;p;q;}' $playlist)
echo "<form method="POST" action="save.cgi"><tr>
<td valign="vertical"><input name="nomerstroki" hidden value="$a">"$a."</td>
<td valign="vertical">
<input class="b2" name="nameURL" size="50" readonly value="
   printf "$name"
   echo " required title="$url" placeholder="список_пуст">
<input class="b2" name="URL2" readonly value="
   printf "$url"
   echo " required hidden>
</td><td valign="vertical">
<input class="b2" type="submit" value="play">
</td></tr></form>"
let "a += 1"
done
echo "</table><br>"
#===================================================================================
#===================================================================================
c=$(sed -n '$=' $playlist)
let "b = (("$c" / 2))"
let "b += 1"
echo "<table>
<form method="POST" action="save.cgi"><table><tr><td align="left"><p>"
printf $b.
echo "
<input size="2" hidden name="n_str" value="
printf $b
echo " required placeholder="add_nev_name">
<input class="b2" size="10" name="add_nev_name" required placeholder="add_name">

<input class="b2" size="35" name="add_nev_url" required placeholder="add_url">
<input class="b2" type="submit" value="Save"></p></td></tr></table>
</form></table>"

#===================================================================================
#===================================================================================
echo "<table><tr><td>.</td><td><p style="color:#$clor">Auto Startup - <b>$cto</b></p></td><td>
<form method="POST" action="save.cgi">"
if [ $star_tup == 1 ]; then
echo "<button $dsd name="$onoff" type=submit title="$ontxtoff" class="b1"><img src=/modules/web-radio2/b.jpg alt= style="vertical-align: middle"> "  Enable  ."</button>"
else
echo "<button $dsd2 name="$onoff" type=submit title="$ontxtoff2" class="b1"><img src=/modules/web-radio2/r.jpg alt= style="vertical-align: middle"> "  Disable  ."</button>"
fi
echo "</form></td></tr></table><br><br>"
#____________________________________________________________
#____________________________________________________________
kollw_str=$(sed -n '$=' $playlist)
echo "<table><tr><td><form method="POST" action="save.cgi">
<p><textarea rows="$kollw_str" cols="64" name="text" spellcheck="false" placeholder=Play-list? >"
cat $playlist
#sed -n '/#EXTINF:/{n;p;}' $playlist
echo "</textarea></td></tr><tr><td ><br><input type="submit" class="b1" value="Save-Play-list" name=></p>
</form></td></tr></table>
</td></tr></table>
 </body>"
#____________________________________________________________



