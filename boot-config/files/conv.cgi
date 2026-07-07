#!/bin/sh

# Функція декодування URL (заміна + на пробіли та %XX на символи)
unescape() {
    echo "$1" | sed 's/+/ /g; s/%/\\x/g' | xargs -0 printf "%b"
}

# Обов'язкові заголовки
echo "Content-type: text/html; charset=utf-8"
echo ""

# Ініціалізація змінних
rebut=0
max_freq=""
set_max_freq=""
corekeeper=""
set_corekeeper=""
mtd=""

# 1. Зчитування POST-даних з stdin
if [ "$REQUEST_METHOD" = "POST" ] && [ -n "$CONTENT_LENGTH" ] && [ "$CONTENT_LENGTH" -gt 0 ]; then
    POST_DATA=$(dd bs=1 count=$CONTENT_LENGTH 2>/dev/null)
else
    # Якщо викликано методом GET, беремо дані з QUERY_STRING
    POST_DATA="$QUERY_STRING"
fi

# 2. Парсинг рядка POST_DATA (розбиваємо по "&")
# Замінюємо амперсанди на пробіли для обходу в циклі
OLD_IFS=$IFS
IFS='&'

for pair in $POST_DATA; do
    IFS=$OLD_IFS
    # Виділяємо ключ і значення (заміна FS="=")
    key="${pair%%=*}"
    val_raw="${pair#*=}"
    
    # Декодуємо значення одразу за потребою
    value=$(unescape "$val_raw")

    case "$key" in
        backup)
            if [ "$value" = "backdtb" ]; then
                echo "<b>backup - OK</b><br>"
                dtc -I dtb -O dts -o /boot/dts.backup /boot/dtb
                dtc -I dts -O dtb -o /boot/dtb.backup /boot/dts.backup
            fi
            ;;
        resback)
            if [ "$value" = "resdtb" ]; then
                echo "<b>Restore u-boot from backup - OK</b><br>"
                rm -f /tmp/t-dts
                cp /boot/dtb.backup /boot/dtb
            fi
            ;;
        max_freq)
            set_max_freq="$value"
            case "$value" in
                "480MHz")  max_freq=480000 ;;
                "648MHz")  max_freq=648000 ;;
                "816MHz")  max_freq=816000 ;;
                "1008MHz") max_freq=1008000 ;;
            esac
            ;;
        astart)
            if [ "$value" = "noastar" ]; then
                /etc/init.d/cpufreq_scaling enable
            fi
            ;;
        set_cpu_freq)
            # Оскільки awk перевіряв поточний стан, порівнюємо з обробленим значенням
            # Примітка: Логіка awk залежала від порядку полів у формі. 
            # Переконайтеся, що поле "max_freq" у HTML йде РАНІШЕ ніж кнопка "set_cpu_freq"
            if [ "$max_freq" != "$value" ]; then
                uci set cpufreq_scaling.@cpufreq[0].max_freq="$max_freq"
                uci commit cpufreq_scaling
                echo "<b>Scaling max_freq: $set_max_freq changed.</b><br>"
                /etc/init.d/cpufreq_scaling start
            else
                echo "<b>max_freq unchanged.</b><br>"
            fi
            ;;
        corekeeper)
            set_corekeeper="$value"
            case "$value" in
                "1 cores") corekeeper=1; forcor="3 2 1" ;;
                "2 cores") corekeeper=2; forcor="3 2"; refrcor=1 ;;
                "3 cores") corekeeper=3; forcor=3; refrcor="2 1" ;;
                "4 cores") corekeeper=4; refrcor="3 2 1" ;;
            esac
            ;;
        set_core_keeper)
            if [ "$corekeeper" != "$value" ]; then
                uci set cpufreq_scaling.@cpufreq[0].corekeeper="$corekeeper"
                uci commit cpufreq_scaling
                # Розкоментуйте, якщо потрібно:
                # if [ "$corekeeper" -lt 4 ]; then
                #     for i in $forcor; do echo 0 > "/sys/devices/system/cpu/cpu${i}/online"; done
                # fi
                # if [ "$corekeeper" -gt 1 ]; then
                #     for i in $refrcor; do echo 1 > "/sys/devices/system/cpu/cpu${i}/online"; done
                # fi
                /etc/init.d/cpufreq_scaling start
                echo "<b>Processor cores: $corekeeper started.</b><br>"
            else
                echo "<b>Unchanged.</b><br>"
            fi
            ;;
        mtd-dtb)
            mtd="$value"
            ;;
        conv)
            if [ "$value" = "dtsmmc" ]; then
                dtc -I dtb -O dts -o /tmp/t-dts /boot/dtb
                echo "<b>Converted dtb to dts-file.</b><br>"
            elif [ "$value" = "dtsspi" ]; then
                dtc -I dtb -O dts -o /tmp/t-dts "/dev/$mtd"
                echo "Converted <b>dtb</b> - /dev/$mtd to dts-file.<br>"
            fi
            ;;
        reconv)
            rebut=1
            if [ "$value" = "dtbmmc" ]; then
                dtc -I dts -O dtb -o /boot/dtb /tmp/t-dts
                echo "<b>Converted dts-file to dtb.</b><br>"
            elif [ "$value" = "dtbspi" ]; then
                dtc -I dts -O dtb -o /tmp/dtb /tmp/t-dts
                mtd -e dtb write /tmp/dtb dtb
                echo "<b>Converted dts-file to dtb.</b><br>"
            fi
            ;;
    esac
    IFS='&'
done
IFS=$OLD_IFS

# Блок END
if [ "$rebut" = "1" ]; then
    sleep 1
    /www/cgi-bin/modules/boot-config/index.html "1"
else
    /www/cgi-bin/modules/boot-config/index.html "save_okay"
fi

