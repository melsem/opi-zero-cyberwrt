#
# Copyright (C) 2007 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=lcd-hd44780
PKG_VERSION:=1.3
PKG_RELEASE:=4

# Использование KERNEL_BUILD_DIR гарантирует изоляцию под конкретную версию ядра
PKG_BUILD_DIR:=$(KERNEL_BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define KernelPackage/$(PKG_NAME)
  SUBMENU:=Other modules
  DEPENDS:=
  TITLE:=Kernel driver for hd44780-based LCD display
  FILES:=$(PKG_BUILD_DIR)/hd44780_mod.$(LINUX_KMOD_SUFFIX)
  VERSION:=$(LINUX_VERSION)+$(PKG_VERSION)-$(PKG_RELEASE)
#  AUTOLOAD:=$(call AutoLoad,90,hd44780_mod)
endef 

define KernelPackage/$(PKG_NAME)/description
	Kernel driver for access to hd44780-based display via GPIO (Compatible with Kernels 5.x and 6.x)
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

# Совместимый блок компиляции.
# Если макрос KERNEL_MAKE не определен в старой OpenWRT, конструируем вызов вручную.
ifdef KERNEL_MAKE
  define Build/Compile
	$(KERNEL_MAKE) M="$(PKG_BUILD_DIR)" modules
  endef
else
  define Build/Compile
	$(MAKE) -C "$(LINUX_DIR)" \
		CROSS_COMPILE="$(TARGET_CROSS)" \
		ARCH="$(LINUX_KARCH)" \
		PATH="$(TARGET_PATH)" \
		M="$(PKG_BUILD_DIR)" \
		modules
  endef
endif

# Установка конфигурационных файлов интерфейса UCI и скриптов инициализации
# Установка конфигурационных файлов интерфейса UCI и скриптов инициализации
define KernelPackage/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_DATA) ./files/hd44780.conf $(1)/etc/config/hd44780
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/hd44780.init $(1)/etc/init.d/hd44780
endef


$(eval $(call KernelPackage,$(PKG_NAME)))

