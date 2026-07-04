#!/bin/sh

echo "Content-type: text/html; charset=utf-8"
echo ""

MIXER_CH="Line Out"
LAST_VOL_FILE="/tmp/radio_last_vol"
tda_volume="/tmp/tda_volume"

# =====================================================================
# ФІКСАЦІЯ: ДИНАМІЧНЕ ВИЗНАЧЕННЯ ШЛЯХУ ТА ПІДКЛЮЧЕННЯ БІБЛІОТЕКИ FADE.SH
# =====================================================================
CURRENT_DIR=$(dirname "$0")
. "$CURRENT_DIR/fade.sh"
# =====================================================================

# Отримання кроків (Playback)
get_current_steps() {
    local steps
    steps=$(amixer -c 0 get "$MIXER_CH" 2>/dev/null | grep -i "Front Left:" | awk -F'Playback ' '{print $2}' | awk '{print $1}')
    echo "${steps:-31}"
}

# Отримання відсотків
get_current_percent() {
    local pct
    pct=$(amixer -c 0 get "$MIXER_CH" 2>/dev/null | grep -i "Front Left:" | grep -o '\[[0-9]\+%\]' | head -n1 | tr -d '[]%')
    echo $pct > $tda_volume
    echo "${pct:-100}"
}

# Зчитуємо поточні кроки
current_steps=$(get_current_steps)

# БЕЗПЕЧНИЙ ПАРСИНГ ПАРАМЕТРІВ ДЛЯ УРІЗАНОГО BUSYBOX (ЗАМІСТЬ SED)
ACTION=""
SET_VOL=""
case "$QUERY_STRING" in
    *act=*)
	ACTION="${QUERY_STRING##*act=}"
	ACTION="${ACTION%%&*}"
	;;
esac
case "$QUERY_STRING" in
    *set=*)
	SET_VOL="${QUERY_STRING##*set=}"
	SET_VOL="${SET_VOL%%&*}"
	;;
esac

# Логіка обробки дій гучності
if [ -n "$SET_VOL" ]; then
    case "$SET_VOL" in
	''|*[!0-9]*) ;;
	*)
	    [ "$SET_VOL" -gt 31 ] && SET_VOL=31
	    amixer -c 0 -q set "$MIXER_CH" "$SET_VOL"
	    rm -f "$LAST_VOL_FILE"
	    ;;
    esac
elif [ "$ACTION" = "up" ]; then
    current_steps=$((current_steps + 2))
    [ "$current_steps" -gt 31 ] && current_steps=31
    amixer -c 0 -q set "$MIXER_CH" "$current_steps"
    rm -f "$LAST_VOL_FILE"
elif [ "$ACTION" = "down" ]; then
    current_steps=$((current_steps - 2))
    [ "$current_steps" -lt 0 ] && current_steps=0
    amixer -c 0 -q set "$MIXER_CH" "$current_steps"
    rm -f "$LAST_VOL_FILE"
elif [ "$ACTION" = "mute" ]; then
    current_dac=$(amixer -c 0 get 'DAC' 2>/dev/null | grep -m1 -o '\[[0-9]\+%\]' | tr -d '[]%')
    : "${current_dac:=85}"

    if [ "$current_dac" -gt 0 ]; then
	sound_off
    else
	sound_on
    fi
fi

# Фінальний звіт для веб-інтерфейсу
volsost_now=$(get_current_percent)
final_dac=$(amixer -c 0 get 'DAC' 2>/dev/null | grep -m1 -o '\[[0-9]\+%\]' | tr -d '[]%')
: "${final_dac:=85}"

is_muted="on"
if [ "$final_dac" -eq 0 ]; then
    is_muted="off"
fi

echo "${volsost_now}%|${is_muted}"
