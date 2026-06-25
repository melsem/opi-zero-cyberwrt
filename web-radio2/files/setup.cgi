#!/bin/sh
#Copyright (C) 2013 cyberwrt.ru, carduino.ru, cyber-place.ru, mp3car.ru

# Обязательный заголовок для CGI скрипта (чтобы веб-сервер не выдавал ошибку 500)
echo "Content-type: text/html; charset=utf-8"
echo ""

# Подгружаем меню веб-интерфейса
if [ -f /www/menu.html ]; then
    cat /www/menu.html
fi

# Исправлено: кавычки внутри JavaScript теперь экранированы (\")
echo "<script type=\"text/javascript\">
function startTime(){ 
    var tm=new Date(); 
    var h=tm.getHours(); 
    var m=tm.getMinutes(); 
    var s=tm.getSeconds(); 
    m=checkTime(m); 
    s=checkTime(s);
    var txtEl = document.getElementById('txt');
    if (txtEl) txtEl.innerHTML=h+\":\"+m+\":\"+s; 
    setTimeout('startTime()',500); 
}
function checkTime(i){ if (i<10) { i=\"0\" + i;} return i; } 
</script>"

echo "<title>WEB-RADIO-2</title>"

playlist=/etc/config/web-radio2
tfile=/tmp/play_name
bitwebradio2=0

# Исправлено: проверка автозапуска и замена оператора '==' на '-eq'
if [ ! -f /etc/rc.d/S99web-radio2 ]; then
    ontxtoff2="Желаете_включить_авто-запуск_???"
    ontxtoff="Желаете_отключить_авто-запуск_???"
    onoff="autostarton"
    cto="OFF"
    clor="ff0000"
    dsd=""
    dsd2="disabled"
    star_tup=1
else
    star_tup=0
    dsd="disabled"
    dsd2=""
    clor="0000ff"
    ontxtoff2="Желаете_отключить_авто-запуск_???"
    ontxtoff="Желаете_включить_авто-запуск_???"
    onoff="autostartoff"
    cto="ON"
fi

# Исправлено: экранирование кавычек в CSS стилях и HTML свойствах
echo "
<body bgcolor=\"#f0f0f0\" onload=\"startTime()\">
<style>
  .b0 { cursor: pointer; }
  .b1 { font-size: 15pt; text-align: center; padding: 5px 30px; margin-left: 1px; cursor: pointer; border-radius:inherit; }
  .b2 { font-size: 12pt; text-align: left; padding: 2px 10px; margin-left: 5px; cursor: pointer; border-radius:inherit; }
  .b3 { font-size: 12pt; text-align: center; padding: 2px 50px; margin-left: 5px; cursor: pointer; border-radius:inherit; }
  textarea { resize: none; } 
</style>

<script>
function deleteschen(f) { if (confirm(\"Are you sure... Delete? \")) f.submit(); }
</script>"

# Вывод таймера перенаправления при действиях с плейлистом
if [ "$1" = "text" ] || [ "$1" = "next" ] || [ "$1" = "add_nev_url" ] || [ "$1" = "delchan" ]; then
    if [ "$1" = "text" ] || [ "$1" = "next" ] || [ "$1" = "add_nev_url" ]; then
        hidden="hidden"; i="2"; time="1000"
    elif [ "$1" = "delchan" ]; then
        hidden=""; i="10"; time="1000"
    fi
    echo "<table $hidden align=\"center\" border=\"0\"><tr><td>The channel link has been removed.</br>The list sorting will be completed in:
    <b><span id=\"time\"></span></b> seconds.
    <script type=\"text/javascript\">var i = $i; function time(){ var tEl = document.getElementById(\"time\"); if(tEl) tEl.innerHTML = i; i--; if (i < 0) location.href = \"/cgi-bin/modules/web-radio2/index.cgi\"; }
    time(); setInterval(time, $time); </script>
    </td></tr></table>"
fi

# Блок вывода текущего статуса воспроизведения
echo "<table><tr><td>
<table><tr><td>
<div class=\"header\"><span id=\"txt\"></span> "

if ! pidof web-radio2 > /dev/null 2>&1; then
    bitwebradio2=0
    if ! pidof madplay > /dev/null 2>&1; then
        echo "Off"
    else
        echo "URL: <b>"
        [ -f "$tfile" ] && cat "$tfile"
        echo "</b>"
    fi
else
    bitwebradio2=1
    if ! pidof curl > /dev/null 2>&1; then
        echo "URL: <b>"
        [ -f "$tfile" ] && cat "$tfile"
        echo "</b>"
    else
        echo "Radio: <b>"
        [ -f "$tfile" ] && cat "$tfile"
        echo "</b>"
    fi
fi
echo "</td></tr></table>
</div>"

# Формы ввода для URL, файла и папки
echo "
<table width=\"100%\" border=\"0\">
<form method=\"POST\" action=\"save.cgi\"><tr><td valign=\"middle\">
<input class=\"b0\" size=\"70\" name=\"URL\" required placeholder=\"http://chanson.hostingradio.ru:8041/chanson-uncensored128.mp3\" title=\"URL-адрес аудио потока\"></td><td>
<input class=\"b0\" type=\"submit\" value=\"Play-URL\">
</td></tr></form>
<form method=\"POST\" action=\"save.cgi\"><tr><td valign=\"middle\">
<input class=\"b0\" size=\"70\" name=\"file\" required placeholder=\"/home/Шансон/044.Очаровала.mp3\" title=\"Полный путь к аудио файлу\"></td><td>
<input class=\"b0\" type=\"submit\" value=\" Play-file \">
</td></tr></form>
<form method=\"POST\" action=\"save.cgi\"><tr><td valign=\"middle\">
<input class=\"b0\" size=\"70\" name=\"folder\" required placeholder=\"/home/Шансон\" title=\"Имя папки с аудио файлами\"></td><td>
<input class=\"b0\" type=\"submit\" value=\"Play-folder\">
</td></tr></form>
</table>"

# Расчет строк плейлиста
c=0
if [ -f "$playlist" ]; then
    c=$(sed -n '$=' "$playlist")
fi
: "${c:=0}"
let "b = (c / 2)"
a=1

# Блок кнопок управления Старт/Стоп/Вперед
echo "
<table align=\"left\"><tr><td></td><td></td><td><form method=\"POST\" action=\"save.cgi\">"

if ! pidof web-radio2 > /dev/null 2>&1; then
    if ! pidof madplay > /dev/null 2>&1; then
        echo "<button class=\"b1\" name=\"zapusk\" type=\"submit\" title=\"Желаете запустить модуль ???\"><img src=\"/modules/web-radio2/b.jpg\" alt=\"\" style=\"vertical-align: middle\"> RADIO START</button>"
    else
        echo "<button class=\"b1\" name=\"ostanov\" type=\"submit\" title=\"Желаете остановить модуль ???\"><img src=\"/modules/web-radio2/r.jpg\" alt=\"\" style=\"vertical-align: middle\"> madplay STOP</button>"
    fi
else
    if ! pidof curl > /dev/null 2>&1; then
        echo "<button class=\"b1\" name=\"ostanov\" type=\"submit\" title=\"Желаете остановить модуль ???\"><img src=\"/modules/web-radio2/r.jpg\" alt=\"\" style=\"vertical-align: middle\"> madplay STOP</button>"
    else
        echo "<button class=\"b1\" name=\"ostanov\" type=\"submit\" title=\"Желаете остановить модуль ???\"><img src=\"/modules/web-radio2/r.jpg\" alt=\"\" style=\"vertical-align: middle\"> RADIO STOP</button>"
    fi
fi

echo "</form></td>
<td><form method=\"POST\" action=\"save.cgi\">
<button name=\"next\" type=\"submit\" title=\"Следующий канал\" class=\"b1\"> Next channel</button>
</form></td></tr></table><br>"


while [ "$a" -le "$b" ]; do
    # Точный поиск по шаблону #EXTINF:-номер, с очисткой кавычек из имени
    name=$(awk -v idx="$a" 'BEGIN {FS=","} $1 ~ "#EXTINF:-"idx"$" || $1 ~ "#EXTINF:-"idx {gsub(/"/, "", $2); print $2; exit}' "$playlist")
    url=$(awk -v idx="$a" '$0 ~ "#EXTINF:-"idx"(,|$)" {getline; print; exit}' "$playlist")

    # Если имя не нашлось, используем заглушку
    if [ -z "$name" ]; then
        name="Channel $a"
    fi

    # Безопасное экранирование кавычек для HTML-атрибутов
    echo "
    <table width=\"100%\"><tr>
    
    <form method=\"POST\" action=\"save.cgi\" onsubmit=\"deleteschen(this); return false;\">
    <td valign=\"middle\">
    <input type=\"checkbox\" name=\"delchan\" checked=\"checked\" hidden value=\"$a\">
    <button name=\"confirm\" type=\"submit\" value=\"$a\" title=\"Delete $name\"><img src=\"/modules/web-radio2/del.png\" alt=\"Delete\"></button>
    </td>
    </form>
    
    <form method=\"POST\" action=\"save.cgi\">
    <td valign=\"middle\"><input name=\"nomerstroki\" hidden value=\"$a\">$a.</td>
    <td valign=\"middle\">
    <input class=\"b2\" name=\"nameURL\" size=\"50\" readonly value=\"$name\" required title=\"$url\" placeholder=\"список_пуст\">
    <input class=\"b2\" name=\"URL2\" readonly value=\"$url\" required hidden>
    </td><td valign=\"middle\">
    <input class=\"b2\" type=\"submit\" value=\"play\">
    </td></form></tr></table>"

    let "a += 1"
done

#===================================================================================
#===================================================================================
c=$(sed -n '$=' "$playlist")
: "${c:=0}"
let "b = (c / 2)"
let "b += 1"

# Исправлено: корректный вывод полей формы добавления нового канала
echo "
<table>
<form method=\"POST\" action=\"save.cgi\">
<tr><td align=\"left\"><p>$b.
<input size=\"2\" hidden name=\"n_str\" value=\"$b\" required placeholder=\"add_nev_name\">
<input class=\"b2\" size=\"10\" name=\"add_nev_name\" required placeholder=\"add_name\">
<input class=\"b2\" size=\"35\" name=\"add_nev_url\" required placeholder=\"add_url\">
<input class=\"b2\" type=\"submit\" value=\"Save\"></p></td></tr>
</form>
</table>"

#===================================================================================
#===================================================================================
echo "
<table><tr><td>.</td><td><p style=\"color:#$clor\">Auto Startup - <b>$cto</b></p></td><td>
<form method=\"POST\" action=\"save.cgi\">"

if [ "$star_tup" -eq 1 ]; then
    echo "<button $dsd name=\"$onoff\" type=\"submit\" title=\"$ontxtoff\" class=\"b1\"><img src=\"/modules/web-radio2/b.jpg\" alt=\"\" style=\"vertical-align: middle\"> Enable</button>"
else
    echo "<button $dsd2 name=\"$onoff\" type=\"submit\" title=\"$ontxtoff2\" class=\"b1\"><img src=\"/modules/web-radio2/r.jpg\" alt=\"\" style=\"vertical-align: middle\"> Disable</button>"
fi

echo "
</form></td></tr></table><br><br>"

#____________________________________________________________
#____________________________________________________________
kollw_str=$(sed -n '$=' "$playlist")
: "${kollw_str:=10}"

echo "
<table><tr><td>
<form method=\"POST\" action=\"save.cgi\">
<p><textarea rows=\"$kollw_str\" cols=\"64\" name=\"text\" spellcheck=\"false\" placeholder=\"Play-list?\">"

if [ -f "$playlist" ]; then
    cat "$playlist"
fi

echo "</textarea></p>
<p><input type=\"submit\" class=\"b1\" value=\"Save-Play-list\"></p>
</form>
</td></tr></table>
</td></tr></table>
</body>"

