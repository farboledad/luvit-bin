#
# Copyright (C) 2010 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#


include $(TOPDIR)/rules.mk

PKG_NAME:=LuaJIT

PKG_RELEASE:=1
#PKG_REV used in pc luvi script
PKG_REV2_1:=99cdfbf6a1e8856f64908072ef10443a7eab14f2
PKG_REV2_0:=9f0caad0e43f97a4613850b3874b851cb1bc301d

ifeq ($(CONFIG_LUAJIT_TWO_ONE),y)
PKG_MAJOR_VERSION:=2
PKG_MINOR_VERSION:=1
PKG_MINOR_REV:=0
PKG_VERSION:=2.1
PKG_SUFFIX=-beta3
PKG_REV:=$(PKG_REV2_1)
else
PKG_MAJOR_VERSION:=2
PKG_MINOR_VERSION:=0
PKG_MINOR_REV:=5
PKG_VERSION:=2.0.5
PKG_REV:=$(PKG_REV2_0)
endif
PKG_LIB_VERSION:=$(PKG_MAJOR_VERSION).$(PKG_MINOR_VERSION).$(PKG_MINOR_REV)
PKG_LUAJIT_EXE:=luajit-$(PKG_LIB_VERSION)$(PKG_SUFFIX)

PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=https://github.com/LuaJIT/LuaJIT.git

PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)

PKG_SOURCE:=$(PKG_NAME)-$(PKG_REV).tar.gz
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_SOURCE_SUBDIR)
HOST_BUILD_DIR:=$(BUILD_DIR_HOST)/$(PKG_SOURCE_SUBDIR)

PKG_BUILD_PARALLEL:=1
HOST_BUILD_PARALLEL:=1

PKG_BUILD_DEPENDS:=luajit/host

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/host-build.mk

define Package/luajit/config
	menu "Configuration"
		depends on PACKAGE_luajit

	config LUAJIT_TWO_ONE
		bool "Build 2.1 (not 2.0.5)"
		default y

	endmenu
endef

define Package/luajit
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:=luajit library
endef

# Do not attempt default downloads. Directly clone from git.
define Download/Defaults
endef

define Build/Compile
	@echo ">>>> Building CloudGate version $(PKG_VERSION) lib $(PKG_LIB_VERSION)"
	$(MAKE) -C $(PKG_BUILD_DIR) \
		HOST_CC="gcc -m32" \
		CROSS=$(TARGET_CROSS) \
		TARGET_CFLAGS="$(TARGET_CFLAGS)" \
		HOST_CFLAGS="$(HOST_CFLAGS)" \
		TARGET_LDFLAGS="$(TARGET_LDFLAGS)" \
		HOST_LDFLAGS="$(HOST_LDFLAGS)" \
		DESTDIR="$(PKG_INSTALL_DIR)" \
		PREFIX=/usr \
		INSTALL_LMOD="/usr/share/lua" \
		INSTALL_CMOD="/usr/share/lua" \
		STAGING_DIR_HOST=$(STAGING_DIR_HOST) amalg
	$(MAKE) PREFIX=$(PKG_BUILD_DIR)/usr -C $(PKG_BUILD_DIR) \
		HOST_CC="gcc -m32" \
		CROSS=$(TARGET_CROSS) \
		TARGET_CFLAGS="$(TARGET_CFLAGS)" \
		HOST_CFLAGS="$(HOST_CFLAGS)" \
		TARGET_LDFLAGS="$(TARGET_LDFLAGS)" \
		HOST_LDFLAGS="$(HOST_LDFLAGS)" \
		DESTDIR="$(PKG_INSTALL_DIR)" \
		PREFIX=/usr \
		INSTALL_LMOD="/usr/share/lua" \
		INSTALL_CMOD="/usr/share/lua" install
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/{lib,include}
	$(INSTALL_DIR) $(1)/usr/lib/pkgconfig
	$(INSTALL_DIR) $(1)/usr/include/luajit-$(PKG_MAJOR_VERSION).$(PKG_MINOR_VERSION)
	$(INSTALL_DIR) $(1)/usr/include/luvit
	$(INSTALL_DIR) $(1)/usr/include/luvit/luajit
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libluajit* $(1)/usr/lib/
	-(cd $(1)/usr/lib ;ln -s libluajit-5.1.so libluajit.so)
	$(CP) $(PKG_INSTALL_DIR)/usr/include/luajit-$(PKG_MAJOR_VERSION).$(PKG_MINOR_VERSION)/* $(1)/usr/include/luajit-$(PKG_MAJOR_VERSION).$(PKG_MINOR_VERSION)
	# clean up old header files that might exist in SDK
	-$(RM) $(1)/usr/include/lauxlib.h
	-$(RM) $(1)/usr/include/luaconf.h
	-$(RM) $(1)/usr/include/lua.hpp
	-$(RM) $(1)/usr/include/lua.h
	-$(RM) $(1)/usr/include/luajit.h
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/pkgconfig/luajit.pc $(1)/usr/lib/pkgconfig/luajit.pc
endef

define Host/Patch
endef

define Host/Compile
	@echo ">>>> Building host version $(PKG_VERSION) lib $(PKG_LIB_VERSION)"
	$(MAKE) -C $(HOST_BUILD_DIR) \
		PREFIX="$(STAGING_DIR_HOST)" \
		INSTALL_LMOD="$(STAGING_DIR_HOST)/share/luajit" \
		INSTALL_CMOD="$(STAGING_DIR_HOST)/share/luajit" \
		STAGING_DIR_HOST=$(STAGING_DIR_HOST) amalg
endef

define Host/Install
	$(INSTALL_DIR) $(STAGING_DIR_HOST)/bin
	$(MAKE) -C $(HOST_BUILD_DIR) \
		PREFIX="$(STAGING_DIR_HOST)" \
		INSTALL_LMOD="$(STAGING_DIR_HOST)/share/luajit" \
		INSTALL_CMOD="$(STAGING_DIR_HOST)/share/luajit" \
		STAGING_DIR_HOST=$(STAGING_DIR_HOST) install
	$(RM) $(STAGING_DIR_HOST)/bin/luajit
	$(CP) $(STAGING_DIR_HOST)/bin/$(PKG_LUAJIT_EXE) $(STAGING_DIR_HOST)/bin/luajit
endef

define Package/luajit/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libluajit*so* $(1)/usr/lib/
	-ln -s libluajit-5.1.so.$(PKG_LIB_VERSION) $(1)/usr/lib/libluajit-5.1.so.2
	-ln -s libluajit-5.1.so.$(PKG_LIB_VERSION) $(1)/usr/lib/libluajit-5.1.so
endef

$(eval $(call HostBuild))
$(eval $(call BuildPackage,luajit))
