#
# Copyright (C) 2015 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=web-ir-remote
PKG_VERSION:=1.0
PKG_RELEASE:=16.03.2020

include $(INCLUDE_DIR)/package.mk

define Package/web-ir-remote
  SECTION:=utils
  CATEGORY:=Utilities
  DEPENDS:=@TARGET_sunxi +kmod-sunxi-ir +v4l-utils +triggerhappy
  TITLE:=Test web-ir-remote setup
endef

define Package/web-ir-remote/description
endef

define Package/web-ir-remote/postrm
#!/bin/sh
 sed -i "/web-ir-remote\/index.cgi/d" /www/menu.html 2> /dev/null
 rm -R /www/cgi-bin/modules/web-ir-remote &> /dev/null
endef

define Build/Compile
endef

define Package/web-ir-remote/install
	$(INSTALL_DIR) $(1)/etc/uci-defaults
	$(INSTALL_BIN) ./files/uci_defaults_web-ir-remote $(1)/etc/uci-defaults/51-web-ir-remote
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/web-ir-remote.init $(1)/etc/init.d/web-ir-remote
	$(INSTALL_DIR) $(1)/etc/rc_keymaps
	$(INSTALL_DATA) ./files/keyes $(1)/etc/rc_keymaps/
#	$(INSTALL_DIR) $(1)/etc/triggerhappy/triggers.d
#	$(INSTALL_DATA) ./files/example.conf $(1)/etc/triggerhappy/triggers.d/
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) ./files/web-ir-remote-demon $(1)/usr/sbin/ir-remote

##		WEB-MORDA_Cyberwrt
	$(INSTALL_DIR) $(1)/www/irc
	$(INSTALL_BIN) ./files/irc.html $(1)/www/irc/index.html
	$(INSTALL_DIR) $(1)/www/cgi-bin/modules/web-ir-remote
	$(INSTALL_BIN) ./files/index.cgi $(1)/www/cgi-bin/modules/web-ir-remote/
	$(INSTALL_BIN) ./files/index.html $(1)/www/cgi-bin/modules/web-ir-remote/
	$(INSTALL_BIN) ./files/Save.cgi $(1)/www/cgi-bin/modules/web-ir-remote/
	$(INSTALL_BIN) ./files/Setup.cgi $(1)/www/cgi-bin/modules/web-ir-remote/
	$(INSTALL_BIN) ./files/Setup.html $(1)/www/cgi-bin/modules/web-ir-remote/
	$(INSTALL_BIN) ./files/Setuptryg.cgi $(1)/www/cgi-bin/modules/web-ir-remote/
	$(INSTALL_BIN) ./files/Setuptryg.html $(1)/www/cgi-bin/modules/web-ir-remote/
	$(INSTALL_DIR) $(1)/www/modules/web-ir-remote
	$(INSTALL_DATA) ./files/b.jpg $(1)/www/modules/web-ir-remote/
	$(INSTALL_DATA) ./files/r.jpg $(1)/www/modules/web-ir-remote/
endef

$(eval $(call BuildPackage,web-ir-remote))
