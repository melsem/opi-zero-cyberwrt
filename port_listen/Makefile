include $(TOPDIR)/rules.mk

PKG_NAME:=port_listen
PKG_VERSION:=19.02.2018
PKG_RELEASE:=test

PKG_BUILD_DIR:= $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/port_listen
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=port_listen
#  DEPENDS:=+kmod-usb-serial-ch341 +kmod-usb-serial-pl2303
endef

define Package/port_listen/description
 port_listen << read and setup port >>
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(TARGET_CC) $(TARGET_CFLAGS) -c -o $(PKG_BUILD_DIR)/port_listen.o $(PKG_BUILD_DIR)/port_listen.c
	$(TARGET_CC) $(TARGET_LDFLAGS) -o $(PKG_BUILD_DIR)/port_listen $(PKG_BUILD_DIR)/port_listen.o
endef

define Package/port_listen/install
	$(INSTALL_DIR) $(1)/usr/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/port_listen $(1)/usr/sbin/

	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/port_listen.init $(1)/etc/init.d/

	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_DATA) ./files/port_listen.conf $(1)/etc/config/port_listen
endef

$(eval $(call BuildPackage,port_listen))
