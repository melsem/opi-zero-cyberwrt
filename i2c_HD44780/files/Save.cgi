#!/bin/sh

# Функція декодування URL (видаляє %0D, міняє + на пробіли, декодує %XX)
unescape() {
    echo "$1" | sed 's/%0[Dd]//g; s/+/ /g; s/%/\\x/g' | xargs -0 printf "%b"
}

# 1. Обов'язкові HTTP-заголовки
echo "Content-type: text/html; charset=utf-8"
echo ""

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
    
    # Декодуємо значення
    val=$(unescape "$val_raw")

    case "$key" in
        # Секція @display[0]
        device|address|sda_gpio|scl_gpio|lcd_number|gros_simvol|info_banner|read_file)
            uci set i2c_HD44780.@display[0]."$key"="$val"
            ;;

        # Секція @displ_tip[0]
        while|clock|led_on|on_read|on_ds18|tip_ds1|tip_ds2|tip_ds3|tip_ds4)
            uci set i2c_HD44780.@displ_tip[0]."$key"="$val"
            ;;

        # Секція @ds18b20[0]
        w1_gpio|usb_w1_uart|\
        name_snr1|name_snr2|name_snr3|name_snr4|\
        sensor1|sensor2|sensor3|sensor4|\
        histe1|histe2|histe3|histe4|\
        alarm1|alarm2|alarm3|alarm4|\
        nGp1_out|nGp2_out|nGp3_out|nGp4_out|\
        invers1|invers2|invers3|invers4)
            uci set i2c_HD44780.@ds18b20[0]."$key"="$val"
            ;;

        # Системні команди та керування службою
        cozapusk)
            /etc/init.d/i2c_HD44780.init start
            ;;
        coostanov)
            /etc/init.d/i2c_HD44780.init stop
            ;;
        staron)
            /etc/init.d/i2c_HD44780.init enable
            ;;
        staroff)
            /etc/init.d/i2c_HD44780.init disable
            ;;
        save_commit)
            uci commit i2c_HD44780
            ;;
        save_com_restart)
            uci commit i2c_HD44780
            /etc/init.d/i2c_HD44780.init stop
            sleep 1
            /etc/init.d/i2c_HD44780.init start
            ;;
    esac
    IFS='&'
done
IFS=$OLD_IFS

# 4. Блок END (затримка та редирект на сторінку налаштувань)
sleep 1
/www/cgi-bin/modules/i2c_hd44780/index.html "save_setup"

