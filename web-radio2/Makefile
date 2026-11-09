include $(TOPDIR)/rules.mk

PKG_NAME:=web-radio2
PKG_VERSION:=1.7
PKG_RELEASE:=2.0

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=Web Radio Player for OpenWrt
  DEPENDS:=+kmod-sound-core +kmod-usb-audio +madplay +kmod-usb-ohci +kmod-usb2 +curl +libcurl
  DEPENDS+= +CONFIG_DEPENDS_TRIGGERHAPPY:v4l-utils \
            +CONFIG_DEPENDS_TRIGGERHAPPY:triggerhappy \
            +CONFIG_DEPENDS_TARGET_SUNXI:kmod-sunxi-ir
endef

define Package/$(PKG_NAME)/description
  Optimized internet radio player for OpenWrt (Kernels 5.15 - 6.x).
  Features network drop protection and atomic m3u parsing.
endef

define Package/$(PKG_NAME)/config
  menu "Web-Radio2 Options"
    depends on PACKAGE_$(PKG_NAME)
	choice
		prompt "Select Web Variant"
		default WEB_RADIO2_MODERN
		depends on PACKAGE_$(PKG_NAME)

		config WEB_RADIO2_CLASSIC
			bool "Install web File classic (Standard)"

		config WEB_RADIO2_MODERN
			bool "Install web File modern (Alternative)"
	endchoice

	comment "--- Extra Modules for Web-Radio2 ---"
		depends on PACKAGE_$(PKG_NAME)

	config DEPENDS_TRIGGERHAPPY
		bool "Enable remote control and button support (Triggerhappy)"
		default n
		depends on PACKAGE_$(PKG_NAME)

	config DEPENDS_TARGET_SUNXI
		bool "Optimize for Allwinner (Orange Pi / Sunxi IR)"
		default n
		depends on PACKAGE_$(PKG_NAME) && TARGET_sunxi

	config TRIGGERHAPPY_CONF
		bool "Install Triggerhappy.conf"
		default y
  endmenu
endef

define Package/$(PKG_NAME)/postrm
#!/bin/sh
    # Безопасное удаление кнопки WPS, которая была создана для web-radio
    # Находим индекс кнопки, у которой handler содержит упоминание madplay или curl
    idx=0
    while true; do
        handler=$$(uci -q get system.@button[$$idx].handler)
        [ -z "$$handler" ] && break
        
        if echo "$$handler" | grep -q "madplay"; then
            uci delete system.@button[$$idx]
            uci commit system
            # Не увеличиваем idx, так как элементы сместились, проверяем этот же индекс снова
            continue
        fi
        idx=$$((idx + 1))
    done

    # Очистка веб-интерфейса
    sed -i "/web-radio2\/index.cgi/d" /www/menu.html 2> /dev/null || true
    rm -rf /www/cgi-bin/modules/web-radio2 2> /dev/null || true
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		CC="$(TARGET_CC)" \
		CFLAGS="$(TARGET_CFLAGS) -Wall" \
		LDFLAGS="$(TARGET_LDFLAGS)"
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/$(PKG_NAME) $(1)/usr/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/ch-buffer $(1)/usr/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/v-buffer $(1)/usr/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/rc-radio $(1)/usr/sbin/
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./src/$(PKG_NAME).init $(1)/etc/init.d/
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_DATA) ./src/$(PKG_NAME).playlist $(1)/etc/config/$(PKG_NAME)
	
	$(INSTALL_DIR) $(1)/etc/uci-defaults
	$(INSTALL_BIN) ./src/uci_defaults_$(PKG_NAME) $(1)/etc/uci-defaults/50-$(PKG_NAME)
	$(INSTALL_BIN) ./src/uci_defaults_triggerhappy $(1)/etc/uci-defaults/52-$(PKG_NAME)
	
#	 --- ДИНАМІЧНЕ ВСТАНОВЛЕННЯ СКРИПТА TRIGGERHAPPY ---
#	$(if $(CONFIG_TRIGGERHAPPY_CONF), \
#		$(INSTALL_BIN) ./src/uci_defaults_triggerhappy $(1)/etc/uci-defaults/52-$(PKG_NAME); \
#	)

	$(INSTALL_DIR) $(1)/etc/hotplug.d/button
	$(INSTALL_BIN) ./src/00-button $(1)/etc/hotplug.d/button/
	
	# --- ЛОГІКА ВИБОРУ ФАЙЛУ НА ОСНОВІ CONFIG ---
	$(if $(CONFIG_WEB_RADIO2_CLASSIC), \
		rm -rf $(1)/www/cgi-bin/modules/$(PKG_NAME); \
		$(INSTALL_DIR) $(1)/www/modules/$(PKG_NAME); \
		$(INSTALL_DATA) ./files/b.jpg $(1)/www/modules/$(PKG_NAME)/; \
		$(INSTALL_DATA) ./files/r.jpg $(1)/www/modules/$(PKG_NAME)/; \
		$(INSTALL_DATA) ./files/del.png $(1)/www/modules/$(PKG_NAME)/; \
		$(INSTALL_DIR) $(1)/www/cgi-bin/modules/$(PKG_NAME); \
		$(INSTALL_BIN) ./files/classic_index.cgi $(1)/www/cgi-bin/modules/$(PKG_NAME)/index.cgi; \
		$(INSTALL_BIN) ./files/save.cgi $(1)/www/cgi-bin/modules/$(PKG_NAME)/; \
		$(INSTALL_BIN) ./files/setup.cgi $(1)/www/cgi-bin/modules/$(PKG_NAME)/; \
		$(INSTALL_BIN) ./files/volume $(1)/www/cgi-bin/modules/$(PKG_NAME)/; \
		$(INSTALL_BIN) ./files/sed.cgi $(1)/www/cgi-bin/modules/$(PKG_NAME)/; \
	)
	$(if $(CONFIG_WEB_RADIO2_MODERN), \
		rm -rf $(1)/www/cgi-bin/modules/$(PKG_NAME); \
		$(INSTALL_DIR) $(1)/www/cgi-bin/modules/$(PKG_NAME); \
		$(INSTALL_BIN) ./files/modern_index.cgi $(1)/www/cgi-bin/modules/$(PKG_NAME)/index.cgi; \
		$(INSTALL_BIN) ./files/action.cgi $(1)/www/cgi-bin/modules/$(PKG_NAME)/; \
		$(INSTALL_BIN) ./files/volume.cgi $(1)/www/cgi-bin/modules/$(PKG_NAME)/; \
		$(INSTALL_BIN) ./files/fade.sh $(1)/www/cgi-bin/modules/$(PKG_NAME)/; \
	)
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
