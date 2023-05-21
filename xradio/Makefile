include $(TOPDIR)/rules.mk

PKG_NAME:=xradio
PKG_RELEASE=1

PKG_LICENSE:=GPLv2
PKG_LICENSE_FILES:=

PKG_SOURCE_URL:=https://github.com/fifteenhex/xradio.git
PKG_SOURCE_PROTO:=git
PKG_SOURCE_DATE:=2023-05-10
PKG_SOURCE_VERSION:=a2dd9cd8b1a8b8a8af8aaff90dfaa65fcf783e26
PKG_HASH:=3ead40075330178d0e1840fa01d382629001bfe83748f6ee3d51d1c99e0da4c4

PKG_BUILD_PARALLEL:=1

include $(INCLUDE_DIR)/kernel.mk
include $(INCLUDE_DIR)/package.mk

define Download/xr819
  FILE:=xr819.tar.bz2
  URL:=https://github.com/melsem/openwrt-lede_xradio-xr819_soc-audio/raw/master/firmware/
  HASH:=bc9bae512267e04b6d00ffe18fe68a4181c9a5af35f765dfbabdeedd28c8f4f7
endef
$(eval $(call Download,xr819))

define KernelPackage/xradio
  SUBMENU:=Wireless Drivers Test-xr819
  TITLE:=Xradio xr819 test wifi-drivers support
  DEPENDS:=@TARGET_sunxi +kmod-mac80211 +wpad-mini
  KCONFIG:= \
	CONFIG_WLAN_VENDOR_XRADIO=m \
	CONFIG_XRADIO_USE_EXTENSIONS=y
  FILES:=$(PKG_BUILD_DIR)/xradio_wlan.ko
  AUTOLOAD:=$(call AutoProbe,xradio_wlan)
endef

NOSTDINC_FLAGS = \
	-I$(PKG_BUILD_DIR) \
	-I$(STAGING_DIR)/usr/include/mac80211-backport/uapi \
	-I$(STAGING_DIR)/usr/include/mac80211-backport \
	-I$(STAGING_DIR)/usr/include/mac80211/uapi \
	-I$(STAGING_DIR)/usr/include/mac80211 \
	-include backport/autoconf.h \
	-include backport/backport.h

define Build/Compile
	+$(MAKE) $(PKG_JOBS) -C "$(LINUX_DIR)" \
		$(KERNEL_MAKE_FLAGS) \
		$(PKG_MAKE_FLAGS) \
		M="$(PKG_BUILD_DIR)" \
		NOSTDINC_FLAGS="$(NOSTDINC_FLAGS)" \
		modules
endef

define KernelPackage/xradio/install
	$(INSTALL_DIR) $(1)/lib/firmware/xr819
	$(TAR) -C $(1)/lib/firmware -xjf $(DL_DIR)/xr819.tar.bz2
endef

$(eval $(call KernelPackage,xradio))
