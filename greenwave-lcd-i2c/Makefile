include $(TOPDIR)/rules.mk

PKG_NAME:=greenwave-lcd-i2c
PKG_RELEASE:=1
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=LCD-I2C Display Driver
  DEPENDS:=+libuci
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/greenwave-lcd-i2c $(1)/usr/bin/

	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_DATA) ./files/greenwave-lcd-i2c.config $(1)/etc/config/greenwave-lcd-i2c

	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/greenwave-lcd-i2c.init $(1)/etc/init.d/
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
