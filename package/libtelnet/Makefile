#
# Copyright (C) 2010 Option NV
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#


include $(TOPDIR)/rules.mk

PKG_NAME:=libtelnet
PKG_VERSION:=0.21.1
PKG_RELEASE:=1

PKG_SOURCE_URL:=https://github.com/sousoux/libtelnet.git
PKG_REV:=4efa4497cdb400356e80b27da50b0c576d62fdef

PKG_SOURCE:=$(PKG_NAME)-$(PKG_REV).tar.gz
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_BUILD_PARALLEL:=0
PKG_SOURCE_PROTO:=git
PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_SOURCE_SUBDIR)

include $(INCLUDE_DIR)/package.mk

define Package/libtelnet
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:=Telnet protocol library
endef

define Build/Prepare
	$(call Build/Prepare/Default)
	( cd $(PKG_BUILD_DIR) ; \
		$(STAGING_DIR_HOST)/bin/autoreconf --install ; \
	)
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/{lib,include}
	$(INSTALL_DIR) $(1)/usr/include/
	$(INSTALL_DIR) $(1)/usr/include/
	$(CP) $(PKG_BUILD_DIR)/.libs/libtelnet.a $(1)/usr/lib/
	$(CP) $(PKG_BUILD_DIR)/.libs/libtelnet.so* $(1)/usr/lib/
	$(CP) $(PKG_BUILD_DIR)/libtelnet.h $(1)/usr/include
endef

define Package/libtelnet/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_BUILD_DIR)/.libs/libtelnet.so* $(1)/usr/lib/
endef

$(eval $(call BuildPackage,libtelnet))
