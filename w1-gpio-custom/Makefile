include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=w1-gpio-custom
PKG_RELEASE:=3.0

include $(INCLUDE_DIR)/package.mk

define KernelPackage/$(PKG_NAME)
  SUBMENU:=W1 support
  TITLE:=Custom GPIO-based W1 driver setup on Kernel 5.15+, 6.x+
#  DEPENDS:=@LINUX_5_4 +kmod-w1 +kmod-w1-master-gpio
  DEPENDS:=@(LINUX_5_4||LINUX_5_10||LINUX_5_15||LINUX_6_1||LINUX_6_6) \
	+kmod-w1 +kmod-w1-master-gpio
  FILES:=$(PKG_BUILD_DIR)/$(PKG_NAME).ko
endef

define KernelPackage/$(PKG_NAME)/description
 Universal custom GPIO-based 1-Wire driver version 3.0.
 Uses modern Software Nodes API. Fully compatible with Kernel 5.15+, 6.x+.
 Eliminates internal gpiolib.h dependencies to prevent Kernel Panic.
endef

# Копіюємо вихідний код у робочу директорію збірки
define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

#EXTRA_CFLAGS+= -I$(LINUX_DIR)/drivers/gpio

define Build/Compile
	$(KERNEL_MAKE) M="$(PKG_BUILD_DIR)" modules
#		EXTRA_CFLAGS="$(EXTRA_CFLAGS)" 
endef

define KernelPackage/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_CONF) ./files/$(PKG_NAME).conf $(1)/etc/config/$(PKG_NAME)

	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/$(PKG_NAME).init $(1)/etc/init.d/$(PKG_NAME)
endef

define Package/$(PKG_NAME)/postinst
#!/bin/sh
# Проверяем, что скрипт выполняется на живом устройстве, а не внутри Image Builder
if [ -z "$${IPKG_INSTROOT}" ]; then
    rm -f /etc/rc.d/S30$(PKG_NAME)
    #echo "Disabling automatic startup for $(PKG_NAME)..."
    # Удаляем симлинки автозапуска, созданные пакетным менеджером по умолчанию
    /etc/init.d/$(PKG_NAME) disable
fi
exit 0
endef

$(eval $(call KernelPackage,$(PKG_NAME)))

