#!/bin/sh
#Copyright (C) 2013 cyberwrt.ru, carduino.ru, cyber-place.ru, mp3car.ru
echo "Content-type: text/html; charset=utf-8"
echo
echo "<title>Пароль</title>"
echo `cat /www/menu.html`
if [ -f /tmp/install.sh ]; then
echo "<pre>`sh /tmp/install.sh`</pre>"
rm -f /tmp/install.sh
fi
conf="/etc/httpd.conf"
login=`echo "$QUERY_STRING" | sed -n 's/^.*login=\([^&]*\).*$/\1/p'`
pass=`echo "$QUERY_STRING" | sed -n 's/^.*pass=\([^&]*\).*$/\1/p'`
aunt=`echo "$QUERY_STRING" | sed -n 's/^.*aunt=\([^&]*\).*$/\1/p'`
if [ -n "$login" ] ; then
if [ "$aunt" = "on" ] ; then
sw="/"
else
sw="#/"
fi
echo "" > $conf
echo "$sw:$login:$pass" >> $conf
`/etc/init.d/uhttpd restart`
echo "<script>window.location.href=\"$SCRIPT_NAME\"</script>"
fi
login=$(cut -f2 -d: $conf)
pass=$(cut -f3 -d: $conf)
if echo `cat $conf` | egrep -q "#" ; then
no="checked"
bg="style=background-color:silver"
else
yes="checked"
bg="style=background-color:FFFFCC"
fi
echo "<h1>Модуль Аутентификации</h1>
<div $bg><form action=$SCRIPT_NAME method=GET>
Включить аутентификацию?
<input type=radio name=aunt value=on $yes>Да<input type=radio name=aunt value=off $no>Нет<br><br>
Логин: <input type=text name=login value=$login>
Пароль: <input type=text name=pass value=$pass>
<button type=submit>Сохранить</button></div>"