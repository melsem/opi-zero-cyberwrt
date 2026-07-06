#!/bin/sh

echo "Content-type: text/html; charset=utf-8"
echo ""

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
CURRENT_DIR=$(dirname "$0")
. "$CURRENT_DIR/fade.sh"
# =====================================================================

# Отримання кроків (Playback)
get_current_steps() {
    if [ "$MODE" = "virtual" ]; then
        echo "25"
        return
    fi
    local steps
    steps=$(amixer -c $CARD_NUM get "$MIXER_CH" 2>/dev/null | grep -i "Front Left:" | awk -F'Playback ' '{print $2}' | awk '{print $1}')
    echo "${steps:-$MAX_STEPS}"
}

# Отримання відсотків
get_current_percent() {
    if [ "$MODE" = "virtual" ]; then
        echo "80"
        return
    fi
    local pct
    pct=$(amixer -c $CARD_NUM get "$MIXER_CH" 2>/dev/null | grep -i "Front Left:" | grep -o '\[[0-9]\+%\]' | head -n1 | tr -d '[]%')
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
	    [ "$SET_VOL" -gt $MAX_STEPS ] && SET_VOL=$MAX_STEPS
	    if [ "$MODE" != "virtual" ]; then
	        amixer -c $CARD_NUM -q set "$MIXER_CH" "$SET_VOL"
	    fi
	    rm -f "$LAST_VOL_FILE"
	    ;;
    esac
elif [ "$ACTION" = "up" ]; then
    current_steps=$((current_steps + 2))
    [ "$current_steps" -gt $MAX_STEPS ] && current_steps=$MAX_STEPS
    if [ "$MODE" != "virtual" ]; then
        amixer -c $CARD_NUM -q set "$MIXER_CH" "$current_steps"
    fi
    rm -f "$LAST_VOL_FILE"
elif [ "$ACTION" = "down" ]; then
    current_steps=$((current_steps - 2))
    [ "$current_steps" -lt 0 ] && current_steps=0
    if [ "$MODE" != "virtual" ]; then
        amixer -c $CARD_NUM -q set "$MIXER_CH" "$current_steps"
    fi
    rm -f "$LAST_VOL_FILE"
elif [ "$ACTION" = "mute" ]; then
    if [ "$MODE" = "usb" ]; then
        # Для USB перевіряємо стан перемикача [on]/[off]
        if amixer -c $CARD_NUM get "$MUTE_CH" 2>/dev/null | grep -q "\[on\]"; then
            amixer -c $CARD_NUM -q set "$MUTE_CH" mute
        else
            amixer -c $CARD_NUM -q set "$MUTE_CH" unmute
        fi
    elif [ "$MODE" = "internal" ]; then
        current_dac=$(amixer -c $CARD_NUM get "$MUTE_CH" 2>/dev/null | grep -m1 -o '\[[0-9]\+%\]' | tr -d '[]%')
        : "${current_dac:=85}"
        if [ "$current_dac" -gt 0 ]; then
            sound_off
        else
            sound_on
        fi
    fi
fi

# Фінальний звіт для веб-інтерфейсу
volsost_now=$(get_current_percent)

is_muted="on"
if [ "$MODE" = "usb" ]; then
    if amixer -c $CARD_NUM get "$MUTE_CH" 2>/dev/null | grep -q "\[off\]"; then
        is_muted="off"
    fi
elif [ "$MODE" = "internal" ]; then
    final_dac=$(amixer -c $CARD_NUM get "$MUTE_CH" 2>/dev/null | grep -m1 -o '\[[0-9]\+%\]' | tr -d '[]%')
    : "${final_dac:=85}"
    if [ "$final_dac" -eq 0 ]; then
        is_muted="off"
    fi
fi

echo "${volsost_now}%|${is_muted}"

