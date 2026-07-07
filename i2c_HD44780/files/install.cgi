#!/bin/sh

# Функція декодування URL (збережена для сумісності з структурою)
unescape() {
    echo "$1" | sed 's/%0[Dd]//g; s/+/ /g; s/%/\\x/g' | xargs -0 printf "%b"
}

# 1. Обов'язкові HTTP-заголовки
echo "Content-type: text/html; charset=utf-8"
echo ""

# 2. Зчитування POST або GET даних (пропускаємо, бо awk їх не використовував)
if [ "$REQUEST_METHOD" = "POST" ] && [ -n "$CONTENT_LENGTH" ] && [ "$CONTENT_LENGTH" -gt 0 ]; then
    POST_DATA=$(dd bs=1 count=$CONTENT_LENGTH 2>/dev/null)
else
    POST_DATA="$QUERY_STRING"
fi

# 3. Блок END (основна логіка виконання)

# Копіюємо файл pp.cgi у /tmp як pp.sh
cp -f /www/cgi-bin/modules/i2c_hd44780/pp.cgi /tmp/pp.sh

# Надаємо права на виконання
chmod 755 /tmp/pp.sh

# Запускаємо скрипт у фоні (&)
/tmp/pp.sh >/dev/null 2>&1 &

# Виводимо повідомлення користувачу
echo "<br><b>Please wait 10 sek.</b><br>"

# Викликаємо індексний cgi-скрипт менеджера модулів
/www/cgi-bin/modules/manmod/index.cgi

