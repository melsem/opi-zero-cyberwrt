#
# Copyright (C) 2006-2009 OpenWrt.org
# Copyright 2010 Vertical Communications
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=i2c-encoder-tda7313
PKG_VERSION:=0.1
PKG_RELEASE:=26.02.2021

include $(INCLUDE_DIR)/package.mk

define Package/i2c-encoder-tda7313
  SECTION:=utils
  CATEGORY:=Utilities
  DEPENDS:=+i2c-tools +kmod-i2c-algo-bit +kmod-i2c-core +kmod-i2c-gpio
  TITLE:=i2c encoder tda7313
endef

define Package/i2c-encoder-tda7313/description
 The TDA7313 is a volume, tone (bass and  treble)balance (Left/Right)
 and fader (front/rear) processorfor quality audio applications in car radio and Hi-Fi systems.
 Selectable input gain and external loudness functionare provided.
 Control is accomplished by serial I2Cbus microprocessor interface.
 The AC signal setting is obtained by resistor networksand switches combined with operationalamplifiers.
 Thanks to the used BIPOLAR/CMOS Tecnology,Low Distortion, Low Noise and Low DC stepping areobtained.
endef

define Build/Compile
endef

define Package/i2c-encoder-tda7313/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) ./src/tda7313 $(1)/usr/sbin/
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./src/tda7313.init $(1)/etc/init.d/tda7313
	$(INSTALL_DIR) $(1)/etc/uci-defaults
	$(INSTALL_BIN) ./src/uci_defaults_tda7313 $(1)/etc/uci-defaults/51-tda7313
endef

$(eval $(call BuildPackage,i2c-encoder-tda7313))

