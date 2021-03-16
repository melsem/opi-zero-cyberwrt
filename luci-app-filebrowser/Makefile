# Copyright 2019 monwf (boluo2@gmail.com) and giaulo
# This is free software, licensed under the Apache License, Version 2.0

include $(TOPDIR)/rules.mk

PKG_NAME:=luci-app-filebrowser
PKG_VERSION:=1
PKG_RELEASE:=16.03.2021

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
	SECTION:=luci
	CATEGORY:=LuCI
	SUBMENU:=3. Applications
	TITLE:=File browser for OpenWrt
	PKGARCH:=all
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./* $(PKG_BUILD_DIR)/
endef

define Build/Compile
endef

define Package/$(PKG_NAME)/postinst
#!/bin/sh
rm -fr /tmp/luci-indexcache /tmp/luci-modulecache
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/www
	cp -pR $(PKG_BUILD_DIR)/htdocs/* $(1)/www/

	$(INSTALL_DIR) $(1)/usr/lib/lua/luci
	cp -pR $(PKG_BUILD_DIR)/luasrc/* $(1)/usr/lib/lua/luci/
	$(INSTALL_DIR) $(1)/usr/lib/lua/luci/i18n
	$$(foreach po,$$(shell find $(PKG_BUILD_DIR)/po/*/*.po), \
		po2lmo $$(po) \
		$(1)/usr/lib/lua/luci/i18n/filebrowser.$$(shell echo $$(po) | awk -F'/' '{print $$$$(NF-1)}').lmo;)
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
