#!/bin/sh

LAST_VOL_FILE="/tmp/radio_last_vol"

# Функція плавного вимкнення звуку (Fade Out)
sound_off() {
    local current_dac
    current_dac=$(amixer -c 0 get 'DAC' 2>/dev/null | grep -m1 -o '\[[0-9]\+%\]' | tr -d '[]%')
    : "${current_dac:=59}"
    [ "$current_dac" -gt 59 ] && current_dac=59

    if [ "$current_dac" -gt 0 ]; then
	echo "$current_dac" > "$LAST_VOL_FILE"
	local vol_loop=$current_dac
	while [ "$vol_loop" -gt 0 ]; do
	    vol_loop=$((vol_loop - 1))
	    [ "$vol_loop" -lt 0 ] && vol_loop=0
	    amixer -c 0 -q set 'DAC' "${vol_loop}"
	    echo 2>/dev/null
	done
    fi
    amixer -c 0 -q set 'DAC' 0
}

# Функція плавного увімкнення звуку (Fade In)
sound_on() {
    local target_dac=60
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
	amixer -c 0 -q set 'DAC' "${vol_loop}"
	echo 2>/dev/null
    done
    amixer -c 0 -q set 'DAC' "${target_dac}"
    rm -f "$LAST_VOL_FILE"
}

case "$1" in
    sound_off|mute) sound_off;;
    sound_on|unmute) sound_on;;
esac
