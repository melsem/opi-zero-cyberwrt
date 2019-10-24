
include $(TOPDIR)/rules.mk

PKG_NAME:=web-radio
PKG_VERSION:=client-10a
PKG_RELEASE:=01.04.2017

include $(INCLUDE_DIR)/package.mk

define Package/web-radio2
  SECTION:=utils
  CATEGORY:=Utilities
  DEPENDS:=+kmod-sound-core +kmod-usb-audio +madplay +kmod-usb-ohci +kmod-usb2 +curl
  TITLE:=web-radio2
  PKGARCH:=all
endef

define Package/web-radio2/description
 web-radio2.
 help(RU) http://cyber-place.ru/showthread.php?t=2731
endef

define Package/web-radio2/postinst
endef

define Package/web-radio2/prerm
#!/bin/sh
	killall -9 web-radio2 2> /dev/null > /dev/null
	killall -9 curl 2> /dev/null > /dev/null
	killall -9 wget 2> /dev/null > /dev/null
	killall -9 madplay 2> /dev/null > /dev/null
endef

define Package/web-radio2/postrm
#!/bin/sh
uci -q show system.@button[0]
while [ $? -ne 1 ]; do
uci delete system.@button[0]
uci commit system
uci -q show system.@button[0]
done

sed -i "/web-radio2\/index.cgi/d" /www/menu.html 2> /dev/null
 rm -R /www/cgi-bin/modules/web-radio2 2> /dev/null
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(TARGET_CC) $(TARGET_CFLAGS) -c -o $(PKG_BUILD_DIR)/webradio.o $(PKG_BUILD_DIR)/webradio.c
	$(TARGET_CC) $(TARGET_LDFLAGS) -o $(PKG_BUILD_DIR)/web-radio2 $(PKG_BUILD_DIR)/webradio.o
endef

define Package/web-radio2/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/web-radio2 $(1)/usr/sbin/

	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./src/web-radio2.init $(1)/etc/init.d/
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_DATA) ./src/web-radio2.playlist $(1)/etc/config/web-radio2
	$(INSTALL_DIR) $(1)/etc/uci-defaults
	$(INSTALL_BIN) ./src/uci_defaults_web-radio2 $(1)/etc/uci-defaults/50-web-radio2
	$(INSTALL_DIR) $(1)/etc/hotplug.d/button
	$(INSTALL_BIN) ./src/00-button $(1)/etc/hotplug.d/button/

	$(INSTALL_DIR) $(1)/www/modules/web-radio2
	$(INSTALL_DATA) ./files/b.jpg $(1)/www/modules/web-radio2/
	$(INSTALL_DATA) ./files/r.jpg $(1)/www/modules/web-radio2/

	$(INSTALL_DIR) $(1)/www/cgi-bin/modules/web-radio2
	$(INSTALL_BIN) ./files/index.cgi $(1)/www/cgi-bin/modules/web-radio2/
	$(INSTALL_BIN) ./files/save.cgi $(1)/www/cgi-bin/modules/web-radio2/
	$(INSTALL_BIN) ./files/setup.cgi $(1)/www/cgi-bin/modules/web-radio2/
endef

$(eval $(call BuildPackage,web-radio2))
