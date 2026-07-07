#!/bin/sh

# Функція декодування URL (залишена для сумісності структури)
unescape() {
    echo "$1" | sed 's/%0[Dd]//g; s/+/ /g; s/%/\\x/g' | xargs -0 printf "%b"
}

# 1. Обов'язкові HTTP-заголовки
echo "Content-type: text/html; charset=utf-8"
echo ""

# 2. Зчитування POST або GET даних (пропускаємо, бо awk їх не обробляв у циклі)
if [ "$REQUEST_METHOD" = "POST" ] && [ -n "$CONTENT_LENGTH" ] && [ "$CONTENT_LENGTH" -gt 0 ]; then
    POST_DATA=$(dd bs=1 count=$CONTENT_LENGTH 2>/dev/null)
else
    POST_DATA="$QUERY_STRING"
fi

# 3. Еквівалент блоку END (Основна логіка)

# Копіюємо файл pp.cgi з модуля termostat-serial-arduino у /tmp як pp.sh
cp -f /www/cgi-bin/modules/termostat-serial-arduino/pp.cgi /tmp/pp.sh

# Надаємо права на виконання
chmod 755 /tmp/pp.sh

# Запускаємо фоновий процес безпечно через direct-виклик з перенаправленням потоків
/tmp/pp.sh >/dev/null 2>&1 &

# Виводимо інформаційне повідомлення для користувача
echo "<br><b>Please wait 10 sek.</b><br>"

# Повертаємо користувача до головного меню менеджеру модулів
/www/cgi-bin/modules/manmod/index.cgi

