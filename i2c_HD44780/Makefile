# http://www.cyber-place.ru/showthread.php?t=2164

include $(TOPDIR)/rules.mk

PKG_NAME:=i2c_HD44780
PKG_VERSION:=3.1
PKG_RELEASE:=2.8

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)/config
	choice
		prompt "Select LCD Hardware Interface Mode"
		default LCD_MODE_I2C
		depends on PACKAGE_i2c_HD44780
		help
		  Choose the hardware interface driver mode for HD44780 LCD.
		  You can select either I2C expander or High-Speed Native GPIO.

		config LCD_MODE_I2C
			bool "I2C Bus expander Mode (PCF8574)"
			help
			  Classic mode for HD44780 via I2C PCF8574 board.
			  Uses standard /dev/i2c-X interface.

		config LCD_MODE_GPIO
			bool "Native GPIO Driver Mode (hd44780.ko)"
			help
			  High-speed native GPIO mode using custom kernel module.
			  Uses standard /dev/hd44780.X interface..
	endchoice
endef

define Package/$(PKG_NAME)
	SECTION:=utils
	CATEGORY:=Utilities
	TITLE:=Universal LCD monitor for HD44780 (I2C PCF8574 / Native GPIO /dev/hd44780.x)
#	DEPENDS:=+i2c-tools +kmod-i2c-gpio-custom +kmod-w1 +kmod-w1-master-gpio +kmod-w1-gpio-custom +kmod-w1-slave-therm +digitemp +digitemp-usb +kmod-usb-serial-pl2303
#	DEPENDS:=+i2c-tools +kmod-i2c-algo-bit +kmod-i2c-core +kmod-i2c-gpio
	DEPENDS:=+LCD_MODE_I2C:i2c-tools kmod-i2c-algo-bit kmod-i2c-core kmod-i2c-gpio +LCD_MODE_GPIO:kmod-hd44780-gpio-custom

endef

define Package/$(PKG_NAME)/description
 Universal LCD monitor for HD44780 screens. Supports both I2C 
 via PCF8574 expander and modern Native GPIO via kernel driver (/dev/hd44780.x).
 Features: W1-gpio thermostat with 4-sensors, digitemp-usb support,
 full web interface control and automatic error logging.
 Original help (RU): http://www.cyber-place.ru/showthread.php?t=2164
endef

define Package/$(PKG_NAME)/postrm
#!/bin/sh
if grep -qi "i2c_hd44780" /www/menu.html; then
    sed -i '/i2c_[hH][dD]44780/d' /www/menu.html
fi
rm -rf /www/modules/i2c_hd44780
rm -rf /www/cgi-bin/modules/i2c_hd44780
exit 0
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	# 1. Компілюємо С-код програми (Ваш оригінальний код)
	$(eval MODE_FLAG:=$(if $(CONFIG_LCD_MODE_GPIO),-DLCD_MODE_GPIO,-DLCD_MODE_I2C))
	$(TARGET_CC) $(TARGET_CFLAGS) $(MODE_FLAG) -c -o $(PKG_BUILD_DIR)/i2c_HD44780.o $(PKG_BUILD_DIR)/i2c_HD44780.c
	$(TARGET_CC) $(TARGET_LDFLAGS) -o $(PKG_BUILD_DIR)/i2c_HD44780 $(PKG_BUILD_DIR)/i2c_HD44780.o

	# 2. Визначаємо фінальне слово (змінну) для підстановки
	$(eval MODE_WORD:=$(if $(CONFIG_LCD_MODE_GPIO),LCD_MODE_GPIO,LCD_MODE_I2C))

	# 3. Копіюємо ваші чисті шаблони з ./files/ у робочу папку збірки
	cp ./files/modern_Setup.cgi $(PKG_BUILD_DIR)/

	# 4. ВАШ ІДЕАЛЬНИЙ ТА НАДІЙНИЙ РЯДОК (Екранування зворотними косими рисками)
	sed -i 's/lcd_mode_dev=\"__LCD_MODE_TEMPLATE__\"/lcd_mode_dev=\"$(MODE_WORD)\"/g' $(PKG_BUILD_DIR)/modern_Setup.cgi
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/i2c_HD44780 $(1)/usr/sbin/
	$(INSTALL_BIN) ./files/digitemp_dem $(1)/usr/sbin/
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_DATA) ./files/t_h $(1)/usr/bin/
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_DATA) ./files/i2c_HD44780.conf $(1)/etc/config/i2c_HD44780
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/i2c_HD44780.init $(1)/etc/init.d/
	$(INSTALL_DIR) $(1)/etc/uci-defaults
	$(INSTALL_BIN) ./files/uci_defaults_i2c_HD44780 $(1)/etc/uci-defaults/50-i2c_HD44780

##		WEB-MORDA_Cyberwrt
	$(INSTALL_DIR) $(1)/www/modules/i2c_hd44780
	$(INSTALL_DATA) ./files/b.jpg $(1)/www/modules/i2c_hd44780/
	$(INSTALL_DATA) ./files/r.jpg $(1)/www/modules/i2c_hd44780/
	$(INSTALL_DIR) $(1)/www/cgi-bin/modules/i2c_hd44780
	$(INSTALL_BIN) ./files/index.cgi $(1)/www/cgi-bin/modules/i2c_hd44780/
	$(INSTALL_BIN) ./files/index.html $(1)/www/cgi-bin/modules/i2c_hd44780/
	$(INSTALL_BIN) ./files/Save.cgi $(1)/www/cgi-bin/modules/i2c_hd44780/
	$(INSTALL_BIN) ./files/get_temp.cgi $(1)/www/cgi-bin/modules/i2c_hd44780/
	$(INSTALL_BIN) ./files/Setup.cgi $(1)/www/cgi-bin/modules/i2c_hd44780/Setup.cgi
#	$(INSTALL_BIN) $(PKG_BUILD_DIR)/modern_Setup.cgi $(1)/www/cgi-bin/modules/i2c_hd44780/Setup.cgi
#	$(INSTALL_BIN) ./files/test_Setup.cgi $(1)/www/cgi-bin/modules/i2c_hd44780/Setup.cgi

	# 2. Додаємо хук для автоматичного вимкнення після встановлення:
	$(INSTALL_DIR) $(1)/usr/lib/opkg/info
	$(INSTALL_BIN) ./files/disable_service.sh $(1)/usr/lib/opkg/info/$(PKG_NAME).postinst
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
