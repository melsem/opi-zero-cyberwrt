
# Copyright (C) 2016 Openwrt.org
#
# This is free software, licensed under the Apache License, Version 2.0 .
#

include $(TOPDIR)/rules.mk

LUCI_TITLE:=LuCI for CPU Freq Setting
LUCI_DEPENDS:=@TARGET_sunxi
PKG_NAME:=luci-app-cpufreq_scaling
PKG_VERSION:=1.2
PKG_RELEASE:=04.03.2021

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
	SECTION:=luci
	CATEGORY:=LuCI
	SUBMENU:=3. Applications
	TITLE:=cpufreq scaling interface for LuCI
	PKGARCH:=all
	DEPENDS:=@TARGET_sunxi
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./* $(PKG_BUILD_DIR)/
endef

define Build/Compile
endef

define Package/$(PKG_NAME)/install

	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./src/cpufreq.init $(1)/etc/init.d/cpufreq
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_BIN) ./src/cpufreq.conf $(1)/etc/config/cpufreq
	$(INSTALL_DIR) $(1)/usr/lib/lua/luci
	cp -pR $(PKG_BUILD_DIR)/luasrc/* $(1)/usr/lib/lua/luci/
	$(INSTALL_DIR) $(1)/usr/lib/lua/luci/i18n
	$$(foreach po,$$(shell find $(PKG_BUILD_DIR)/po/*/*.po), \
		po2lmo $$(po) \
		$(1)/usr/lib/lua/luci/i18n/cpufreq.$$(shell echo $$(po) | awk -F'/' '{print $$$$(NF-1)}').lmo;)
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
