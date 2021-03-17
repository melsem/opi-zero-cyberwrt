
include $(TOPDIR)/rules.mk

PKG_NAME:=boot-config
PKG_VERSION:=1.2
PKG_RELEASE:=2020.03.29

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
  SECTION:=utils
  CATEGORY:=Utilities
  DEPENDS:=@TARGET_sunxi +mount-utils +block-mount +uhttpd +dtc
  TITLE:=boot-config
  PKGARCH:=all
endef

define Package/$(PKG_NAME)/description
 Utility for editing boot configuration. Sunxi Allwiner H2+ H3
endef

define Package/$(PKG_NAME)/postrm
#!/bin/sh
sed -i "/boot-config\/index.cgi/d" /www/menu.html &> /dev/null
 rm -R /www/cgi-bin/modules/boot-config &> /dev/null
endef

define Build/Compile
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) ./src/boot-config $(1)/usr/sbin/

	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./src/cpufreq_scaling $(1)/etc/init.d/
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_BIN) ./src/cpufreq_scaling.conf $(1)/etc/config/cpufreq_scaling
	$(INSTALL_DIR) $(1)/etc/uci-defaults
	$(INSTALL_BIN) ./src/uci_defaults_boot-config $(1)/etc/uci-defaults/50-boot-config
	$(INSTALL_BIN) ./src/add-mount-boot $(1)/etc/uci-defaults/20-boot-config

	$(INSTALL_DIR) $(1)/www/boot
	$(INSTALL_BIN) ./files/boot.html $(1)/www/boot/index.html
	$(INSTALL_DIR) $(1)/www/cgi-bin/modules/boot-config
	$(INSTALL_BIN) ./files/conv.cgi $(1)/www/cgi-bin/modules/boot-config/
	$(INSTALL_BIN) ./files/index.cgi $(1)/www/cgi-bin/modules/boot-config/
	$(INSTALL_BIN) ./files/index.html $(1)/www/cgi-bin/modules/boot-config/
	$(INSTALL_BIN) ./files/Save.cgi $(1)/www/cgi-bin/modules/boot-config/
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
