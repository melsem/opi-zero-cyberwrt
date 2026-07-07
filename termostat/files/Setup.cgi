#!/bin/sh

# Функція декодування URL (видаляє %0D, міняє + на пробіли, декодує %XX)
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
        # Секція uci налаштувань, де назва ключа збігається з опцією UCI
        search|enabled0|ftp_svr|pcf8574|init_port|srv_d|srv_port|d_ay|adres_i2c|sda_scl_i2c|allset|oInvert|pu_Skryp|imya_skript)
            # Відображення специфічних назв з форми на опції uci (якщо вони відрізняються)
            uci_opt="$key"
            [ "$key" = "interval" ] && uci_opt="times"
            [ "$key" = "init_port" ] && uci_opt="gpio1wr"
            [ "$key" = "srv_d" ] && uci_opt="serv_d"
            [ "$key" = "srv_port" ] && uci_opt="serv_port"
            [ "$key" = "adres_i2c" ] && uci_opt="hexadr"
            [ "$key" = "sda_scl_i2c" ] && uci_opt="sda_scl"
            
            uci set ds18b20.@Gpio1Wr[0]."$uci_opt"="$val"
            ;;

        # Керування автозапуском та службою
        staron)
            /etc/init.d/ds18b20_init enable
            echo "<br>AUTO-START ENABLE"
            ;;
        staroff)
            /etc/init.d/ds18b20_init disable
            echo "<br>AUTO-START DISABLE"
            ;;
        commit)
            uci commit ds18b20
            echo "<br>Setup Global."
            echo "<br><b>Save OK.</b>"
            ;;
        cozapusk)
            /etc/init.d/ds18b20_init restart
            echo "<br><b>START - OK.</b><br>"
            echo "<b>search sensor:<b><br> >>"
            sleep 4
            ;;
        coostanov)
            /etc/init.d/ds18b20_init stop
            echo "STOP - OK."
            ;;

        # Запис скрипта користувача з текстового поля формы
        text)
            # Записуємо очищений від кареток текст прямо у файл
            echo "$val" > /usr/sbin/skript_term
            chmod 755 /usr/sbin/skript_term
            echo "<br><b> Save Script OK</b>"
            ;;
    esac
    IFS='&'
done
IFS=$OLD_IFS

# 4. Блок END (вивід таблиці навігації)
echo "<br><br><br><table align=center border=0 size=18 width=>"
echo "<tr><td align=center bgcolor=#8891f1><b><a href=/cgi-bin/modules/termostatGpio/Sset.cgi>In settings</a></b></td></tr>"
echo "<tr><td></td></tr>"
echo "<tr><td align=center bgcolor=#90f1f1><b><a href=/cgi-bin/modules/termostatGpio/index.cgi>Exit settings</a></b></td></tr>"
echo "</table>"

