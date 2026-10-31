#!/bin/sh

unescape() {
    echo "$1" | sed 's/+/ /g; s/%/\\x/g' | xargs -0 printf "%b"
}

echo "Content-type: text/html; charset=utf-8"
echo ""

overl_ok=0
overlays=""
name=""

if [ "$REQUEST_METHOD" = "POST" ] && [ -n "$CONTENT_LENGTH" ] && [ "$CONTENT_LENGTH" -gt 0 ]; then
    POST_DATA=$(dd bs=1 count=$CONTENT_LENGTH 2>/dev/null)
else
    POST_DATA="$QUERY_STRING"
fi

OLD_IFS=$IFS
IFS='&'

for pair in $POST_DATA; do
    IFS=$OLD_IFS
    key="${pair%%=*}"
    val_raw="${pair#*=}"
    
    # Декодуємо значення
    value=$(unescape "$val_raw")

    if [ "$key" = "overlays" ]; then
        overlays="overlays"
        name="$value"
        overl_ok=1
    else
        # Важливо: awk приймав сирий ключ $1, тому передаємо оригінальний key
        driv="$key"
        stats="$value"
        # Виконуємо системну команду у фоні/тиші як було в awk (&> /dev/null замінено на сумісний >/dev/null 2>&1)
        boot-config "$driv" "$stats" >/dev/null 2>&1
    fi
    IFS='&'
done
IFS=$OLD_IFS

# Блок END
if [ "$overl_ok" = "1" ]; then
    echo "<b>$overlays -- $name</b><br>"
    /www/cgi-bin/modules/boot-config/index.html "$overlays" "$name"
else
    /www/cgi-bin/modules/boot-config/index.html "save_okay"
fi
