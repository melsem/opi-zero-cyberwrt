#!/bin/sh

# Обязательные HTTP-заголовки
echo "Content-type: text/html; charset=utf-8"
echo ""

TMP_RAW="/tmp/edit_raw.tmp"
rm -f "$TMP_RAW"

# 1. Быстрое чтение бинарного потока во временный файл
if [ "$REQUEST_METHOD" = "POST" ] && [ -n "$CONTENT_LENGTH" ] && [ "$CONTENT_LENGTH" -gt 0 ]; then
    dd bs=1 count=$CONTENT_LENGTH 2>/dev/null > "$TMP_RAW"
else
    echo -n "$QUERY_STRING" > "$TMP_RAW"
fi

if [ ! -s "$TMP_RAW" ]; then
    echo "<span style='color:red;'>Ошибка: Данные не дошли до сервера. Файл не изменен.</span><br>"
    rm -f "$TMP_RAW"
    exit 0
fi

# 2. Быстрое извлечение пути к файлу
path_encoded=$(sed -n 's/.*B3=\([^&]*\).*/\1/p' "$TMP_RAW")
path=$(echo "$path_encoded" | sed 's/%0[Dd]//g; s/+/ /g; s/%/\\x/g' | xargs -0 printf "%b" 2>/dev/null)

if [ -z "$path" ]; then
    echo "<span style='color:red;'>Ошибка: Путь к файлу не найден.</span><br>"
    rm -f "$TMP_RAW"
    exit 0
fi

# 3. СВЕРХБЫСТРОЕ ДЕКОДИРОВАНИЕ С АВТОМАТИЧЕСКИМ УДАЛЕНИЕМ BOM И ВОЗВРАТОВ КАРЕТКИ (\r)
sed -n 's/.*S1=\([^&]*\).*/\1/p' "$TMP_RAW" | awk '
BEGIN {
    for (i = 0; i <= 255; i++) {
        char = sprintf("%c", i)
        hex = sprintf("%02X", i)
        hextbl[hex] = char
        hextbl[tolower(hex)] = char
    }
}
{
    gsub(/\+/, " ")
    gsub(/%0[Dd]/, "")
    
    split($0, parts, "%")
    printf "%s", parts
    
    for (j = 2; j <= length(parts); j++) {
        h = substr(parts[j], 1, 2)
        rest = substr(parts[j], 3)
        if (h in hextbl) {
            printf "%s%s", hextbl[h], rest
        } else {
            printf "%%%s", parts[j]
        }
    }
}' | sed '1s/^\xEF\xBB\xBF//' | tr -d '\r' > "$path"

# 4. Проверка результата
if [ -s "$path" ]; then
    echo "Сохранено успешно (BOM и возвраты каретки удалены).<br>"
else
    echo "<span style='color:orange;'>Файл сохранен, но он пустой.</span><br>"
fi

rm -f "$TMP_RAW"
echo "<a href=\"javascript:window.open(window.location, '_self').close();\">Закрыть окно</a>"

