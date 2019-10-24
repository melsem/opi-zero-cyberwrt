include $(TOPDIR)/rules.mk

PKG_NAME:=termostat-serial-arduino
PKG_VERSION:=19.02.2018
PKG_RELEASE:=test

PKG_BUILD_DIR:= $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/termostat-serial-arduino
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=termostat-serial-arduino
  DEPENDS:=+port_listen
#  DEPENDS:=+port_listen +kmod-usb-serial-ch341 +kmod-usb-serial-pl2303
endef

define Package/termostat-serial-arduino/description
 termostat-serial-arduino << read arduino-menu >>
 help(RU) http://cyber-place.ru/showthread.php?t=2752
endef

define Package/termostat-serial-arduino/prerm
killall port_listen
endef

define Package/termostat-serial-arduino/postrm
#!/bin/sh
sed -i "/termostat-serial-arduino\/index.cgi/d" /www/menu.html
rm /etc/config/termostat-serial-arduino  2> /dev/null
rm /www/cgi-bin/modules/termostat-serial-arduino/saveApply.cgi 2> /dev/null
rm /www/cgi-bin/modules/termostat-serial-arduino/setupMenu.cgi 2> /dev/null
rm /www/cgi-bin/modules/termostat-serial-arduino/index.cgi 2> /dev/null
rm /www/cgi-bin/modules/termostat-serial-arduino/menuwriter.d 2> /dev/null
rm /www/cgi-bin/modules/termostat-serial-arduino/sync.cgi 2> /dev/null
rmdir /www/cgi-bin/modules/termostat-serial-arduino 2> /dev/null
rm /usr/lib/opkg/info/termostat-serial-arduino.control 2> /dev/null
rm /usr/lib/opkg/info/termostat-serial-arduino.postinst 2> /dev/null
rm /usr/lib/opkg/info/termostat-serial-arduino.postinst-pkg 2> /dev/null
rm /usr/lib/opkg/info/termostat-serial-arduino.prerm 2> /dev/null
rm /usr/lib/opkg/info/termostat-serial-arduino.prerm-pkg 2> /dev/null
rm /usr/lib/opkg/info/termostat-serial-arduino.list 2> /dev/null
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
endef

define Package/termostat-serial-arduino/install
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_DATA) ./files/termostat-serial-arduino.conf $(1)/etc/config/termostat-serial-arduino
	$(INSTALL_DIR) $(1)/etc/uci-defaults
	$(INSTALL_BIN) ./src/uci_defaults_termostat-serial-arduino $(1)/etc/uci-defaults/50-termostat-serial-arduino

##		WEB-Cyberwrt
	$(INSTALL_DIR) $(1)/www/cgi-bin/modules/termostat-serial-arduino
	$(INSTALL_BIN) ./files/index.cgi $(1)/www/cgi-bin/modules/termostat-serial-arduino/
	$(INSTALL_BIN) ./files/setupMenu.cgi $(1)/www/cgi-bin/modules/termostat-serial-arduino/
	$(INSTALL_BIN) ./files/saveApply.cgi $(1)/www/cgi-bin/modules/termostat-serial-arduino/
	$(INSTALL_BIN) ./files/sync.cgi $(1)/www/cgi-bin/modules/termostat-serial-arduino/
endef

$(eval $(call BuildPackage,termostat-serial-arduino))
