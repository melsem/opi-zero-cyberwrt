#
# Copyright (C) 2006-2009 OpenWrt.org
# Copyright 2010 Vertical Communications
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=web-cyberwrt
PKG_VERSION:=1.2
PKG_RELEASE:=web-uhttpd-cyberwrt

include $(INCLUDE_DIR)/package.mk

define Package/web-cyberwrt
  SECTION:=utils
  CATEGORY:=Utilities
  DEPENDS:=+uhttpd
  TITLE:=web-uhttpd
  PKGARCH:=all
endef

define Build/Compile
endef

define Package/web-cyberwrt/install
	$(INSTALL_DIR) $(1)/www
	$(INSTALL_DATA) ./files/favicon.ico $(1)/www/favicon.ico
	$(INSTALL_DATA) ./files/menu.html $(1)/www/menu.html
	$(INSTALL_DIR) $(1)/www/modules
	$(INSTALL_BIN) ./files/modules/style.css $(1)/www/modules/style.css
	$(INSTALL_DIR) $(1)/www/modules/manmod
	$(INSTALL_BIN) ./files/modules/manmod/module.js $(1)/www/modules/manmod/module.js
	$(INSTALL_DIR) $(1)/www/modules/filemanager
	$(INSTALL_BIN) ./files/modules/filemanager/checkbox.png $(1)/www/modules/filemanager/checkbox.png
	$(INSTALL_BIN) ./files/modules/filemanager/chmod.js $(1)/www/modules/filemanager/chmod.js
	$(INSTALL_BIN) ./files/modules/filemanager/del.png $(1)/www/modules/filemanager/del.png
	$(INSTALL_BIN) ./files/modules/filemanager/edit.png $(1)/www/modules/filemanager/edit.png
	$(INSTALL_BIN) ./files/modules/filemanager/file.png $(1)/www/modules/filemanager/file.png
	$(INSTALL_BIN) ./files/modules/filemanager/folder.png $(1)/www/modules/filemanager/folder.png
	$(INSTALL_DIR) $(1)/www/cyberwrt
	$(INSTALL_DATA) ./files/cyberwrt/index.html $(1)/www/cyberwrt/index.html
	$(INSTALL_DIR) $(1)/www/cgi-bin
	$(INSTALL_BIN) ./files/cgi-bin/recovery.cgi $(1)/www/cgi-bin/recovery.cgi
	$(INSTALL_DIR) $(1)/www/cgi-bin/modules/about
	$(INSTALL_BIN) ./files/cgi-bin/modules/about/index.cgi $(1)/www/cgi-bin/modules/about/index.cgi
	$(INSTALL_DIR) $(1)/www/cgi-bin/modules/authorization
	$(INSTALL_BIN) ./files/cgi-bin/modules/authorization/index.cgi $(1)/www/cgi-bin/modules/authorization/index.cgi
	$(INSTALL_DIR) $(1)/www/cgi-bin/modules/backup_rom
	$(INSTALL_BIN) ./files/cgi-bin/modules/backup_rom/index.cgi $(1)/www/cgi-bin/modules/backup_rom/index.cgi
	$(INSTALL_BIN) ./files/cgi-bin/modules/backup_rom/bigpic.jpg.cgi $(1)/www/cgi-bin/modules/backup_rom/bigpic.jpg.cgi
	$(INSTALL_BIN) ./files/cgi-bin/modules/backup_rom/f.cgi $(1)/www/cgi-bin/modules/backup_rom/f.cgi
	$(INSTALL_DIR) $(1)/www/cgi-bin/modules/cron
	$(INSTALL_BIN) ./files/cgi-bin/modules/cron/index.cgi $(1)/www/cgi-bin/modules/cron/index.cgi
	$(INSTALL_DIR) $(1)/www/cgi-bin/modules/filemanager
	$(INSTALL_BIN) ./files/cgi-bin/modules/filemanager/index.cgi $(1)/www/cgi-bin/modules/filemanager/index.cgi
	$(INSTALL_BIN) ./files/cgi-bin/modules/filemanager/edit.cgi $(1)/www/cgi-bin/modules/filemanager/edit.cgi
	$(INSTALL_BIN) ./files/cgi-bin/modules/filemanager/editd.cgi $(1)/www/cgi-bin/modules/filemanager/editd.cgi
	$(INSTALL_BIN) ./files/cgi-bin/modules/filemanager/edit_save.cgi $(1)/www/cgi-bin/modules/filemanager/edit_save.cgi
	$(INSTALL_BIN) ./files/cgi-bin/modules/filemanager/fil.cgi $(1)/www/cgi-bin/modules/filemanager/fil.cgi
	$(INSTALL_BIN) ./files/cgi-bin/modules/filemanager/upload_file.cgi $(1)/www/cgi-bin/modules/filemanager/upload_file.cgi
	$(INSTALL_DIR) $(1)/www/cgi-bin/modules/manmod
	$(INSTALL_BIN) ./files/cgi-bin/modules/manmod/index.cgi $(1)/www/cgi-bin/modules/manmod/index.cgi
	$(INSTALL_BIN) ./files/cgi-bin/modules/manmod/aunt.cyber $(1)/www/cgi-bin/modules/manmod/aunt.cyber
endef

$(eval $(call BuildPackage,web-cyberwrt))

