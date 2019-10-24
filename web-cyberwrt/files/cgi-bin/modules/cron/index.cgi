#!/bin/sh
#Copiright (C) 2013 cyberwrt.ru, carduino.ru, cyber-place.ru, mp3car.ru
echo "Content-type: text/html; charset=utf-8"
echo
echo "<title>Планировщик</title>`cat /www/menu.html`"
script=$(echo -e $(echo "$QUERY_STRING" | sed -n "s/^.*script=\([^&]*\).*$/\1/p" | sed "s/+/ /g" |sed -e 's/%\(\)/\\\x/g'))
#`echo "$QUERY_STRING" | sed -n 's/^.*script=\([^&]*\).*$/\1/p' | sed "s/%3A//g" | sed "s/%2F/\//g" | sed "s/+/ /g" | sed "s/%2C/,/g" | sed 's/%22/"/g'`
min=`echo "$QUERY_STRING" | sed -n 's/^.*min=\([^&]*\).*$/\1/p' | sed "s/%3A//g" | sed "s/%2F/\//g" | sed "s/%2C/,/g"`
hour=`echo "$QUERY_STRING" | sed -n 's/^.*hour=\([^&]*\).*$/\1/p' | sed "s/%3A//g" | sed "s/%2F/\//g" | sed "s/%2C/,/g"`
day=`echo "$QUERY_STRING" | sed -n 's/^.*day=\([^&]*\).*$/\1/p' | sed "s/%3A//g" | sed "s/%2F/\//g" | sed "s/%2C/,/g"`
month=`echo "$QUERY_STRING" | sed -n 's/^.*month=\([^&]*\).*$/\1/p' | sed "s/%3A//g" | sed "s/%2F/\//g" | sed "s/%2C/,/g"`
week=`echo "$QUERY_STRING" | sed -n 's/^.*week=\([^&]*\).*$/\1/p' | sed "s/%3A//g" | sed "s/%2F/\//g" | sed "s/%2C/,/g"`
act=`echo "$QUERY_STRING" | sed -n 's/^.*act=\([^&]*\).*$/\1/p'`
string=`echo "$QUERY_STRING" | sed -n 's/^.*string=\([^&]*\).*$/\1/p'`
task="$min $hour $day $month $week $script"
rel="<script>window.location.href=\"$SCRIPT_NAME\"</script>"
if [ -f /tmp/install.sh ]; then
echo "<pre>`sh /tmp/install.sh`</pre>"
rm -f /tmp/install.sh
fi
if echo "$QUERY_STRING" | egrep -q "addtask" && [ -n "$script" ]; then
#`sed -i 1i\ "$task" /etc/crontabs/root`
echo "$task" >> /etc/crontabs/root
/etc/init.d/cron start
/etc/init.d/cron restart
echo $rel
fi
case "$act" in
"remove" )
`sed -i "$string"d /etc/crontabs/root`
/etc/init.d/cron start
/etc/init.d/cron restart
echo $rel;;
"start" )
`sed -i "$string i$task" /etc/crontabs/root`
let string=string+1
`sed -i "$string"d /etc/crontabs/root`
/etc/init.d/cron start
/etc/init.d/cron restart
echo $rel;;
"stop" )
echo $act
echo $string
sed -i "$string s/^/#/" /etc/crontabs/root
/etc/init.d/cron start
/etc/init.d/cron restart
echo $rel;;
esac
echo "<h2>Планировщик заданий</h2><h3>Текущие задания</h3>"
tab="<table><tr style=background:#667;color:#fff;><td>Минуты</td><td>Часы</td><td>Дни</td><td>Месяцы</td><td>Дни недели</td><td>Путь к скрипту</td><td>Действие</td>"
echo "$tab<td>Удалить?</td></tr>"
i=1
crontab -l |
while read CMD; do
f=`echo "$CMD" | awk '{print$1}'`
if echo "$f" | egrep -q -v "#" ; then
act="stop"
ro="readonly"
cl="#CCCCFF"
dis="disabled"
s="Остановить"
else
f=`echo "$f" | sed 's/#//g'`
act="start"
cl="#CCCCCC"
ro=""
dis=""
s="Запустить"
fi
# | sed 's/[ ^t]*$//'
echo "<form action=$SCRIPT_NAME method=GET><tr bgcolor=$cl><input type=hidden name=string value=$i><td><input name=min type=text $dis size=3 value=$f></td><td><input name=hour type=text $dis size=3 value=`echo "$CMD" | awk '{print$2}'`></td><td><input name=day type=text $dis size=3 value=`echo "$CMD" | awk '{print$3}'`></td><td><input name=month type=text $dis size=3 value=`echo "$CMD" | awk '{print$4}'`></td><td><input name=week type=text $dis size=3 value=`echo "$CMD" | awk '{print$5}'`></td><td><input type=text id=script name=script $ro value=\"`echo "$CMD" | awk '{for(i=6;i<=NF;++i)print" "$i}' | sed 's/"/\&quot;/'`\"></td>
<td><button type=subbmit name=act value=$act onClick=\"javascript:return conf('$s', '$CMD') ? true : false;\" style=background-color:$cl>$act</button></td><td align=center><button type=subbmit $dis onClick=\"javascript:return conf('Удалить', '$CMD') ? true : false;\" name=act value=remove style=background-color:$cl>удалить</button></td></tr></form>"
let i=i+1
done
echo "</table><br><style>form.add input, select{width:65px;}
.add{width:65px;}</style>
<h3>Добавить новое задание</h3><form class=add><form action=$SCRIPT_NAME method=GET>$tab</tr>
<tr bgcolor=#FFFFCC><td><input id=min name=min type=text size=3 value=*><br><select onchange=\"document.getElementById('min').value =value ; \"><option value=*>-МИН-</option><option value=*>Каждую минуту</option><option value=*/2>Каждые 2 минуты</option><option value=*/5>Каждые 5 минут</option><option value=0,30>Каждые 30 минут</option><option value=0>:В начале каждого часа</option><option value="1">Каждый час</option><option value="5">Каждые 5 часов</option><option value="15">Каждые 1час 15мин.</option><option value="30">Каждые 1час 30мин.</option><option value="45">Каждый 1час 45мин.</option></select></td>
<td><input id=hour name=hour  type=text size=3 value=*><br><select onchange=\"document.getElementById('hour').value =value ; \"><option value=*>-ЧАС-</option><option value=*/1>Каждый час</option><option value=*/2>Каждые 2 часа</option><option value=0,12>Каждые 12 часов</option><option value=0>Каждые сутки в полночь</option><option value=1>Каждые сутки в 1:00</option><option value=2>Каждые сутки в 2:00</option><option value=12>Каждые сутки в 12:00</option><option value=13>Каждые сутки в 13:00</option>
</select></td><td><input id=day name=day type=text size=3 value=*><select onchange=\"document.getElementById('day').value =value ; \"><option value=*>-ДНИ-</option><option value=*/1>Каждый день</option><option value=*/2>Через день</option><option value=1,15>1-го и 15-го числа</option><option value=1>1-го числа</option><option value=2>2-го числа</option><option value=3>3-го числа</option><option value=30>30-го числа</option></select></td>
<td><input id=month name=month type=text size=3 value=*><select onchange=\"document.getElementById('month').value =value ; \"><option value=*>-МЕС-</option><option value=*>Каждый месяц</option><option value=*/2>Через месяц</option><option value=*/4>Через 3 месяца</option><option value=1,7>Через 6 месяцев</option><option value=1>Январь</option><option value=2>Февраль</option><option value=3>Март</option><option value=4>Апрель</option><option value=5>Май</option><option value=6>Июнь</option><option value=7>Июль</option><option value=8>Август</option><option value=9>Сентябрь</option><option value=10>Октябрь</option><option value=11>Ноябрь</option><option value=12>Декабрь</option></select></td>
<td><input id=week name=week type=text size=3 value=*><select onchange=\"document.getElementById('week').value =value ; \"><option value=*>-НЕД-</option><option value=*>Каждый день</option><option value=1-5>Пнд - Птн</option><option value=0,6>Сбт и Вскр</option><option value=1,3,5>Пнд, Срд, Птн</option><option value=2,4>Втр, Чтв</option><option value=0>Воскресенье</option><option value=1>Понедельник</option><option value=2>Вторник</option><option value=3>Среда</option><option value=4>Четверг</option><option value=5>Пятница</option><option value=6>Суббота</option></td>
<td align=center><input name=script id=sc type=text style=width:200px><br><- Подсказки</td>
<td><button type=subbmit name=addtask onClick=\"javascript:return conf('Добавить задание', document.getElementById('sc').value) ? true : false;\">Добавить задание</button></td></tr></table></form>
<script>function conf(a,b){ var z = ' (' + b + ')?'; var x = a + z; if(confirm(x)){return true}; }</script>"