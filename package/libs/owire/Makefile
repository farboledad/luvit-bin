#
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=owire
PKG_VERSION:=2.0

PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=https://github.com/luvitred/dallas-sdk.git
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_REV:=
PKG_SOURCE_VERSION:=.

PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_SOURCE_SUBDIR)

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
	SECTION:=libs
	CATEGORY:=Libraries
	DEPENDS:=+kmod-cortex
	TITLE:=Owire Public Domain Kit
	URL:=http://files.maximintegrated.com/sia_bu/public/
	MAINTAINER:=Option <support@option.com>
endef

define Download/Defaults
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) CC="$(TARGET_CC)" LD="$(TARGET_LD)" CFLAGS="$(TARGET_CFLAGS) $(FPIC)" LDFLAGS="$(TARGET_LDFLAGS)"
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_DIR) $(1)/usr/include
	$(INSTALL_DIR) $(1)/usr/include/owire
	$(INSTALL_DIR) $(1)/usr/bin
	$(CP) $(PKG_BUILD_DIR)/libowire.so* $(1)/usr/lib/
	$(CP) $(PKG_BUILD_DIR)/libows.a $(1)/usr/lib/
	$(CP) $(PKG_BUILD_DIR)/common/ownet.h $(1)/usr/include/
	$(CP) $(PKG_BUILD_DIR)/common/findtype.h $(1)/usr/include/
	$(CP) $(PKG_BUILD_DIR)/common/atod26.h $(1)/usr/include/
	$(CP) $(PKG_BUILD_DIR)/common/temp28.h $(1)/usr/include/
	$(CP) $(PKG_BUILD_DIR)/common/swt29.h $(1)/usr/include/
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_DIR) $(1)/usr/bin
	$(CP) $(PKG_BUILD_DIR)/libowire.so* $(1)/usr/lib/
endef

$(eval $(call BuildPackage,$(PKG_NAME)))

