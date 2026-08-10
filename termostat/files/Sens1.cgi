#!/bin/sh

# Функція декодування URL (видаляє каретки %0D, міняє + на пробіли, декодує %XX)
unescape() {
    echo "$1" | sed 's/%0[Dd]//g; s/+/ /g; s/%/\\x/g' | xargs -0 printf "%b"
}

# 1. Обов'язкові HTTP-заголовки
echo "Content-type: text/html; charset=utf-8"
echo ""

# 2. Зчитування POST або GET даних
if [ "$REQUEST_METHOD" = "POST" ] && [ -n "$CONTENT_LENGTH" ] && [ "$CONTENT_LENGTH" -gt 0 ]; then
    POST_DATA=$(dd bs=1 count=$CONTENT_LENGTH 2>/dev/null)
else
    POST_DATA="$QUERY_STRING"
fi

# 3. Парсинг потоку даних (еквівалент RS="&" та FS="=")
OLD_IFS=$IFS
IFS='&'

for pair in $POST_DATA; do
    IFS=$OLD_IFS
    key="${pair%%=*}"
    val_raw="${pair#*=}"
    
    # Декодуємо значення
    val=$(unescape "$val_raw")

    case "$key" in
        # Специфічні назви полів, які відрізняються від опцій в UCI
        id_dev)
            uci set ds18b20.@ds18b20_s1[0].id_sensor="$val"
            ;;
        hster)
            uci set ds18b20.@ds18b20_s1[0].hister="$val"
            ;;
        pwut)
            uci set ds18b20.@ds18b20_s1[0].pwout="$val"
            ;;
        flder)
            uci set ds18b20.@ds18b20_s1[0].folder="$val"
            ;;

        # Збереження та комміт для Narodmon
        narmon1)
            uci set ds18b20.@ds18b20_s1[0].enab_nar="$val"
            uci commit ds18b20
            echo "<br>narodmon - Sensor 1 Save OK."
            ;;

        # Загальний комміт для 1-го датчика
        commit1)
            uci commit ds18b20
            echo "<br>Sensor 1 Save OK."
            ;;

        # Універсальне правило для ВСІХ інших полів (градуси, часи по днях тижня)
        # Сюди потрапляють: logns, tipsensr, gra0dus, *_*_gra0dus, *_t1ime, *_m1ime, *_gradus тощо.
        *)
            if [ -n "$key" ]; then
                uci set ds18b20.@ds18b20_s1[0]."$key"="$val"
            fi
            ;;
    esac
    IFS='&'
done
IFS=$OLD_IFS

echo "<br><br><br>
<table align=center border=0 size=18 width=>
<tr><td align=center bgcolor=#8891f1><b><a href=/cgi-bin/modules/termostatGpio/Sset.cgi>In settings</a></b></td></tr>
<tr><td></td></tr><tr>
<td align=center bgcolor=#90f1f1><b><a href=/cgi-bin/modules/termostatGpio/index.cgi>Exit settings</a></b></td></tr>
</table>"
