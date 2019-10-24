#!/bin/sh
#Copiright (C) 2013 cyberwrt.ru, carduino.ru, cyber-place.ru, mp3car.ru
categ="0"
categ=`echo "$QUERY_STRING" | sed -n 's/^.*categories=\([^&]*\).*$/\1/p' | sed "s/%20/ /g"`
mod=`echo "$QUERY_STRING" | sed -n 's/^.*mod=\([^&]*\).*$/\1/p' | sed "s/%20/ /g"`
action=`echo "$QUERY_STRING" | sed -n 's/^.*action=\([^&]*\).*$/\1/p' | sed "s/%20/ /g"`
cyberlogin=`echo "$QUERY_STRING" | sed -n 's/^.*cyberlogin=\([^&]*\).*$/\1/p' | sed "s/%20/ /g" | sed "s/%40/@/g"`
pass=`echo "$QUERY_STRING" | sed -n 's/^.*pass=\([^&]*\).*$/\1/p' | sed "s/%20/ /g"`
searchmod=`echo "$QUERY_STRING" | sed -n 's/^.*searchmod=\([^&]*\).*$/\1/p' | sed "s/%20/ /g"`
mdl=/www/cgi-bin/modules/
if echo "$QUERY_STRING" | egrep -q "searchmod=" ; then
QUERY_STRING=${QUERY_STRING//=$searchmod}
categ=""
fi
if echo "$QUERY_STRING" | egrep -q "cyberlogin=" ; then
echo "$cyberlogin:$pass" > $mdl/manmod/aunt.cyber
fi
if echo "$QUERY_STRING" | egrep -q "cyberlogin=" ; then
echo "$cyberlogin:$pass" > $mdl/manmod/aunt.cyber
fi
if echo "$QUERY_STRING" | egrep -q "definmod=" ; then
QUERY_STRING=${QUERY_STRING/definmod=}
`sed -i "/window.location.replace/d" /www/index.html`
`sed -i 2i\ "window.location.replace('/cgi-bin/modules/$QUERY_STRING/index.cgi');" /www/index.html`
fi
login=$(cut -f1 -d: $mdl/manmod/aunt.cyber)
pass=$(cut -f2 -d: $mdl/manmod/aunt.cyber)
reposit=cyberwrt.ru/application/modules/
if [ -f /www/cgi-bin/modules/manmod/1 ]; then
repo=" --user=$login --password=$pass http://$reposit$categ"
else
repo="http://$login:$pass@$reposit$categ"
fi


echo "Content-type: text/html; charset=utf-8"
echo 
echo "<title>Установка модулей</title>"
if [ $action == "install" ] ; then
mkdir $mdl$mod/
title=`wget -O - $repo/$mod/install.php?title`
`sed -i 3i\ '<li><a href=/cgi-bin/modules/'$mod'/index.cgi>'$title'</a></li>' /www/menu.html`
`wget -P /tmp/ $repo/$mod/install.sh`
for param in `wget -O - $repo/$mod/install.php` ; do
if echo "$param" | egrep -q "/modules/" ; then
mkdir /www/modules/$mod/
`wget -P /www/modules/$mod/ $repo/$mod/$param`
else
param=${param/.cgi}
`wget -P $mdl$mod/ $repo/$mod/$param`
`mv $mdl$mod/$param $mdl$mod/$param.cgi`
`chmod 747 $mdl$mod/$param.cgi`
fi
done
echo "<script>window.location.replace('/cgi-bin/modules/$mod/index.cgi');</script>"
fi
if [ $action == "remove" ] ; then
sed -i "/$mod\/index.cgi/d" /www/menu.html
`sed -i "/window.location.replace/d" /www/index.html`
`sed -i 2i\ "window.location.replace('/cgi-bin/modules/manmod/index.cgi');" /www/index.html`
`wget -P /tmp/ $repo/$mod/remove.sh`
`sh /tmp/remove.sh`
`rm -f /tmp/remove.sh`
for param in `ls $mdl$mod/` ; do
rm $mdl$mod/$param
done
rmdir $mdl$mod
if [ -e "/www/modules/$mod" ]
then
for param in `ls /www/modules/$mod/` ; do
rm /www/modules/$mod/$param
done
rmdir /www/modules/$mod/
fi
fi
echo "<link rel=stylesheet type=text/css href=http://$reposit/style.css>
<script type="text/javascript" src="/modules/manmod/module.js"></script>"
echo `cat /www/menu.html`
if [ $categ != "0" ]  || [ -n "$categ" ] ; then
echo "<br><b>Категория: $categ </b><a href=$SCRIPT_NAME>выбор категории</a>"
elif [ $QUERY_STRING == "searchmod" ] ; then
echo "<br><h2>Результат поиска: </h2><a href=$SCRIPT_NAME>выбор категрии</a>"
fi
echo "<table border=0 width=100%><tr><td>
<table border=0 cellspacing=10><tr><td width=130px>Доступно: `df -h | sed '/overlayfs/!d' | awk '{print $4}'`
</td><td><a style=\"cursor:pointer;\" onclick=\"show('cat1')\">Модуль по умолчанию</a></td>
<td><a style=\"cursor:pointer;\" onclick=\"show('cat2')\">Авторизация к серверу для установки модулей</a></td>
<td><a style=\"cursor:pointer;\" onclick=\"show('cat3')\">Поиск модуля на сервере</a></td>
</tr></table><div class=\"hidden\" id=\"cat1\"><form action=$SCRIPT_NAME method=GET>
<select size=1 name=definmod>"
for param in `ls $mdl` ; do
echo "<option>$param</option>"
done
echo "</select><input type=submit value=Отправить></form></div>
<div class=\"hidden\" id=\"cat2\"><form action=$SCRIPT_NAME method=GET>
Логин: <input type=text name=cyberlogin value=$login> Пароль: <input type=text name=pass value=$pass><input type=submit value=изменить></form></div>
<div class=\"hidden\" id=\"cat3\"><form action=$SCRIPT_NAME method=GET>
<input type=text name=searchmod><input type=submit value=Искать></form></div>
</td></tr><tr><td>"
if [ $QUERY_STRING == "searchmod" ] ; then
path=$repo/searchmod.php?searchmod=$searchmod
else
path=$repo/mod.php
#path="http://$reposit/mod.php"
fi
rep="http://$reposit$categ"
for param in `wget -O - $path` ; do
if [ $categ != "0" ] && [ $QUERY_STRING != "searchmod" ]; then
echo "<div class=leftmod><div class=mod><div align=center><a href=# onclick=window.open('$rep/$param/bigpic.png','Picture','width=555,height=555,toolbar=0,location=0')  title=Увеличить><img src=$rep/$param/tumb.png></a></div><div class=modinf>`wget -O - $repo/$param/install.php?module=$param`</div>"
i=$mdl$param/
if [ -e "$i" ]
then
echo "<a href=?categories=$categ&mod=$param&action=remove><div style=background-color:#CCFFCC; align=center>Remove"
else
echo "<a href=?categories=$categ&mod=$param&action=install><div style=background-color:silver; align=center>Install"
fi
echo "</div></a></div></div>"
elif [ $categ != "0" ] & [ $QUERY_STRING == "searchmod" ] ; then
categ=$(echo $param | tr '/' '\n' | head -1)
mod=$(echo $param | tr '/' '\n' | tail -n1)
echo "<div class=leftmod><div class=mod><div align=center><a href=# onclick=window.open('$rep/$param/bigpic.png','Picture','width=555,height=555,toolbar=0,location=0')  title=Увеличить><img src=$rep/$param/tumb.png></a></div><div class=modinf>`wget -O - $repo/$param/install.php?module=$param\&search=$searchmod`</div>"
i=$mdl$mod/
if [ -e "$i" ]
then
echo "<a href=?categories=$categ&mod=$mod&action=remove><div style=background-color:#CCFFCC; align=center>Remove"
else
echo "<a href=?categories=$categ&mod=$mod&action=install><div style=background-color:silver; align=center>Install"
fi
echo "</div></a></div></div>"
else
echo "<div class=leftcat><a href=?categories=$param><div class=cat><img src=$rep/$param/tumb.png><br>$param</div></a></div>"
fi
done
rss=`wget -O - http://cyberwrt.ru/rss.php`
echo "</td></tr></table>
<marquee scrollamount=3 style=\"color:white; background-color:black;\">$rss</marquee>"
