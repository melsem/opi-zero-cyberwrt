
include $(TOPDIR)/rules.mk

PKG_NAME:=boot-config-spi
PKG_VERSION:=1.2
PKG_RELEASE:=2020.04.30

include $(INCLUDE_DIR)/package.mk

define Package/boot-config-spi
  SECTION:=utils
  CATEGORY:=Utilities
  DEPENDS:=@TARGET_sunxi +uhttpd +dtc
  TITLE:=boot-config-spi
  PKGARCH:=all
endef

define Package/boot-config-spi/description
 Utility for editing boot configuration. Sunxi Allwiner H2+ H3
endef

define Package/boot-config-spi/postinst
endef

define Package/boot-config-spi/postrm
#!/bin/sh
sed -i "/boot-config-spi\/index.cgi/d" /www/menu.html &> /dev/null
 rm -R /www/cgi-bin/modules/boot-config-spi &> /dev/null
endef

define Build/Compile
endef

define Package/boot-config-spi/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) ./files/boot-config-spi $(1)/usr/sbin/

	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/cpufreq_scaling.init $(1)/etc/init.d/cpufreq_scaling
	$(INSTALL_DIR) $(1)/etc/uci-defaults
	$(INSTALL_BIN) ./files/uci_defaults_boot-config $(1)/etc/uci-defaults/50-boot-config-spi

	$(INSTALL_DIR) $(1)/www/boot
	$(INSTALL_BIN) ./files/boot.html $(1)/www/boot/index.html
	$(INSTALL_DIR) $(1)/www/cgi-bin/modules/boot-config-spi
	$(INSTALL_BIN) ./files/conv.cgi $(1)/www/cgi-bin/modules/boot-config-spi/
	$(INSTALL_BIN) ./files/index.cgi $(1)/www/cgi-bin/modules/boot-config-spi/
	$(INSTALL_BIN) ./files/index.html $(1)/www/cgi-bin/modules/boot-config-spi/
	$(INSTALL_BIN) ./files/Save.cgi $(1)/www/cgi-bin/modules/boot-config-spi/
endef

$(eval $(call BuildPackage,boot-config-spi))
