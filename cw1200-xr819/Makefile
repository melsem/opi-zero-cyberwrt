include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=cw1200-xr819
PKG_VERSION:=1.0
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(KERNEL_BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Download/xr819
  FILE:=xr819.tar.bz2
  URL:=https://github.com/melsem/openwrt-lede_xradio-xr819_soc-audio/raw/master/firmware/
  HASH:=bc9bae512267e04b6d00ffe18fe68a4181c9a5af35f765dfbabdeedd28c8f4f7
endef
$(eval $(call Download,xr819))

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) $(shell ls -d $(KERNEL_BUILD_DIR)/backports-*/drivers/net/wireless/st/cw1200)/* $(PKG_BUILD_DIR)/
	$(Build/Patch)
endef

define KernelPackage/$(PKG_NAME)
  SUBMENU:=Wireless Drivers
  TITLE:=ST-Ericsson CW1200 / XRADIO XR819 Driver
  DEPENDS:=@TARGET_sunxi +kmod-mac80211 +wpad-mini
  FILES:= \
	$(PKG_BUILD_DIR)/cw1200_core.ko \
	$(PKG_BUILD_DIR)/cw1200_wlan_sdio.ko
  AUTOLOAD:=$(call AutoLoad,55,cw1200_core cw1200_wlan_sdio)
endef

define KernelPackage/$(PKG_NAME)/description
  External kernel module for ST-Ericsson CW1200 and Xradio XR819 SDIO chips
  compiled with mac80211-backport headers and localized package patches.
endef

NOSTDINC_FLAGS:= \
	-I$(PKG_BUILD_DIR) \
	-I$(STAGING_DIR)/usr/include/mac80211-backport/uapi \
	-I$(STAGING_DIR)/usr/include/mac80211-backport \
	-I$(STAGING_DIR)/usr/include/mac80211/uapi \
	-I$(STAGING_DIR)/usr/include/mac80211 \
	-include $(STAGING_DIR)/usr/include/mac80211-backport/backport/autoconf.h \
	-include $(STAGING_DIR)/usr/include/mac80211-backport/backport/backport.h

define Build/Compile
	+$(MAKE) $(PKG_JOBS) -C "$(LINUX_DIR)" \
		$(KERNEL_MAKE_FLAGS) \
		$(PKG_MAKE_FLAGS) \
		M="$(PKG_BUILD_DIR)" \
		NOSTDINC_FLAGS="$(NOSTDINC_FLAGS)" \
		CPTCFG_CW1200=m \
		CPTCFG_CW1200_WLAN_SDIO=m \
		modules
endef

define KernelPackage/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/lib/firmware/xr819
	$(TAR) -C $(1)/lib/firmware -xjf $(DL_DIR)/xr819.tar.bz2
endef

$(eval $(call KernelPackage,$(PKG_NAME)))
