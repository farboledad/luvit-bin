#
# Copyright (C) 2006-2010 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=leveldb
PKG_VERSION:=1.20
PKG_RELEASE:=1

PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=https://github.com/google/leveldb.git
PKG_REV:=ea0a7586b8615fd39c6b8f5a8a21a1f242129c2f

PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE:=$(PKG_NAME)-$(PKG_REV).tar.gz
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_SOURCE_SUBDIR)

PKG_MIRROR:=0

include $(INCLUDE_DIR)/package.mk

define Package/leveldb
  SECTION:=libs
  CATEGORY:=Libraries
  CATEGORY:=Libraries
  TITLE:=A fast and lightweight key/value database library by Google.
  URL:=https://github.com/google/leveldb
  MAINTAINER:=Option <support@option.com>
  DEPENDS:=+libpthread +libstdc++
endef

define Package/leveldb/extra_provides
	echo libstdc++.so.6
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
