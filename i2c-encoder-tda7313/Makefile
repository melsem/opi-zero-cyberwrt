#
# Copyright (C) 2006-2009 OpenWrt.org
# Copyright 2010 Vertical Communications
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=i2c-encoder-tda7313
PKG_VERSION:=0.2
PKG_RELEASE:=3

include $(INCLUDE_DIR)/package.mk

define Package/i2c-encoder-tda7313
  SECTION:=utils
  CATEGORY:=Utilities
  DEPENDS:=+kmod-i2c-algo-bit +kmod-i2c-core +kmod-i2c-gpio
  TITLE:=i2c encoder tda7313 (C version)
endef

define Package/i2c-encoder-tda7313/description
  The TDA7313 is a volume, tone (bass and treble) balance and fader processor 
  for quality audio applications. Custom multi-band amplification control version written in C.
endef

# Логіка підготовки директорії збірки (створення папки є обов'язковим для SDK)
define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
endef

# Логіка компіляції файлу tda7313.c
define Build/Compile
	$(TARGET_CC) $(TARGET_CFLAGS) $(TARGET_CPPFLAGS) \
		$(SRC_DIR)/tda7313.c -o $(PKG_BUILD_DIR)/tda7313 $(TARGET_LDFLAGS)
endef

define Package/i2c-encoder-tda7313/install
	# Створюємо директорію у віртуальній системі роутера та встановлюємо бінарник
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/tda7313 $(1)/usr/bin/tda7313

	# Копіюємо ваші файли ініціалізації
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) $(SRC_DIR)/tda7313.init $(1)/etc/init.d/tda7313
	
	$(INSTALL_DIR) $(1)/etc/uci-defaults
	$(INSTALL_BIN) $(SRC_DIR)/uci_defaults_tda7313 $(1)/etc/uci-defaults/51-tda7313
endef

$(eval $(call BuildPackage,i2c-encoder-tda7313))

