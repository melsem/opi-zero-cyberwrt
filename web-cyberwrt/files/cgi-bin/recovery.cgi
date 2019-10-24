#!/bin/sh
echo "Content-type: text/html; charset=utf-8"
echo

ttimer() {
sync
echo "Устройство перезагрузится через <b><span id="time"></span></b> секунд.
<script type="text/javascript">var i = 30; function time(){ document.getElementById(\"time\").innerHTML = i; i--; if (i < 0) location.href = \"/index.html\"; }
time(); setInterval(time, 1000); </script>"
reboot
}
reb=`echo "$QUERY_STRING" | sed -n 's/^.*reb=\([^&]*\).*$/\1/p' | sed "s/%20/ /g"`
all=`echo "$QUERY_STRING" | sed -n 's/^.*all=\([^&]*\).*$/\1/p' | sed "s/%20/ /g"`
if echo "$QUERY_STRING" > /dev/nul; then
if [ "$reb" = "ON" ] ; then
sync
sleep 2
ttimer
exit 0
fi
if [ "$all" = "ON" ] ; then
jffs2reset -y &
echo "FACTORY RESET - Выполняется!<br>"
ttimer
exit 0
fi
fi
echo "<script>
function confirm1(f) { if (confirm(\"Откатить к первоначальному состоянию ( FACTORY RESET )? Это займет около 30 секунд.\")) f.submit(); }
function reboot(f) {if (confirm(\"Перезагрузть? Это займет около 30 секунд.\")) f.submit(); }
</script>`cat /www/menu.html`
<h1>Востановление к начальным установкам</h1><form action=$SCRIPT_NAME methot=GET onsubmit=\"confirm1(this);return false;\">
<input type=checkbox name=all checked=checked hidden value=ON><button title="Сбросить_настройки_и_перезагрузить" type=submit name=confirm>FACTORY RESET</button></form>
<h1>Перезагрузka</h1><form action=$SCRIPT_NAME methot=GET onsubmit=\"reboot(this);return false;\">
<input type=checkbox name=reb checked=checked hidden value=ON><button title="Перезагрузить?" type=submit name=confirm>Reboot</button></form>"

