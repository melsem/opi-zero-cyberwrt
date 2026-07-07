#!/bin/sh

# Функція декодування URL (видаляє каретки %0D, міняє + на пробіли, декодує %XX)
unescape() {
    echo "$1" | sed 's/%0[Dd]//g; s/+/ /g; s/%/\\x/g' | xargs -0 printf "%b"
}

# 1. Обов'язкові HTTP-заголовки
echo "Content-type: text/html; charset=utf-8"
echo ""

# Константи (Блок BEGIN)
chrony_conf="/etc/chrony/chrony.conf"
sysntpd="/etc/init.d/sysntpd"
chronyd="/etc/init.d/chronyd"
gpsd="/etc/init.d/gpsd"

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
        # --- Перемикачі СЕРВІСІВ ЧАСУ ---
        staron_chronyd)
            "$sysntpd" stop
            "$sysntpd" disable
            "$chronyd" enable
            "$chronyd" start
            ;;
        staroff_chronyd)
            "$sysntpd" enable
            "$sysntpd" start
            "$chronyd" stop
            "$chronyd" disable
            ;;

        # --- Керування GPSD сервісом ---
        staron_gpsd)
            "$gpsd" enable
            "$gpsd" start
            ;;
        staroff_gpsd)
            "$gpsd" stop
            "$gpsd" disable
            ;;

        # --- Параметри CHRONY ---
        refclock_shm)
            refclock_shm="$val"
            ;;
        refclock_pps)
            refclock_pps="$val"
            ;;
        refclock_setsave)
            # Модифікуємо конфіг chrony.conf
            sed -i '/refclock SHM 0/d' "$chrony_conf"
            echo "$refclock_shm" >> "$chrony_conf"
            sed -i '/refclock PPS /d' "$chrony_conf"
            echo "$refclock_pps" >> "$chrony_conf"
            ;;

        # --- Параметри GPSD ---
        gpsd_enabled)
            gpsd_enabled="$val"
            ;;
        device)
            device="$val"
            ;;
        devPort)
            devPort="$val"
            ;;
        listen_globally)
            listen_globally="$val"
            ;;
        setsave)
            # Записуємо конфігурацію в UCI
            uci set gpsd.@gpsd[0].enabled="$gpsd_enabled"
            uci set gpsd.@gpsd[0].device="$device"
            uci set gpsd.@gpsd[0].port="$devPort"
            uci set gpsd.@gpsd[0].listen_globally="$listen_globally"
            # Якщо потрібно автоматично зберігати зміни в UCI:
            # uci commit gpsd
            ;;
    esac
    IFS='&'
done
IFS=$OLD_IFS

# 4. Блок END (Виклик індексної сторінки модуля)
/www/cgi-bin/modules/web-pps-gps/index.html

