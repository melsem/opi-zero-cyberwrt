#
# Copyright (C) 2015 OpenWrt
# See LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=wiringOP
PKG_RELEASE:=2.46
DESTDIR=/usr
PREFIX=

PKG_SOURCE_URL:=https://github.com/orangepi-xunlong/wiringOP.git
PKG_SOURCE_PROTO:=git
PKG_SOURCE_DATE:=2021-11-19
PKG_SOURCE_VERSION:=9bc43382780730ce0a7438a7829fea521049f983
PKG_HASH:=e8f03e2d9a2b69e4ea4f5199e8960ae8573dfd5726103ab7836244a374555dd0

PKG_CONFIG_DEPENDS:= \
	CONFIG_KERNEL_DEVMEM \
	CONFIG_KERNEL_DEVKMEM \

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=$(PKG_NAME) for orangepi.
  DEPENDS:=@TARGET_sunxi
endef

define Package/$(PKG_NAME)/description
  Depends:
  'make menuconfig', select 'Global Build Settings' ---> 'Kernel Build Options':
  check "/dev/mem virtual device support" and
  check "/dev/kmem virtual device support"

  -------------------------------
  This is a modified WiringPi for:
	"orangepione-h3" "orangepilite-h3"
	"orangepipc-h3" "orangepiplus-h3"
	"orangepipcplus-h3" "orangepiplus2e-h3"
	"orangepi2giot"
	"orangepipc2-h5"
	"orangepiprime-h5"
	"orangepizeroplus-h5"
	"orangepiwin-a64" "orangepiwinplus-a64"
	"orangepizero-h2" "orangepir1-h2"
	"orangepioneplus-h6" "orangepilite2-h6"
	"orangepi3-h6"
	"orangepizero2-h616"
	"orangepizeroplus2h3"
	"orangepizeroplus2h5"
	"orangepirk3399"
	"orangepi4"
	"orangepi800"
	"orangepir1plus-rk3328"
  (https://github.com/orangepi-xunlong/wiringOP.git)
endef

CFLAGS="$(TARGET_CFLAGS) $(TARGET_CPPFLAGS) -I/$(PKG_BUILD_DIR)/wiringPi/ -I/$(PKG_BUILD_DIR)/devLib/"

SRC_EXAMPLES =	speed blink blink8 blink12			\
		blink12drcs pwm					\
		wfi isr isr-osc					\
		lcd lcd-adafruit clock				\
		nes softPwm softTone 				\
		delayTest serialRead serialTest okLed ds1302	\
		lowPower max31855 rht03				\
		oled_demo spiSpeed spidev_test			\
		w25q64_test watchdog				\
		spidev_test_linux3_4

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
		LIBS="$(TARGET_LDFLAGS) -L$(PKG_BUILD_DIR)/devLib -L$(PKG_BUILD_DIR)/wiringPi -lwiringPi -lwiringPiDev -lpthread -lrt -lm -lcrypt"

	make -C $(PKG_BUILD_DIR)/examples $(SRC_EXAMPLES)			\
		$(TARGET_CONFIGURE_OPTS)					\
		CFLAGS=$(CFLAGS)						\
		LIBS="$(TARGET_LDFLAGS) -L$(PKG_BUILD_DIR)/devLib -L$(PKG_BUILD_DIR)/wiringPi -lwiringPi -lwiringPiDev -lm -lcrypt -lrt"
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
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/examples/speed $(1)$(DESTDIR)$(PREFIX)/bin/

	$(INSTALL_BIN) $(PKG_BUILD_DIR)/examples/serialRead $(1)$(DESTDIR)$(PREFIX)/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/examples/blink $(1)$(DESTDIR)$(PREFIX)/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/examples/lcd $(1)$(DESTDIR)$(PREFIX)/bin/

	$(INSTALL_BIN) $(PKG_BUILD_DIR)/examples/blink8 $(1)$(DESTDIR)$(PREFIX)/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/examples/blink12 $(1)$(DESTDIR)$(PREFIX)/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/examples/blink12drcs $(1)$(DESTDIR)$(PREFIX)/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/examples/pwm $(1)$(DESTDIR)$(PREFIX)/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/examples/wfi $(1)$(DESTDIR)$(PREFIX)/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/examples/isr $(1)$(DESTDIR)$(PREFIX)/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/examples/isr-osc $(1)$(DESTDIR)$(PREFIX)/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/examples/lcd-adafruit $(1)$(DESTDIR)$(PREFIX)/bin/

	$(INSTALL_BIN) $(PKG_BUILD_DIR)/examples/clock $(1)$(DESTDIR)$(PREFIX)/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/examples/nes $(1)$(DESTDIR)$(PREFIX)/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/examples/softPwm $(1)$(DESTDIR)$(PREFIX)/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/examples/softTone $(1)$(DESTDIR)$(PREFIX)/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/examples/delayTest $(1)$(DESTDIR)$(PREFIX)/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/examples/serialTest $(1)$(DESTDIR)$(PREFIX)/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/examples/okLed $(1)$(DESTDIR)$(PREFIX)/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/examples/ds1302 $(1)$(DESTDIR)$(PREFIX)/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/examples/rht03 $(1)$(DESTDIR)$(PREFIX)/bin/

	$(INSTALL_BIN) $(PKG_BUILD_DIR)/examples/lowPower $(1)$(DESTDIR)$(PREFIX)/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/examples/max31855 $(1)$(DESTDIR)$(PREFIX)/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/examples/oled_demo $(1)$(DESTDIR)$(PREFIX)/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/examples/spiSpeed $(1)$(DESTDIR)$(PREFIX)/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/examples/spidev_test $(1)$(DESTDIR)$(PREFIX)/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/examples/w25q64_test $(1)$(DESTDIR)$(PREFIX)/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/examples/watchdog $(1)$(DESTDIR)$(PREFIX)/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/examples/spidev_test_linux3_4 $(1)$(DESTDIR)$(PREFIX)/bin/

	$(INSTALL_DIR) $(1)$(DESTDIR)$(PREFIX)/include
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/wiringPi/*.h $(1)$(DESTDIR)$(PREFIX)/include/
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/devLib/*.h $(1)$(DESTDIR)$(PREFIX)/include/

	$(INSTALL_DIR) $(1)$(DESTDIR)$(PREFIX)/share/man/man1
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/gpio/gpio.1 $(1)$(DESTDIR)$(PREFIX)/share/man/man1/
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
