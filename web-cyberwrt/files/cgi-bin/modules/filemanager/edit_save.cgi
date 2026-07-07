#!/bin/sh

# Copyright (C) 2013 cyberwrt.ru, carduino.ru, cyber-place.ru, mp3car.ru
# Ported to /bin/sh for uHTTPd / OpenWrt

# Функція декодування URL (заміна + на пробіли, видалення %0D (\r) та заміна %XX)
unescape() {
    # Видаляємо %0D (повернення каретки \r) та замінюємо + на пробіли
    # Після чого перетворюємо %XX на шістнадцяткові коди для printf
    echo "$1" | sed 's/%0[Dd]//g; s/+/ /g; s/%/\\x/g' | xargs -0 printf "%b"
}

# 1. Обов'язкові HTTP-заголовки
echo "Content-type: text/html; charset=utf-8"
echo ""

# Ініціалізація змінної шляху
path=""

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
    # Розділяємо пару на Ключ та Значення
    key="${pair%%=*}"
    val_raw="${pair#*=}"
    
    case "$key" in
        B3)
            # Отримуємо шлях до файлу
            path=$(unescape "$val_raw")
            ;;
        S1)
            # Якщо шлях уже відомий, записуємо вміст у файл
            if [ -n "$path" ]; then
                unescape "$val_raw" > "$path"
            fi
            ;;
    esac
    IFS='&'
done
IFS=$OLD_IFS

# 4. Блок END (вивід результату в браузер)
echo "Сохранено.<br>"
echo "<a href=\"javascript:window.open(window.location, '_self').close();\">Закрыть окно</a>"

