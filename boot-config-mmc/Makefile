
include $(TOPDIR)/rules.mk

PKG_NAME:=boot-config-mmc
PKG_VERSION:=1.2
PKG_RELEASE:=2020.03.29

include $(INCLUDE_DIR)/package.mk

define Package/boot-config-mmc
  SECTION:=utils
  CATEGORY:=Utilities
  DEPENDS:=@TARGET_sunxi +mount-utils +block-mount +uhttpd +dtc
  TITLE:=boot-config-mmc
  PKGARCH:=all
endef

define Package/boot-config-mmc/description
 Utility for editing boot configuration. Sunxi Allwiner H2+ H3
endef

define Package/$(PKG_NAME)/postinst
#!/bin/sh
cat /tmp/cpufreq_scaling.init > /etc/init.d/cpufreq_scaling
rm -fr /tmp/cpufreq_scaling.init > /dev/null
endef

define Package/boot-config-mmc/postrm
#!/bin/sh
sed -i "/boot-config-mmc\/index.cgi/d" /www/menu.html &> /dev/null
 rm -R /www/cgi-bin/modules/boot-config-mmc &> /dev/null
endef

define Build/Compile
endef

define Package/boot-config-mmc/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) ./src/boot-config-mmc $(1)/usr/sbin/

	$(INSTALL_DIR) $(1)/tmp
	$(INSTALL_BIN) ./src/cpufreq_scaling $(1)/tmp/cpufreq_scaling.init
	$(INSTALL_DIR) $(1)/etc/uci-defaults
	$(INSTALL_BIN) ./src/uci_defaults_boot-config $(1)/etc/uci-defaults/50-boot-config
	$(INSTALL_BIN) ./src/add-mount-boot $(1)/etc/uci-defaults/20-boot-config-mmc

	$(INSTALL_DIR) $(1)/www/boot
	$(INSTALL_BIN) ./files/boot.html $(1)/www/boot/index.html
	$(INSTALL_DIR) $(1)/www/cgi-bin/modules/boot-config-mmc
	$(INSTALL_BIN) ./files/conv.cgi $(1)/www/cgi-bin/modules/boot-config-mmc/
	$(INSTALL_BIN) ./files/index.cgi $(1)/www/cgi-bin/modules/boot-config-mmc/
	$(INSTALL_BIN) ./files/index.html $(1)/www/cgi-bin/modules/boot-config-mmc/
	$(INSTALL_BIN) ./files/Save.cgi $(1)/www/cgi-bin/modules/boot-config-mmc/
endef

$(eval $(call BuildPackage,boot-config-mmc))
