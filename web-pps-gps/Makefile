
include $(TOPDIR)/rules.mk

PKG_NAME:=web-pps-gps
PKG_VERSION:=1.0
PKG_RELEASE:=21.03.2020

include $(INCLUDE_DIR)/package.mk

define Package/web-pps-gps
  SECTION:=utils
  CATEGORY:=Utilities
  DEPENDS:=@TARGET_sunxi +chrony +gpsd +gpsd-clients +pps-tools +kmod-pps +kmod-pps-gpio
  TITLE:=web-pps-gps setup
endef

define Package/web-pps-gps/postrm
#!/bin/sh
 sed -i "/web-pps-gps\/index.cgi/d" /www/menu.html & > /dev/null
 rm -R /www/cgi-bin/modules/web-pps-gps & > /dev/null
endef

define Build/Compile
endef

define Package/web-pps-gps/install
	$(INSTALL_DIR) $(1)/etc/uci-defaults
	$(INSTALL_BIN) ./files/uci_defaults_web-pps-gps $(1)/etc/uci-defaults/51-web-pps-gps

	$(INSTALL_DIR) $(1)/www/cgi-bin/modules/web-pps-gps
	$(INSTALL_BIN) ./files/index.cgi $(1)/www/cgi-bin/modules/web-pps-gps/
	$(INSTALL_BIN) ./files/index.html $(1)/www/cgi-bin/modules/web-pps-gps/
	$(INSTALL_BIN) ./files/Save.cgi $(1)/www/cgi-bin/modules/web-pps-gps/

#	$(INSTALL_BIN) ./files/Setup.cgi $(1)/www/cgi-bin/modules/web-pps-gps/
#	$(INSTALL_DIR) $(1)/www/modules/web-pps-gps
#	$(INSTALL_DATA) ./files/b.jpg $(1)/www/modules/web-pps-gps/
#	$(INSTALL_DATA) ./files/r.jpg $(1)/www/modules/web-pps-gps/
endef

$(eval $(call BuildPackage,web-pps-gps))
