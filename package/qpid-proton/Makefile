#
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk

PKG_NAME:=qpid-proton
PKG_VERSION:=0.9
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=http://archive.apache.org/dist/qpid/proton/$(PKG_VERSION)
PKG_MD5SUM:=aa42b8465d35cf902931daabbe4ef489

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

TARGET_CFLAGS += -I$(STAGING_DIR)/usr/include

CMAKE_INSTALL:=1

CMAKE_OPTIONS += -DCMAKE_BUILD_TYPE=Release \
	-DBUILD_PHP=OFF -DBUILD_RUBY=OFF -DBUILD_PERL=OFF -DBUILD_PYTHON=OFF -DBUILD_JAVA=OFF

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/cmake.mk

define Package/qpid-proton
  SECTION:=libs
  CATEGORY:=Libraries
  DEPENDS:=+libopenssl
  TITLE:=qpid-proton v0.8 AMQP library
endef

define Build/InstallDev
	$(INSTALL_DIR) $(STAGING_DIR)/usr/include/proton
	$(CP) $(PKG_BUILD_DIR)/proton-c/include/* $(STAGING_DIR)/usr/include/
	$(INSTALL_DIR) $(STAGING_DIR)/usr/lib
	rm -f $(STAGING_DIR)/usr/lib/libqpid-proton.*
	$(CP) $(PKG_BUILD_DIR)/proton-c/libqpid-proton.so.2.0.0 $(STAGING_DIR)/usr/lib/
	ln -s ./libqpid-proton.so.2.0.0 $(STAGING_DIR)/usr/lib/libqpid-proton.so.2
	ln -s ./libqpid-proton.so.2.0.0 $(STAGING_DIR)/usr/lib/libqpid-proton.so
endef

define Package/qpid-proton/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/proton-c/libqpid-proton.so.2.0.0 $(1)/usr/lib/
	ln -s ./libqpid-proton.so.2.0.0 $(1)/usr/lib/libqpid-proton.so.2
	ln -s ./libqpid-proton.so.2.0.0 $(1)/usr/lib/libqpid-proton.so
endef

$(eval $(call BuildPackage,qpid-proton))
