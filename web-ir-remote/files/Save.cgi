#!/bin/sh

# Функція декодування URL (видаляє каретки %0D, міняє + на пробіли, декодує %XX)
unescape() {
    echo "$1" | sed 's/%0[Dd]//g; s/+/ /g; s/%/\\x/g' | xargs -0 printf "%b"
}

# 1. Обов'язкові HTTP-заголовки
echo "Content-type: text/html; charset=utf-8"
echo ""

# Ініціалізація змінних (Блок BEGIN)
rckeymaps="/etc/rc_keymaps/keyes"
triggerhappy="/etc/triggerhappy/triggers.d/example.conf"
sost=0

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
    
    val=$(unescape "$val_raw")

    case "$key" in
        nomerstroki)     nomerstroki="$val" ;;
        ircode_star)     ircode_star="$val" ;;
        keyname_star)    keyname_star="$val" ;;
        keysost_star)    keysost_star="$val" ;;
        command_star)    command_star="$val" ;;
        ircode)          ircode="$val" ;;
        keyname)         keyname="$val" ;;
        keysost)         keysost="$val" ;;
        command)         command="$val" ;;
        add_nev_ircode)  add_nev_ircode="$val" ;;
        add_nev_keyname) add_nev_keyname="$val" ;;
        add_nev_keysost) add_nev_keysost="$val" ;;
        add_nev_command) add_nev_command="$val" ;;
        exampleconf)     exampleconf="$val" ;;
        webirremoteinit) webirremoteinit="$val" ;;

        # --- ДІЯ: РЕДАГУВАННЯ ---
        edit)
            echo "<br>"
            ir-remote rckeymaps "$ircode_star" "$keyname_star" "$ircode" "$keyname"
            echo "<br>"
            ir-remote triggerhappy "$keyname_star" "$keysost_star" "$command_star" "$keyname" "$keysost" "$command"
            echo "<br>"
            /etc/init.d/web-ir-remote restart
            ;;

        # --- ДІЯ: ВИДАЛЕННЯ ---
        delete)
            ir-remote rckeymapsdelete "$ircode_star" "$keyname_star"
            ir-remote triggerhappydelete "$keyname_star" "$keysost_star" "$command_star" "$nomerstroki"
            echo "DELETE-OK<br>"
            /etc/init.d/web-ir-remote restart
            ;;

        # --- ДІЯ: ЗБЕРЕЖЕННЯ НОВОГО ---
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

        # --- КЕРУВАННЯ СЛУЖБОЮ ---
        staron)
            /etc/init.d/web-ir-remote enable
            ;;
        staroff)
            /etc/init.d/web-ir-remote disable
            ;;

        # --- ЗАПИС ТЕКСТУ КОНФІГУ ТРИГЕРІВ ---
        text)
            sost=1
            # Передаємо вміст змінної через потік, видаляючи порожні рядки (еквівалент sed '/^$/d')
            echo "$exampleconf" | sed '/^$/d' > /etc/triggerhappy/triggers.d/example.conf
            /etc/init.d/web-ir-remote restart
            ;;

        # --- ЗАПИС ТЕКСТУ ІНІЦІАЛІЗАЦІЇ (було закоментовано в awk) ---
        textremoteinit)
            # Якщо забажаєте розкоментувати логіку:
            # sost=1
            # echo "$webirremoteinit" | sed '/^$/d' > /etc/init.d/web-ir-remote
            # chmod 755 /etc/init.d/web-ir-remote
            ;;
    esac
    IFS='&'
done
IFS=$OLD_IFS

# 4. Блок END (Редирект залежно від статусу збереження конфігу)
if [ "$sost" -eq 0 ]; then
    /www/cgi-bin/modules/web-ir-remote/Setup.cgi setup_irc
elif [ "$sost" -eq 1 ]; then
    /www/cgi-bin/modules/web-ir-remote/Setuptryg.cgi setup_tryg
fi

