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

define Package/i2c_HD44780/postinst
endef

define Package/i2c_HD44780/prerm
#!/bin/sh
	killall -9 i2c_HD44780 2> /dev/null > /dev/null
endef

define Package/i2c_HD44780/postrm
#!/bin/sh
sed -i "/i2c_HD44780\/index.cgi/d" /www/menu.html 2> /dev/null
rmdir /www/cgi-bin/modules/i2c_HD44780 2> /dev/null
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
	$(INSTALL_BIN) ./src/uci_defaults_i2c_HD44780 $(1)/etc/uci-defaults/50-i2c_HD44780

##		WEB-MORDA_Cyberwrt
	$(INSTALL_DIR) $(1)/www/modules/i2c_hd44780
	$(INSTALL_DATA) ./files/b.jpg $(1)/www/modules/i2c_hd44780/
	$(INSTALL_DATA) ./files/r.jpg $(1)/www/modules/i2c_hd44780/
	$(INSTALL_DIR) $(1)/www/cgi-bin/modules/i2c_hd44780
	$(INSTALL_BIN) ./files/index.cgi $(1)/www/cgi-bin/modules/i2c_hd44780/
	$(INSTALL_BIN) ./files/index.html $(1)/www/cgi-bin/modules/i2c_hd44780/
	$(INSTALL_BIN) ./files/Setup.cgi $(1)/www/cgi-bin/modules/i2c_hd44780/
	$(INSTALL_BIN) ./files/Save.cgi $(1)/www/cgi-bin/modules/i2c_hd44780/
endef

$(eval $(call BuildPackage,i2c_HD44780))
