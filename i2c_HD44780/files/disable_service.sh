#!/bin/sh
if [ -z "${IPKG_INSTROOT}" ]; then
	/etc/init.d/i2c_HD44780.init disable
	/etc/init.d/i2c_HD44780.init stop
fi
# Шукаємо слово i2c_hd44780 або i2c_HD44780 у будь-якому регістрі
if grep -qi "i2c_hd44780" /www/menu.html; then
    # Якщо знайшли — видаляємо весь рядок, де зустрілося це слово (ігноруючи регістр)
    sed -i '/i2c_[hH][dD]44780/d' /www/menu.html 2>/dev/null
fi
	# К моей вебке cyberwrt - раскомментировать если будет использоваться ####
sed -i 3i\ '<li><a href=/cgi-bin/modules/i2c_hd44780/index.cgi>'i2c-LCD-monitor'</a></li>' /www/menu.html 2> /dev/null
	# К вебке cyberwrt - закомментировть если НЕ будет использоваться ####
#sed -i 3i\ '<a href=/cgi-bin/modules/i2c_hd44780/index.cgi><div class=left>'i2c-LCD-monitor'</div></a>' /www/menu.html 2> /dev/null
exit 0

