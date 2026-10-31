#!/bin/sh

echo "Content-type: text/html; charset=utf-8"
echo ""

playlist="/etc/config/web-radio2"
tfile="/tmp/play_name"
real_init="/etc/init.d/web-radio2.init"
LAST_VOL_FILE="/tmp/radio_last_vol"

# =====================================================================
# АВТОМАТИЧНЕ ВИЗНАЧЕННЯ КАРТИ ТА ЗАХИСТ ВІД ВІДСУТНОСТІ ЗВУКУ
# =====================================================================
if amixer -c 1 get "Speaker" >/dev/null 2>&1; then
    CARD_NUM=1
    MIXER_CH="Speaker"
    MUTE_CH="Speaker"
    MAX_STEPS=30
    MODE="usb"
elif amixer -c 0 get "Speaker" >/dev/null 2>&1; then
    CARD_NUM=0
    MIXER_CH="Speaker"
    MUTE_CH="Speaker"
    MAX_STEPS=30
    MODE="usb"
elif amixer -c 0 get "Line Out" >/dev/null 2>&1; then
    CARD_NUM=0
    MIXER_CH="Line Out"
    MUTE_CH="DAC"
    MAX_STEPS=31
    MODE="internal"
else
    CARD_NUM=0
    MIXER_CH="Virtual"
    MUTE_CH="Virtual"
    MAX_STEPS=31
    MODE="virtual"
fi
# =====================================================================

# =====================================================================
# ФІКСАЦІЯ: ДИНАМІЧНЕ ВИЗНАЧЕННЯ ШЛЯХУ ТА ПІДКЛЮЧЕННЯ БІБЛІОТЕКИ FADE.SH
# =====================================================================
#CURRENT_DIR=$(dirname "$0")
#. "$CURRENT_DIR/fade.sh"
# =====================================================================

# Функція плавного вимкнення звуку (Fade Out)
sound_off() {
    [ "$MODE" = "virtual" ] && return

    if [ "$MODE" = "usb" ]; then
        # USB-карта не підтримує плавний ЦАП-fade без затинань, тому робимо швидкий mute
        amixer -c $CARD_NUM -q set "$MUTE_CH" mute
        return
    fi

    local current_dac
    current_dac=$(amixer -c $CARD_NUM get "$MUTE_CH" 2>/dev/null | grep -m1 -o '\[[0-9]\+%\]' | tr -d '[]%')
    : "${current_dac:=59}"
    [ "$current_dac" -gt 59 ] && current_dac=59

    if [ "$current_dac" -gt 0 ]; then
        echo "$current_dac" > "$LAST_VOL_FILE"
        local vol_loop=$current_dac
        while [ "$vol_loop" -gt 0 ]; do
            vol_loop=$((vol_loop - 1))
            [ "$vol_loop" -lt 0 ] && vol_loop=0
            amixer -c $CARD_NUM -q set "$MUTE_CH" "${vol_loop}"
            echo 2>/dev/null
        done
    fi
    amixer -c $CARD_NUM -q set "$MUTE_CH" 0
}

# Функція плавного увімкнення звуку (Fade In)
sound_on() {
    [ "$MODE" = "virtual" ] && return

    if [ "$MODE" = "usb" ]; then
        # Повертаємо звук для USB-карти
        amixer -c $CARD_NUM -q set "$MUTE_CH" unmute
        return
    fi

    local target_dac=59
    if [ -f "$LAST_VOL_FILE" ]; then
        target_dac=$(cat "$LAST_VOL_FILE")
        case "$target_dac" in
            ''|*[!0-9]*) target_dac=59 ;;
        esac
    fi
    [ "$target_dac" -gt 59 ] && target_dac=59
    [ "$target_dac" -le 0 ] && target_dac=59

    local vol_loop=0
    while [ "$vol_loop" -lt "$target_dac" ]; do
        vol_loop=$((vol_loop + 1))
        [ "$vol_loop" -gt "$target_dac" ] && vol_loop=$target_dac
        amixer -c $CARD_NUM -q set "$MUTE_CH" "${vol_loop}"
        echo 2>/dev/null
    done
    amixer -c $CARD_NUM -q set "$MUTE_CH" "${target_dac}"
    rm -f "$LAST_VOL_FILE"
}

