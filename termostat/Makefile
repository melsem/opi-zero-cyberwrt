#

include $(TOPDIR)/rules.mk

PKG_NAME:=termostat
PKG_VERSION:=1.8
PKG_RELEASE:=08.01.2020-CyberWrt-GPIOx_1Wire

include $(INCLUDE_DIR)/package.mk

define Package/termostat
  SECTION:=utils
  CATEGORY:=Utilities
#  DEPENDS:=+kmod-w1 +kmod-w1-master-gpio +kmod-w1-gpio-custom +kmod-w1-slave-therm
  DEPENDS:=+kmod-w1 +kmod-w1-master-gpio +kmod-w1-slave-therm
  TITLE:=TERMOMETR-TERMOSTAT-auto-init-narodmon.ru
  PKGARCH:=all
endef

define Package/termostat/postinst
endef

define Package/termostat/prerm
#!/bin/sh
 ps > /tmp/1.ds18
 dserr18=$(awk -F" " '/dserr18/ {print $1}' /tmp/1.ds18)
 ds18b20=$(awk -F" " '/ds18b20/ {print $1}' /tmp/1.ds18)
 kill $dserr18 &>/dev/null
 kill $ds18b20 &>/dev/null
 rm /tmp/1.ds18
endef

define Package/termostat/postrm
#!/bin/sh
sed -i "/termostatGpio\/index.cgi/d" /www/menu.html 2> /dev/null
rmdir /www/cgi-bin/modules/termostatGpio 2> /dev/null
endef

define Build/Compile
endef

define Package/termostat/install
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_DATA) -m 0644 ./files/cconff/ds18b20 $(1)/etc/config/
	$(INSTALL_DIR) $(1)/etc/uci-defaults
	$(INSTALL_BIN) ./src/uci_defaults_termostat $(1)/etc/uci-defaults/50-termostat
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_DATA) -m 0644 ./files/tstatgpio.html $(1)/usr/bin/
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_DATA) -m 0755 ./files/ds18b20 $(1)/usr/sbin/
	$(INSTALL_DATA) -m 0755 ./files/dserr18 $(1)/usr/sbin/
	$(INSTALL_DATA) -m 0755 ./files/init18b20 $(1)/usr/sbin/
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_DATA) -m 0755 ./files/ds18b20_init $(1)/etc/init.d/

	$(INSTALL_DIR) $(1)/www/cgi-bin/modules/termostatGpio
	$(INSTALL_DATA) -m 0777 ./files/Sset.cgi $(1)/www/cgi-bin/modules/termostatGpio/
	$(INSTALL_DATA) -m 0777 ./files/index.cgi $(1)/www/cgi-bin/modules/termostatGpio/
	$(INSTALL_DATA) -m 0777 ./files/Sens0.cgi $(1)/www/cgi-bin/modules/termostatGpio/
	$(INSTALL_DATA) -m 0777 ./files/Sens1.cgi $(1)/www/cgi-bin/modules/termostatGpio/
	$(INSTALL_DATA) -m 0777 ./files/Sens2.cgi $(1)/www/cgi-bin/modules/termostatGpio/
	$(INSTALL_DATA) -m 0777 ./files/Sens3.cgi $(1)/www/cgi-bin/modules/termostatGpio/
	$(INSTALL_DATA) -m 0777 ./files/Sens4.cgi $(1)/www/cgi-bin/modules/termostatGpio/
	$(INSTALL_DATA) -m 0777 ./files/Sens5.cgi $(1)/www/cgi-bin/modules/termostatGpio/
	$(INSTALL_DATA) -m 0777 ./files/Sens6.cgi $(1)/www/cgi-bin/modules/termostatGpio/
	$(INSTALL_DATA) -m 0777 ./files/Sens7.cgi $(1)/www/cgi-bin/modules/termostatGpio/
	$(INSTALL_DATA) -m 0777 ./files/Sens8.cgi $(1)/www/cgi-bin/modules/termostatGpio/
	$(INSTALL_DATA) -m 0777 ./files/Sens9.cgi $(1)/www/cgi-bin/modules/termostatGpio/
	$(INSTALL_DATA) -m 0777 ./files/Setup.cgi $(1)/www/cgi-bin/modules/termostatGpio/
	$(INSTALL_DIR) $(1)/www/modules/termostatGpio
	$(INSTALL_DATA) -m 0777 ./files/r.jpg $(1)/www/modules/termostatGpio/
	$(INSTALL_DATA) -m 0777 ./files/b.jpg $(1)/www/modules/termostatGpio/

endef

$(eval $(call BuildPackage,termostat))

