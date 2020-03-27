#
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk

PKG_NAME:=qpid-proton
PKG_VERSION:=0.30.0
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=http://archive.apache.org/dist/qpid/proton/$(PKG_VERSION)
PKG_MD5SUM:=0f66d297cc9116b50835dc845150e3a7

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

TARGET_CFLAGS += -I$(STAGING_DIR)/usr/include

CMAKE_INSTALL:=0

CMAKE_OPTIONS += -DCMAKE_BUILD_TYPE=Release \
	-DBUILD_TESTING=OFF -DBUILD_BINDINGS="" \
	-DENABLE_FUZZ_TESTING=OFF -DENABLE_LINKTIME_OPTIMIZATION=OFF

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/cmake.mk

define Package/qpid-proton
  SECTION:=libs
  CATEGORY:=Libraries
  DEPENDS:=+libopenssl
  TITLE:=qpid-proton AMQP library
endef

define Build/InstallDev
	$(INSTALL_DIR) $(STAGING_DIR)/usr/include/proton
	$(CP) $(PKG_BUILD_DIR)/c/include/* $(STAGING_DIR)/usr/include/
	$(INSTALL_DIR) $(STAGING_DIR)/usr/lib
	rm -f $(STAGING_DIR)/usr/lib/libqpid-proton.*
	$(CP) $(PKG_BUILD_DIR)/c/libqpid-proton.so* $(STAGING_DIR)/usr/lib/
endef

define Package/qpid-proton/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_BUILD_DIR)/c/libqpid-proton.so* $(1)/usr/lib/
endef

$(eval $(call BuildPackage,qpid-proton))
