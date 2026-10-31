#!/bin/sh

# Функция декодирования URL (удаляет %0D, меняет + на пробелы, декодирует %XX)
# Дополнительно очищает от UTF-8 BOM и Windows-переносов \r
unescape() {
    echo "$1" | sed 's/%0[Dd]//g; s/+/ /g; s/%/\\x/g' | xargs -0 printf "%b" 2>/dev/null | sed '1s/^\xEF\xBB\xBF//' | tr -d '\r'
}

# 1. Обязательные HTTP-заголовки
echo "Content-type: text/html; charset=utf-8"
echo ""

# Конфиги
rckeymaps="/etc/rc_keymaps/keyes"
triggerhappy="/etc/triggerhappy/triggers.d/example.conf"
sost=0

# Инициализация переменных (чтобы не было мусора)
nomerstroki=""
ircode_star=""
keyname_star=""
keysost_star=""
command_star=""
ircode=""
keyname=""
keysost=""
command=""
add_nev_ircode=""
add_nev_keyname=""
add_nev_keysost=""
add_nev_command=""
exampleconf=""
webirremoteinit=""

# 2. Чтение POST или GET данных во временный буфер в RAM (защита от падения длинных строк)
TMP_RAW="/tmp/ir_raw.tmp"
rm -f "$TMP_RAW"

if [ "$REQUEST_METHOD" = "POST" ] && [ -n "$CONTENT_LENGTH" ] && [ "$CONTENT_LENGTH" -gt 0 ]; then
    dd bs=1 count=$CONTENT_LENGTH 2>/dev/null > "$TMP_RAW"
else
    echo -n "$QUERY_STRING" > "$TMP_RAW"
fi

# Если данных нет вообще - выходим
if [ ! -s "$TMP_RAW" ]; then
    rm -f "$TMP_RAW"
    exit 0
fi

# Считываем POST_DATA для цикла
POST_DATA=$(cat "$TMP_RAW")
rm -f "$TMP_RAW"

# 3. Парсинг потока данных (эквивалент RS="&" и FS="=")
OLD_IFS=$IFS
IFS='&'

for pair in $POST_DATA; do
    IFS=$OLD_IFS
    key="${pair%%=*}"
    val_raw="${pair#*=}"
    
    # Декодируем значение на лету
    val=$(unescape "$val_raw")

    case "$key" in
        nomerstroki)    nomerstroki="$val" ;;
        ircode_star)    ircode_star="$val" ;;
        keyname_star)   keyname_star="$val" ;;
        keysost_star)   keysost_star="$val" ;;
        command_star)   command_star="$val" ;;
        
        ircode)         ircode="$val" ;;
        keyname)        keyname="$val" ;;
        keysost)        keysost="$val" ;;
        command)        command="$val" ;;

        # --- ОБРАБОТКА ДЕЙСТВИЙ (Эквивалент условий в AWK) ---
        edit)
            echo "<br>"
            ir-remote rckeymaps "$ircode_star" "$keyname_star" "$ircode" "$keyname"
            echo "<br>"
            ir-remote triggerhappy "$keyname_star" "$keysost_star" "$command_star" "$keyname" "$keysost" "$command"
            echo "<br>"
            /etc/init.d/web-ir-remote restart
            ;;
        delete)
            ir-remote rckeymapsdelete "$ircode_star" "$keyname_star"
            ir-remote triggerhappydelete "$keyname_star" "$keysost_star" "$command_star" "$nomerstroki"
            echo "DELETE-OK<br>"
            /etc/init.d/web-ir-remote restart
            ;;
        add_nev_ircode)  add_nev_ircode="$val" ;;
        add_nev_keyname) add_nev_keyname="$val" ;;
        add_nev_keysost) add_nev_keysost="$val" ;;
        add_nev_command) add_nev_command="$val" ;;
        Save_nev)
            if [ "$add_nev_ircode" = "keyboard" ]; then
                ir-remote triggerhappy_add "$keyname_star" "$keysost_star" "$command_star" "$add_nev_keyname" "$add_nev_keysost" "$add_nev_command"
                echo "<br>"
                /etc/init.d/web-ir-remote restart
            else
                ir-remote rckeymaps_add "$add_nev_ircode" "$add_nev_keyname"
                echo "<br>"
                ir-remote triggerhappy_add "$keyname_star" "$keysost_star" "$command_star" "$add_nev_keyname" "$add_nev_keysost" "$add_nev_command"
                echo "<br>"
                /etc/init.d/web-ir-remote restart
            fi
            ;;
        staron)
            /etc/init.d/web-ir-remote enable
            ;;
        staroff)
            /etc/init.d/web-ir-remote disable
            ;;
        exampleconf)     exampleconf="$val" ;;
        text)
            sost=1
            # Запись конфигурации напрямую, убирая пустые строки и \r
            echo "$exampleconf" | sed '/^$/d' > /etc/triggerhappy/triggers.d/example.conf
            # echo "<b> Save example.conf OK</b><br>"
            /etc/init.d/web-ir-remote restart
            ;;
        webirremoteinit) webirremoteinit="$val" ;;
        textremoteinit)
            # Раскомментируйте, если логика инциализации инициализации будет нужна:
            # sost=1
            # echo "$webirremoteinit" | sed '/^$/d' > /etc/init.d/web-ir-remote
            # echo "<b> Save web-ir-remote-init OK</b><br>"
            ;;
    esac
    IFS='&'
done
IFS=$OLD_IFS

# 4. Блок END (эквивалент секции END в AWK)
sleep 1
if [ "$sost" -eq 0 ]; then
    /www/cgi-bin/modules/web-ir-remote/Setup.cgi setup_irc
elif [ "$sost" -eq 1 ]; then
    /www/cgi-bin/modules/web-ir-remote/Setuptryg.cgi setup_tryg
fi

