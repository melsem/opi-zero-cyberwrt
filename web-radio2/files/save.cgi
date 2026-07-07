#!/bin/sh

# Функція декодування URL (видаляє каретки %0D, міняє + на пробіли, декодує %XX)
unescape() {
    echo "$1" | sed 's/%0[Dd]//g; s/+/ /g; s/%/\\x/g' | xargs -0 printf "%b"
}

# 1. Обов'язкові HTTP-заголовки
echo "Content-type: text/html; charset=utf-8"
echo ""

# Константи
playlist="/etc/config/web-radio2"

# Змінні для відстеження дій в блоці END
delchan=""
add_nev_url=""
action_type=""

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
        # --- ВИДАЛЕННЯ КАНАЛУ ---
        delchan)
            delchan="delchan"
            action_type="delchan"
            delstroki="$val"
            /www/cgi-bin/modules/web-radio2/sed.cgi delet "$delstroki"
            ;;
        
        # --- ВІДТВОРЕННЯ З ПЛЕЙЛИСТА (ПОРЯДКОВИЙ НОМЕР + НАЗВА + URL) ---
        nomerstroki)
            nomerstroki="$val"
            ;;
        nameURL)
            nameUrlStroki="$val"
            ;;
        URL2)
            amixer -c 0 -q set 'DAC' 100%-
            sleep 1
            killall -9 web-radio2 2>/dev/null
            web-radio2 -u "$val" -n "$nameUrlStroki" -p "$nomerstroki" &
            amixer -c 0 -q set 'DAC' 100%
            ;;

        # --- КЕРУВАННЯ ВІДТВОРЕННЯМ ТА СЛУЖБОЮ ---
        next)
            action_type="next"
            amixer -c 0 -q set 'DAC' 100%-
            sleep 1
            killall -9 madplay 2>/dev/null
            killall -9 curl 2>/dev/null
            amixer -c 0 -q set 'DAC' 100%
            ;;
        zapusk)
            /etc/init.d/web-radio2 start
            ;;
        ostanov)
            /etc/init.d/web-radio2 stop
            ;;
        autostarton)
            /etc/init.d/web-radio2 enable
            ;;
        autostartoff)
            /etc/init.d/web-radio2 disable
            ;;

        # --- ВІДТВОРЕННЯ ОДНОГО URL, ФАЙЛУ АБО ПАПКИ ---
        URL)
            amixer -c 0 -q set 'DAC' 100%-
            sleep 1
            killall -9 web-radio2 2>/dev/null
            web-radio2 -u "$val" &
            amixer -c 0 -q set 'DAC' 100%
            ;;
        file)
            amixer -c 0 -q set 'DAC' 100%-
            sleep 1
            killall -9 web-radio2 2>/dev/null
            web-radio2 -f "$val" &
            amixer -c 0 -q set 'DAC' 100%
            ;;
        folder)
            amixer -c 0 -q set 'DAC' 100%-
            sleep 1
            killall -9 web-radio2 2>/dev/null
            web-radio2 -l "$val" &
            amixer -c 0 -q set 'DAC' 100%
            ;;

        # --- РЕДАГУВАННЯ ПЛЕЙЛИСТА ЦІЛКОМ ---
        text)
            action_type="text"
            # Очищаємо від порожніх рядків та перезаписуємо файл
            echo "$val" | sed '/^$/d' > "$playlist"
            ;;

        # --- ДОДАВАННЯ НОВОГО КАНАЛУ ---
        n_str)
            n="$val"
            ;;
        add_nev_name)
            nb="$val"
            /www/cgi-bin/modules/web-radio2/sed.cgi addnev "$n" "$nb"
            ;;
        add_nev_url)
            add_nev_url="add_nev_url"
            action_type="add_nev_url"
            echo "$val" | sed '/^$/d' >> "$playlist"
            ;;
    esac
    IFS='&'
done
IFS=$OLD_IFS

# 4. Блок END (Вивід результату та фінальний редирект)
case "$action_type" in
    text)
        echo "<b>Play list saved. OK</b><br>"
        /www/cgi-bin/modules/web-radio2/setup.cgi text
        ;;
    add_nev_url)
        echo "<b>NEW URL saved. OK</b><br>"
        /www/cgi-bin/modules/web-radio2/setup.cgi add_nev_url
        ;;
    delchan)
        echo "<b>Delete. OK</b><br>"
        /www/cgi-bin/modules/web-radio2/setup.cgi delchan
        ;;
    next)
        /www/cgi-bin/modules/web-radio2/setup.cgi next
        ;;
    *)
        /www/cgi-bin/modules/web-radio2/setup.cgi text
        ;;
esac

