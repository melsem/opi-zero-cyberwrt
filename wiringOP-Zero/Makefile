#
# Copyright (C) 2015 OpenWrt
# See LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=wiringOP-Zero
PKG_RELEASE:=2.6
DESTDIR=/usr
PREFIX=


PKG_SOURCE_DATE:=2020-03-21
PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=https://github.com/melsem/WiringOP-Zero.git
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_SOURCE_DATE)
PKG_SOURCE_VERSION:=a0305aae10c47a2214a8ce7beefab8561b17639c
#PKG_SOURCE_VERSION:=2f4246c33af114e899aec7c7aad691468882dabb
PKG_SOURCE:=$(PKG_SOURCE_SUBDIR).tar.xz
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_SOURCE_SUBDIR)

PKG_CONFIG_DEPENDS:= \
	CONFIG_KERNEL_DEVMEM \
	CONFIG_KERNEL_DEVKMEM \
	CONFIG_EXAMPLES \
	CONFIG_SHARE_MAN1 \
	CONFIG_INCLUDE_LIB_WIRINGOP \
	CONFIG_ORANGEPI_ZERO_H2_PLUS \
	CONFIG_ORANGEPI_R1_H2_PLUS \
	CONFIG_ORANGEPI_PLUS_H3 \
	CONFIG_ORANGEPI_ONE_H3 \
	CONFIG_ORANGEPI_LITE_H3 \
	CONFIG_ORANGEPI_PC_H3 \
	CONFIG_ORANGEPI_PC_PLUS_H3 \
	CONFIG_ORANGEPI_PLUS_2E_H3 \
	CONFIG_ORANGEPI_ZERO_PLUS2_H3 \
	CONFIG_ORANGEPI_ZERO_PLUS2_H5 \
	CONFIG_ORANGEPI_LITE_2 \
	CONFIG_ORANGE_PI_WIN \
	CONFIG_ORANGEPI_ZERO_PLUS \
	CONFIG_ORANGEPI_PC2 \
	CONFIG_ORANGEPI_PRIME \
	CONFIG_ORANGEPI_3

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=$(PKG_NAME) for orangepi.
  DEPENDS:=@TARGET_sunxi
  MENU:=1
endef

define Package/$(PKG_NAME)/description
  Depends: 
  'make menuconfig', select 'Global Build Settings' ---> 'Kernel Build Options':
  check "/dev/mem virtual device support" and
  check "/dev/kmem virtual device support"

  -------------------------------
  This is a modified WiringPi for:
	"ORANGEPI_ZERO_H2+"
	"ORANGEPI_R1_H2+"
	"ORANGEPI_PLUS_H3"
	"ORANGEPI_ONE_H3"
	"ORANGEPI_LITE_H3"
	"ORANGEPI_PC_H3"
	"ORANGEPI_PC_PLUS_H3"
	"ORANGEPI_PLUS_2E_H3"
	"ORANGEPI_ZERO_PLUS2_H3"
	"ORANGEPI_ZERO_PLUS2_H5"
	"ORANGEPI_LITE_2"
	"ORANGE_PI_WIN"
	"ORANGEPI_ZERO_PLUS"
	"ORANGEPI_PC2"
	"ORANGEPI_PRIME"
	"ORANGEPI_3"
  -------------------------------

  https://github.com/sanjuruk/WiringOP-Zero
#  https://github.com/sunzone93/WiringOP-Zero
endef

define Package/$(PKG_NAME)/config
	source "$(SOURCE)/Config.in"
endef

#################
ifeq ($(CONFIG_ORANGEPI_ZERO_H2_PLUS),y)
EXTRA_CFLAGS = -DCONFIG_ORANGEPI_ZERO
else ifeq ($(CONFIG_ORANGEPI_R1_H2_PLUS),y)
EXTRA_CFLAGS = -DCONFIG_ORANGEPI_ZERO

else ifeq ($(CONFIG_ORANGEPI_PLUS_H3),y)
EXTRA_CFLAGS = -DCONFIG_ORANGEPI_H3
else ifeq ($(CONFIG_ORANGEPI_ONE_H3),y)
EXTRA_CFLAGS = -DCONFIG_ORANGEPI_H3
else ifeq ($(CONFIG_ORANGEPI_LITE_H3),y)
EXTRA_CFLAGS = -DCONFIG_ORANGEPI_H3
else ifeq ($(CONFIG_ORANGEPI_PC_H3),y)
EXTRA_CFLAGS = -DCONFIG_ORANGEPI_H3
else ifeq ($(CONFIG_ORANGEPI_PC_PLUS_H3),y)
EXTRA_CFLAGS = -DCONFIG_ORANGEPI_H3
else ifeq ($(CONFIG_ORANGEPI_PLUS_2E_H3),y)
EXTRA_CFLAGS = -DCONFIG_ORANGEPI_H3
else ifeq ($(CONFIG_ORANGEPI_PLUS_2_H3),y)
EXTRA_CFLAGS = -DCONFIG_ORANGEPI_H3

