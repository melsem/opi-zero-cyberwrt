include $(TOPDIR)/rules.mk

PKG_NAME:=rtl8189es
PKG_RELEASE=1

PKG_LICENSE:=GPLv2
PKG_LICENSE_FILES:=

PKG_SOURCE_URL:=https://github.com/melsem/rtl8189es.git
PKG_SOURCE_PROTO:=git
PKG_SOURCE_DATE:=2020-03-24
PKG_SOURCE_VERSION:=b7049b0ee50b1e82307a75a6f3e1e3d6111de7e8
CRLF_WORKAROUND:=1

PKG_MAINTAINER:=Hauke Mehrtens <hauke@hauke-m.de>

STAMP_CONFIGURED_DEPENDS := $(STAGING_DIR)/usr/include/mac80211-backport/backport/autoconf.h

include $(INCLUDE_DIR)/kernel.mk
include $(INCLUDE_DIR)/package.mk

define KernelPackage/rtl8189es
  SUBMENU:=Wireless Drivers
  TITLE:=Realtek RTL8189ES / RTL8189ETV SDIO WiFi
  DEPENDS:=+kmod-cfg80211 +kmod-mmc +@DRIVER_11N_SUPPORT @!LINUX_4_19
  FILES:=$(PKG_BUILD_DIR)/8189es.ko
  AUTOLOAD:=$(call AutoLoad,50,cfg80211 8189es)
endef

NOSTDINC_FLAGS = \
	-I$(STAGING_DIR)/usr/include/mac80211-backport/uapi \
	-I$(STAGING_DIR)/usr/include/mac80211-backport \
	-I$(STAGING_DIR)/usr/include/mac80211/uapi \
	-I$(STAGING_DIR)/usr/include/mac80211 \
	-include backport/autoconf.h \
	-include backport/backport.h

define Build/Compile
	+$(MAKE) $(PKG_JOBS) -C $(LINUX_DIR) \
		ARCH=$(LINUX_KARCH) \
		CROSS_COMPILE=$(TARGET_CROSS) \
		SUBDIRS="$(PKG_BUILD_DIR)" \
		NOSTDINC_FLAGS="$(NOSTDINC_FLAGS)" \
		CONFIG_RTL8189ES=m \
		modules
endef

$(eval $(call KernelPackage,rtl8189es))