# Декодування URL (замінюємо %XX на символи через awk для надійності)
urldecode() {
    echo -e "$(echo "$1" | sed 's/+/ /g; s/%/\\x/g')"
}

if [ "$REQUEST_METHOD" = "POST" ] && [ "$CONTENT_LENGTH" -gt 0 ]; then
    read -n "$CONTENT_LENGTH" POST_DATA
fi

# Надійний і швидкий парсинг параметрів БЕЗ sed
get_var() {
    local data="$POST_DATA"
    [ -z "$data" ] && data="$QUERY_STRING"
    local var_name="$1"
    case "$data" in
        *"$var_name="*)
            local val="${data##*"$var_name="}"
            val="${val%%&*}"
            urldecode "$val"
            ;;
        *)
            echo ""
            ;;
    esac
}

# Перевірка статусу для веб-інтерфейсу
if [ "$(get_var "cmd")" = "get_status" ]; then
    if ! pidof madplay &>/dev/null && ! pidof curl &>/dev/null && ! pidof web-radio2 &>/dev/null; then
        echo "Off"
    else
        if [ -f "$tfile" ] && [ -s "$tfile" ]; then
            cat "$tfile"
        else
            echo "<b>Воспроизведение...</b>"
        fi
    fi
    exit 0
fi

URL=$(get_var "URL")
file=$(get_var "file")
folder=$(get_var "folder")
zapusk=$(get_var "zapusk")
ostanov=$(get_var "ostanov")
next=$(get_var "next")
delchan=$(get_var "delchan")
nomerstroki=$(get_var "nomerstroki")
add_nev_name=$(get_var "add_nev_name")
add_nev_url=$(get_var "add_nev_url")
text_playlist=$(get_var "text")

# 1. КНОПКА PLAY НАВПРОТИ КОНКРЕТНОЇ СТАНЦІЇ (НАЙВИЩИЙ ПРІОРИТЕТ)
if [ -n "$nomerstroki" ]; then
    sound_off
    killall web-radio2 madplay curl 2>/dev/null
    sleep 1
    killall -9 web-radio2 madplay curl 2>/dev/null
    
    st_name=$(get_var "nameURL")
    [ -z "$st_name" ] && st_name="Канал $nomerstroki"
    echo "<b>$st_name</b>" > "$tfile"
    
    web-radio2 -p "$nomerstroki" &
    sound_on
    echo "OK"

# 2. Пряме відтворення URL з поля введення
elif [ -n "$URL" ]; then
    sound_off
    killall web-radio2 madplay curl 2>/dev/null
    sleep 1
    killall -9 web-radio2 madplay curl 2>/dev/null
    echo "Интернет поток" > "$tfile"
    (curl -s "$URL" | madplay -) &
    sound_on
    echo "OK"

# 3. Відтворення конкретного файлу
elif [ -n "$file" ]; then
    sound_off
    killall web-radio2 madplay curl 2>/dev/null
    sleep 1
    killall -9 web-radio2 madplay curl 2>/dev/null
    echo "Файл: $(basename "$file")" > "$tfile"
    web-radio2 -f "$file" &
    sound_on
    echo "OK"

