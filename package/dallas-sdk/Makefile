#
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=dallas-sdk
PKG_VERSION:=310
PKG_RELEASE:=r2


PKG_SOURCE:=owpd$(PKG_VERSION)$(PKG_RELEASE).zip
PKG_SOURCE_URL:=http://files.maximintegrated.com/sia_bu/public/
PKG_MD5SUM:=b80de3bc73c8419e4b249da584eed028

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

UNPACK_CMD=unzip -d $(1)/ $(DL_DIR)/$(PKG_SOURCE)

define Package/dallas-sdk
  SECTION:=iButton-3rd-party
  CATEGORY:=iButton-3rd-party
  DEPENDS:=+kmod-cortex
  TITLE:=Owire Public Domain Kit
  URL:=http://files.maximintegrated.com/sia_bu/public/
  MAINTAINER:=Option <support@option.com>
endef

define Package/dallas-sdk/config
	source "$(SOURCE)/Config.in"
endef

define Build/Prepare
	$(call Build/Prepare/Default)
	$(CP) -r src/* $(PKG_BUILD_DIR)/
endef

define Build/Configure
endef

OW_APP_FILE_NAME = $(CONFIG_DALLAS_SDK_OWIRE_EXAMPLE_APP)
export OW_APP_FILE_NAME

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) CC="$(TARGET_CC)" LD="$(TARGET_LD)" CFLAGS="$(TARGET_CFLAGS) $(TARGET_CPPFLAGS) $(FPIC)" LDFLAGS="$(TARGET_LDFLAGS)" $(ds-lib-y)
	$(MAKE) -C $(PKG_BUILD_DIR) CC="$(TARGET_CC)" LD="$(TARGET_LD)" CFLAGS="$(TARGET_CFLAGS) $(TARGET_CPPFLAGS) $(FPIC)" LDFLAGS="$(TARGET_LDFLAGS)" libows.a
ifneq ($(CONFIG_DALLAS_SDK_OWIRE_EXAMPLE_APP),"")
	$(MAKE) -C $(PKG_BUILD_DIR) CC="$(TARGET_CC)" LD="$(TARGET_LD)" CFLAGS="$(TARGET_CFLAGS) $(TARGET_CPPFLAGS) $(FPIC)" LDFLAGS="$(TARGET_LDFLAGS)"
endif
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_DIR) $(1)/usr/include
	$(INSTALL_DIR) $(1)/usr/include/owire
	$(INSTALL_DIR) $(1)/usr/bin
	$(CP) $(PKG_BUILD_DIR)/libowirecortex.so* $(1)/usr/lib/
	$(CP) $(PKG_BUILD_DIR)/libows.a $(1)/usr/lib/
	$(CP) $(PKG_BUILD_DIR)/common/findtype.h $(1)/usr/include/
	$(CP) $(PKG_BUILD_DIR)/common/temp28.h $(1)/usr/include/
	$(CP) $(PKG_BUILD_DIR)/common/ownet.h $(1)/usr/include/
endef

define Package/dallas-sdk/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_DIR) $(1)/usr/bin
	$(CP) $(PKG_BUILD_DIR)/libowirecortex.so* $(1)/usr/lib/
ifneq ($(CONFIG_DALLAS_SDK_OWIRE_EXAMPLE_APP),"")
	$(CP) $(PKG_BUILD_DIR)/$(OW_APP_FILE_NAME) $(1)/usr/bin/
endif
endef

$(eval $(call BuildPackage,dallas-sdk))

