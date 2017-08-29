#
# Copyright (C) 2006-2010 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=leveldb
PKG_VERSION:=1.19
PKG_RELEASE:=1

PKG_SOURCE:=v$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=http://github.com/google/leveldb/archive/
PKG_MD5SUM:=6c201409cce6b711f46d68e0f4b1090a
PKG_MIRROR:=0

include $(INCLUDE_DIR)/package.mk

define Package/leveldb
	SECTION:=libs
	CATEGORY:=Libraries
	CATEGORY:=Libraries
	TITLE:=A fast and lightweight key/value database library by Google.
	URL:=https://github.com/google/leveldb
	MAINTAINER:=Martin Croome <m.croome@option.com>
endef

define Package/leveldb/config
	source "$(SOURCE)/Config.in"
endef

define Package/leveldb/description
	LevelDB is a fast key-value storage library written at Google
	that provides an ordered mapping from string keys to string values.
endef

TARGET_CFLAGS += -Wno-psabi

define Build/InstallDev
	echo =========== - $(1) - =============
	echo =========== - $(STAGING_DIR) - =============
	$(INSTALL_DIR) $(STAGING_DIR)/usr/include
	$(INSTALL_DIR) $(STAGING_DIR)/usr/include/leveldb
	$(CP) $(PKG_BUILD_DIR)/include/leveldb/* $(STAGING_DIR)/usr/include/leveldb
	$(INSTALL_DIR) $(STAGING_DIR)/usr/lib
	$(CP) $(PKG_BUILD_DIR)/out-shared/libleveldb.so* $(STAGING_DIR)/usr/lib
	$(CP) $(PKG_BUILD_DIR)/out-static/libleveldb.a $(STAGING_DIR)/usr/lib
endef

define Package/leveldb/install/test
	$(INSTALL_DIR) $(1)/usr/bin/leveldb
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/out-static/*_test $(1)/usr/bin/leveldb
endef

define Package/leveldb/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_BUILD_DIR)/out-shared/libleveldb.so* $(1)/usr/lib/
ifeq ($(CONFIG_LVLDB_TEST),y)
	$(call Package/leveldb/install/test, $(1))
endif
endef

$(eval $(call BuildPackage,leveldb))