# 4. Відтворення папки
elif [ -n "$folder" ]; then
    sound_off
    killall web-radio2 madplay curl 2>/dev/null
    sleep 1
    killall -9 web-radio2 madplay curl 2>/dev/null
    echo "Папка: $(basename "$folder")" > "$tfile"
    web-radio2 -l "$folder"/*.mp3 &
    sound_on
    echo "OK"

# 5. Кнопка RADIO START
elif [ -n "$zapusk" ]; then
    sound_off
    killall web-radio2 madplay curl 2>/dev/null
    sleep 1
    killall -9 web-radio2 madplay curl 2>/dev/null
    web-radio2 -p 1 &
    sound_on
    echo "OK"

# 6. Кнопка RADIO STOP
elif [ -n "$ostanov" ]; then
    sound_off
    killall web-radio2 madplay curl 2>/dev/null
    sleep 1
    killall -9 web-radio2 madplay curl 2>/dev/null
    echo "Off" > "$tfile"
    sound_on
    echo "OK"

# 7. Кнопка Next channel
elif [ -n "$next" ]; then
    sound_off
    killall madplay curl 2>/dev/null 2>/dev/null
    sleep 1
    killall -9 madplay curl 2>/dev/null 2>/dev/null
    sound_on
    echo "OK"

# 8. Видалення каналу
elif [ -n "$delchan" ]; then
    sed -i "/#EXTINF:-$delchan[ ,]/ {N;d;}" "$playlist"
    if [ -f "$playlist" ]; then
        tmp_pl="/tmp/pl_arrange.tmp"
        echo "#EXTM3U" > "$tmp_pl"
        idx=1
        grep -E "^#EXTINF|^http" "$playlist" | grep -v "#EXTM3U" | tr -d '\r' | while read -r line1; do
            if echo "$line1" | grep -q "^#EXTINF"; then
                ch_name=$(echo "$line1" | cut -d, -f2- | tr -d '"' | tr -d "'")
                read -r line2
                [ -z "$line2" ] && continue
                echo -e "#EXTINF:-$idx,\"$ch_name\"" >> "$tmp_pl"
                echo "$line2" >> "$tmp_pl"
                idx=$((idx + 1))
            fi
        done
        mv "$tmp_pl" "$playlist"
    fi
    echo "OK"

# 9. Додавання нової станції
elif [ -n "$add_nev_name" ] && [ -n "$add_nev_url" ]; then
    if grep -q 'EXTINF:-' "$playlist" 2>/dev/null; then
        last_idx=$(grep -o 'EXTINF:-[0-9]\+' "$playlist" | tail -n1 | cut -d- -f2)
    else
        last_idx=0
    fi
    : "${last_idx:=0}"
    n_str=$((last_idx + 1))
    
    [ -n "$(tail -c1 "$playlist" 2>/dev/null)" ] && echo "" >> "$playlist"
    echo -e "#EXTINF:-$n_str,\"$add_nev_name\"" >> "$playlist"
    echo "$add_nev_url" >> "$playlist"
    echo "OK"

# 10. Керування автостартом
elif echo "&$POST_DATA&$QUERY_STRING" | grep -q "autostarton="; then
    /etc/init.d/web-radio2.init enable
    echo "OK"

elif echo "&$POST_DATA&$QUERY_STRING" | grep -q "autostartoff="; then
    /etc/init.d/web-radio2.init disable
    echo "OK"

# 11. Синхронізація текстового поля плейлиста
elif [ -n "$text_playlist" ]; then
    echo "$text_playlist" | tr -d '\r' > "$playlist"
    if [ -f "$playlist" ]; then
        tmp_pl="/tmp/pl_arrange.tmp"
        echo "#EXTM3U" > "$tmp_pl"
        idx=1
        grep -E "^#EXTINF|^http" "$playlist" | grep -v "#EXTM3U" | while read -r line1; do
            if echo "$line1" | grep -q "^#EXTINF"; then
                ch_name=$(echo "$line1" | cut -d, -f2- | tr -d '"' | tr -d "'")
                read -r line2
                [ -z "$line2" ] && continue
                echo -e "#EXTINF:-$idx,\"$ch_name\"" >> "$tmp_pl"
                echo "$line2" >> "$tmp_pl"
                idx=$((idx + 1))
            fi
        done
        mv "$tmp_pl" "$playlist"
    fi
    echo "OK"
fi

