
include $(TOPDIR)/rules.mk

PKG_NAME:=boot-config
PKG_VERSION:=1.5
PKG_RELEASE:=09.01.2024

PKG_CONFIG_DEPENDS:= \
	CONFIG_overlays \
	CONFIG_overlay_1wire \
	CONFIG_overlay_pps \
	CONFIG_overlay_gpio_keys_user \
	CONFIG_overlay_rotary_encoder \
	CONFIG_overlay_i2c4 \
	CONFIG_boot-config-luci-web-control \
	CONFIG_boot-config-cyberwrt-web-control

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=$(PKG_NAME) for OpenWrt.
  DEPENDS:=@TARGET_sunxi +mount-utils +block-mount +dtc
  MENU:=1
endef

define Package/$(PKG_NAME)/description
 Utility for editing boot configuration.
 Sunxi Allwiner sun8i-h3, sun50i-h5.

 Enable overlay editor (enable ready-made examples, add a new one).
endef

define Package/$(PKG_NAME)/config
	# Invisible config dependency
	source "$(SOURCE)/Config.in"
endef


ifeq ($(CONFIG_overlays),y)
  EXTRA_CFLAGS += -DCONFIG_overlays
endif
ifeq ($(CONFIG_overlay_1wire),y)
  EXTRA_CFLAGS += -DCONFIG_overlay_1wire
endif
ifeq ($(CONFIG_overlay_pps),y)
  EXTRA_CFLAGS += -DCONFIG_overlay_pps
endif
ifeq ($(CONFIG_overlay_gpio_keys_user),y)
  EXTRA_CFLAGS += -DCONFIG_overlay_gpio_keys_user
endif
ifeq ($(CONFIG_overlay_rotary_encoder),y)
  EXTRA_CFLAGS += -DCONFIG_overlay_rotary_encoder
endif
ifeq ($(CONFIG_overlay_i2c4),y)
  EXTRA_CFLAGS += -DCONFIG_overlay_i2c4
endif


ifeq ($(CONFIG_boot-config-cyberwrt-web-control),y)
define Package/$(PKG_NAME)/postrm
#!/bin/sh
sed -i "/boot-config\/index.cgi/d" /www/menu.html &> /dev/null
rm -R /www/cgi-bin/modules/boot-config &> /dev/null
rm -fr /tmp/luci-indexcache /tmp/luci-modulecache &> /dev/null
endef
endif

CFLAGS=$(EXTRA_CFLAGS)

define Build/Compile
	$(TARGET_CC) \
	$(TARGET_CFLAGS) \
	$(EXTRA_CFLAGS) \
	-c -o $(PKG_BUILD_DIR)/$(PKG_NAME).o \
	$(PKG_BUILD_DIR)/$(PKG_NAME).c

	$(TARGET_CC) $(TARGET_LDFLAGS) -o $(PKG_BUILD_DIR)/$(PKG_NAME) $(PKG_BUILD_DIR)/$(PKG_NAME).o
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/$(PKG_NAME) $(1)/usr/sbin/

#ifeq ($(CONFIG_overlays),y)
	$(INSTALL_DIR) $(1)/etc/dt-overlays
	$(INSTALL_DATA) ./src/dt-overlays/* $(1)/etc/dt-overlays/
#endif

ifeq ($(CONFIG_boot-config-luci-web-control),y)
	$(INSTALL_DIR) $(1)/usr/lib/lua/luci
	cp -pR ./luasrc/* $(1)/usr/lib/lua/luci/
endif

	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./src/cpufreq_scaling $(1)/etc/init.d/
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_BIN) ./src/cpufreq_scaling.conf $(1)/etc/config/cpufreq_scaling
	$(INSTALL_DIR) $(1)/etc/uci-defaults
	$(INSTALL_BIN) ./src/uci_defaults_$(PKG_NAME) $(1)/etc/uci-defaults/50-$(PKG_NAME)
	$(INSTALL_BIN) ./src/add-mount-boot $(1)/etc/uci-defaults/20-$(PKG_NAME)
	$(INSTALL_BIN) ./src/uci_defaults_usb_otg $(1)/etc/uci-defaults/100-usb_otg

ifeq (($(CONFIG_boot-config-cyberwrt-web-control),y) || ($(CONFIG_boot-config-luci-web-control),y))
	$(INSTALL_DIR) $(1)/www/boot
	$(INSTALL_BIN) ./files/boot.html $(1)/www/boot/index.html
	$(INSTALL_DIR) $(1)/www/cgi-bin/modules/$(PKG_NAME)
	$(INSTALL_BIN) ./files/*.cgi $(1)/www/cgi-bin/modules/$(PKG_NAME)/
	$(INSTALL_BIN) ./files/luci.htm $(1)/www/cgi-bin/modules/$(PKG_NAME)/
	$(INSTALL_BIN) ./files/index.html $(1)/www/cgi-bin/modules/$(PKG_NAME)/
endif

endef

$(eval $(call BuildPackage,$(PKG_NAME)))
