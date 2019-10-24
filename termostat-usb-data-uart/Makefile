#
# Copyright (C) 2006-2009 OpenWrt.org
# Copyright 2010 Vertical Communications
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=termostat-usb-data-uart
PKG_VERSION:=1.9
PKG_RELEASE:=27.11.2016-CyberWrt

include $(INCLUDE_DIR)/package.mk

define Package/termostat-usb-data-uart
  SECTION:=utils
  CATEGORY:=Utilities
  DEPENDS:=+digitemp +digitemp-usb +libusb-compat
  TITLE:=TERMOMETR-TERMOSTAT-auto-init-narodmon.ru
  PKGARCH:=all
endef

define Package/termostat-usb-data-uart/postinst
endef

define Package/termostat-usb-data-uart/prerm
#!/bin/sh
	killall -9 ds18b20usba 2> /dev/null > /dev/null
	killall -9 dserr18usba 2> /dev/null > /dev/null
endef

define Package/termostat-usb-data-uart/postrm
#!/bin/sh
sed -i "/termostatUsb\/index.cgi/d" /www/menu.html 2> /dev/null
rmdir /www/cgi-bin/modules/termostatUsb 2> /dev/null
endef

define Build/Compile
endef

define Package/termostat-usb-data-uart/install
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_DATA) -m 0644 ./files/ds18b20a_conf $(1)/etc/config/
	$(INSTALL_DIR) $(1)/etc/uci-defaults
	$(INSTALL_BIN) ./src/uci_defaults_termostat-usb-data-uart $(1)/etc/uci-defaults/50-termostat-usb-data-uart
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_DATA) -m 0644 ./files/tstatusb.html $(1)/usr/bin/
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_DATA) -m 0755 ./files/ds18b20usba $(1)/usr/sbin/
	$(INSTALL_DATA) -m 0755 ./files/dserr18usba $(1)/usr/sbin/
	$(INSTALL_DATA) -m 0755 ./files/init18b20usba $(1)/usr/sbin/
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_DATA) -m 0755 ./files/ds18b20usba_init $(1)/etc/init.d/
	$(INSTALL_DATA) -m 0755 ./files/dserr18usba_init $(1)/etc/init.d/

	$(INSTALL_DIR) $(1)/www/cgi-bin/modules/termostatUsb
	$(INSTALL_DATA) -m 0777 ./files/index.cgi $(1)/www/cgi-bin/modules/termostatUsb/
	$(INSTALL_DATA) -m 0777 ./files/Sset.cgi $(1)/www/cgi-bin/modules/termostatUsb/
	$(INSTALL_DATA) -m 0777 ./files/Sens0.cgi $(1)/www/cgi-bin/modules/termostatUsb/
	$(INSTALL_DATA) -m 0777 ./files/Sens1.cgi $(1)/www/cgi-bin/modules/termostatUsb/
	$(INSTALL_DATA) -m 0777 ./files/Sens2.cgi $(1)/www/cgi-bin/modules/termostatUsb/
	$(INSTALL_DATA) -m 0777 ./files/Sens3.cgi $(1)/www/cgi-bin/modules/termostatUsb/
	$(INSTALL_DATA) -m 0777 ./files/Sens4.cgi $(1)/www/cgi-bin/modules/termostatUsb/
	$(INSTALL_DATA) -m 0777 ./files/Sens5.cgi $(1)/www/cgi-bin/modules/termostatUsb/
	$(INSTALL_DATA) -m 0777 ./files/Sens6.cgi $(1)/www/cgi-bin/modules/termostatUsb/
	$(INSTALL_DATA) -m 0777 ./files/Sens7.cgi $(1)/www/cgi-bin/modules/termostatUsb/
	$(INSTALL_DATA) -m 0777 ./files/Sens8.cgi $(1)/www/cgi-bin/modules/termostatUsb/
	$(INSTALL_DATA) -m 0777 ./files/Sens9.cgi $(1)/www/cgi-bin/modules/termostatUsb/
	$(INSTALL_DATA) -m 0777 ./files/Setup.cgi $(1)/www/cgi-bin/modules/termostatUsb/
	$(INSTALL_DIR) $(1)/www/modules/termostatUsb
	$(INSTALL_DATA) -m 0777 ./files/r.jpg $(1)/www/modules/termostatUsb/
	$(INSTALL_DATA) -m 0777 ./files/b.jpg $(1)/www/modules/termostatUsb/
endef

$(eval $(call BuildPackage,termostat-usb-data-uart))