else ifeq ($(CONFIG_ORANGEPI_ZERO_PLUS2_H3),y)
EXTRA_CFLAGS = -DCONFIG_ORANGEPI_ZEROPLUS2_H3
else ifeq ($(CONFIG_ORANGEPI_ZERO_PLUS2_H5),y)
EXTRA_CFLAGS = -DCONFIG_ORANGEPI_ZEROPLUS2_H3

else ifeq ($(CONFIG_ORANGEPI_LITE_2),y)
EXTRA_CFLAGS = -DCONFIG_ORANGEPI_LITE2

else ifeq ($(CONFIG_ORANGE_PI_WIN),y)
EXTRA_CFLAGS = -DCONFIG_ORANGEPI_WIN

else ifeq ($(CONFIG_ORANGEPI_ZERO_PLUS),y)
EXTRA_CFLAGS = -DCONFIG_ORANGEPI_ZERO

else ifeq ($(CONFIG_ORANGEPI_PC2),y)
EXTRA_CFLAGS = -DCONFIG_ORANGEPI_PC2

else ifeq ($(CONFIG_ORANGEPI_PRIME),y)
EXTRA_CFLAGS = -DCONFIG_ORANGEPI_PRIME

else ifeq ($(CONFIG_ORANGEPI_3),y)
EXTRA_CFLAGS = -DCONFIG_ORANGEPI_3

endif
#################

#define Build/Prepare
#	mkdir -p $(PKG_BUILD_DIR)
#	$(CP) ./* $(PKG_BUILD_DIR)/
#endef

CFLAGS="$(TARGET_CFLAGS) $(TARGET_CPPFLAGS) -I/$(PKG_BUILD_DIR)/wiringPi/ \
			-I/$(PKG_BUILD_DIR)/devLib/ $(EXTRA_CFLAGS)"

SRC_EXAMPLES =	blink blink8 blink12					\
		blink12drcs						\
		pwm							\
		speed wfi isr isr-osc					\
		lcd lcd-adafruit clock					\
		nes							\
		softPwm softTone 					\
		delayTest serialRead serialTest okLed ds1302		\
		rht03 7segments

define Build/Compile
	make -C $(PKG_BUILD_DIR)/wiringPi	\
		$(TARGET_CONFIGURE_OPTS)	\
		CFLAGS=$(CFLAGS)		\
		LIBS="$(TARGET_LDFLAGS)"
	ln -sf $(PKG_BUILD_DIR)/wiringPi/libwiringPi.so.$(PKG_RELEASE) $(PKG_BUILD_DIR)/wiringPi/libwiringPi.so

	make -C $(PKG_BUILD_DIR)/devLib		\
		$(TARGET_CONFIGURE_OPTS)	\
		CFLAGS=$(CFLAGS)		\
		LIBS="$(TARGET_LDFLAGS)"
	ln -sf $(PKG_BUILD_DIR)/devLib/libwiringPiDev.so.$(PKG_RELEASE) $(PKG_BUILD_DIR)/devLib/libwiringPiDev.so

	make -C $(PKG_BUILD_DIR)/gpio		\
		$(TARGET_CONFIGURE_OPTS)	\
		CFLAGS=$(CFLAGS)		\
		LIBS="$(TARGET_LDFLAGS) -L$(PKG_BUILD_DIR)/devLib -L$(PKG_BUILD_DIR)/wiringPi -lwiringPi -lwiringPiDev -lpthread -lm"

	make -C $(PKG_BUILD_DIR)/examples $(SRC_EXAMPLES)			\
		$(TARGET_CONFIGURE_OPTS)					\
		CFLAGS=$(CFLAGS)						\
		LIBS="$(TARGET_LDFLAGS) -L$(PKG_BUILD_DIR)/devLib -L$(PKG_BUILD_DIR)/wiringPi -lwiringPi -lwiringPiDev -lpthread -lm"

endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)$(DESTDIR)/lib
	$(INSTALL_DIR) $(1)$(DESTDIR)$(PREFIX)/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/wiringPi/libwiringPi.so.$(PKG_RELEASE) $(1)$(DESTDIR)$(PREFIX)/lib/
	ln -sf $(DESTDIR)$(PREFIX)/lib/libwiringPi.so.$(PKG_RELEASE)	$(1)$(DESTDIR)/lib/libwiringPi.so
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/devLib/libwiringPiDev.so.$(PKG_RELEASE) $(1)$(DESTDIR)$(PREFIX)/lib/
	ln -sf $(DESTDIR)$(PREFIX)/lib/libwiringPiDev.so.$(PKG_RELEASE)	$(1)$(DESTDIR)/lib/libwiringPiDev.so

	$(INSTALL_DIR) $(1)$(DESTDIR)$(PREFIX)/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/gpio/gpio $(1)$(DESTDIR)$(PREFIX)/bin/

ifeq ($(CONFIG_EXAMPLES),y)
	$(INSTALL_DIR) $(1)$(DESTDIR)$(PREFIX)/examples
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/examples/speed $(1)$(DESTDIR)$(PREFIX)/examples/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/examples/7segments $(1)$(DESTDIR)$(PREFIX)/examples/

	$(INSTALL_BIN) $(PKG_BUILD_DIR)/examples/serialRead $(1)$(DESTDIR)$(PREFIX)/examples/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/examples/blink $(1)$(DESTDIR)$(PREFIX)/examples/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/examples/lcd $(1)$(DESTDIR)$(PREFIX)/examples/

	$(INSTALL_BIN) $(PKG_BUILD_DIR)/examples/blink8 $(1)$(DESTDIR)$(PREFIX)/examples/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/examples/blink12 $(1)$(DESTDIR)$(PREFIX)/examples/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/examples/blink12drcs $(1)$(DESTDIR)$(PREFIX)/examples/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/examples/pwm $(1)$(DESTDIR)$(PREFIX)/examples/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/examples/wfi $(1)$(DESTDIR)$(PREFIX)/examples/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/examples/isr $(1)$(DESTDIR)$(PREFIX)/examples/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/examples/isr-osc $(1)$(DESTDIR)$(PREFIX)/examples/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/examples/lcd-adafruit $(1)$(DESTDIR)$(PREFIX)/examples/

	$(INSTALL_BIN) $(PKG_BUILD_DIR)/examples/clock $(1)$(DESTDIR)$(PREFIX)/examples/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/examples/nes $(1)$(DESTDIR)$(PREFIX)/examples/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/examples/softPwm $(1)$(DESTDIR)$(PREFIX)/examples/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/examples/softTone $(1)$(DESTDIR)$(PREFIX)/examples/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/examples/delayTest $(1)$(DESTDIR)$(PREFIX)/examples/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/examples/serialTest $(1)$(DESTDIR)$(PREFIX)/examples/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/examples/okLed $(1)$(DESTDIR)$(PREFIX)/examples/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/examples/ds1302 $(1)$(DESTDIR)$(PREFIX)/examples/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/examples/rht03 $(1)$(DESTDIR)$(PREFIX)/examples/

#	$(INSTALL_BIN) $(PKG_BUILD_DIR)/examples/lowPower $(1)$(DESTDIR)$(PREFIX)/examples/
#	$(INSTALL_BIN) $(PKG_BUILD_DIR)/examples/max31855 $(1)$(DESTDIR)$(PREFIX)/examples/
#	$(INSTALL_BIN) $(PKG_BUILD_DIR)/examples/oled_demo $(1)$(DESTDIR)$(PREFIX)/examples/
#	$(INSTALL_BIN) $(PKG_BUILD_DIR)/examples/spiSpeed $(1)$(DESTDIR)$(PREFIX)/examples/
#	$(INSTALL_BIN) $(PKG_BUILD_DIR)/examples/spidev_test $(1)$(DESTDIR)$(PREFIX)/examples/
#	$(INSTALL_BIN) $(PKG_BUILD_DIR)/examples/w25q64_test $(1)$(DESTDIR)$(PREFIX)/examples/
#	$(INSTALL_BIN) $(PKG_BUILD_DIR)/examples/watchdog $(1)$(DESTDIR)$(PREFIX)/examples/
#	$(INSTALL_BIN) $(PKG_BUILD_DIR)/examples/spidev_test_linux3_4 $(1)$(DESTDIR)$(PREFIX)/examples/
endif

ifeq ($(CONFIG_INCLUDE_LIB_WIRINGOP),y)
	$(INSTALL_DIR) $(1)$(DESTDIR)$(PREFIX)/include
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/wiringPi/*.h $(1)$(DESTDIR)$(PREFIX)/include/
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/devLib/*.h $(1)$(DESTDIR)$(PREFIX)/include/
endif

ifeq ($(CONFIG_SHARE_MAN1),y)
	$(INSTALL_DIR) $(1)$(DESTDIR)$(PREFIX)/share/man/man1
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/gpio/gpio.1 $(1)$(DESTDIR)$(PREFIX)/share/man/man1/
endif
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
