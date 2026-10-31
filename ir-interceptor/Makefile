include $(TOPDIR)/rules.mk

PKG_NAME:=ir-interceptor
PKG_VERSION:=1.0.0
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=Smart Input Interceptor for IR, Rotary Encoders and GPIO
  DEPENDS:=+kmod-input-core
endef

define Package/$(PKG_NAME)/description
  A lightweight input event interceptor for IR remotes, 
  rotary encoders, and GPIO keys optimized for OpenWrt.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		CC="$(TARGET_CC)" \
		CFLAGS="$(TARGET_CFLAGS) -Wall"
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/ir-interceptor $(1)/usr/bin/

	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_CONF) ./files/ir-interceptor.conf $(1)/etc/config/ir-interceptor

	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/ir-interceptor.init $(1)/etc/init.d/ir-interceptor
endef

$(eval $(call BuildPackage,ir-interceptor))
