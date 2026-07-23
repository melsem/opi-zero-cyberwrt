# http://www.cyber-place.ru/showthread.php?t=2164

include $(TOPDIR)/rules.mk

PKG_NAME:=i2c_HD44780
PKG_VERSION:=17.03.2018
PKG_RELEASE:=read-file+ds18b20_gpio+USB-UART_digitemp+clock_Web_Cyberwrt

include $(INCLUDE_DIR)/package.mk

define Package/i2c_HD44780
	SECTION:=utils
	CATEGORY:=Utilities
	TITLE:=i2c_HD44780 LCD monitor
#	DEPENDS:=+i2c-tools +kmod-i2c-gpio-custom +kmod-w1 +kmod-w1-master-gpio +kmod-w1-gpio-custom +kmod-w1-slave-therm +digitemp +digitemp-usb +kmod-usb-serial-pl2303
	DEPENDS:=+i2c-tools +kmod-i2c-algo-bit +kmod-i2c-core +kmod-i2c-gpio
endef

define Package/i2c_HD44780/description
 LCD monitor for HD44780 over I2C by PCF8574 bus expander.
 W1-gpio thermostat-thermometer 4-sensors or digitemp-usb thermometer.
 help(RU) http://www.cyber-place.ru/showthread.php?t=2164
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
	$(TARGET_CC) $(TARGET_CFLAGS) -c -o $(PKG_BUILD_DIR)/i2c_HD44780.o $(PKG_BUILD_DIR)/i2c_HD44780.c
	$(TARGET_CC) $(TARGET_LDFLAGS) -o $(PKG_BUILD_DIR)/i2c_HD44780 $(PKG_BUILD_DIR)/i2c_HD44780.o
endef

define Package/i2c_HD44780/install
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

	# 2. Додаємо хук для автоматичного вимкнення після встановлення:
	$(INSTALL_DIR) $(1)/usr/lib/opkg/info
	$(INSTALL_BIN) ./files/disable_service.sh $(1)/usr/lib/opkg/info/$(PKG_NAME).postinst
endef

$(eval $(call BuildPackage,i2c_HD44780))
