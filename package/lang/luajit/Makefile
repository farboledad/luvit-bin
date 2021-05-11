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
PKG_REV2_1:=1e66d0f9e6698fdee672c40a9a5b4159c9254091
PKG_REV2_0:=f47c864b0186fc92a2264e68e1151d0d2b24969e

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
  SUBMENU:=Lua
  SECTION:=lang
  CATEGORY:=Languages
  TITLE:=LuaJIT
  URL:=http://www.luajit.org
  MAINTAINER:=Option <support@option.com>
endef

define Package/luajit/description
 LuaJIT is a Just-In-Time (JIT) compiler for the Lua programming language. *** Requires GCC Multilib on host system to build! ***
endef

# Do not attempt default downloads. Directly clone from git.
define Download/Defaults
endef

XCFLAGS:=-DLUAJIT_DISABLE_GC64 -DLUAJIT_ENABLE_LUA52COMPAT

define Build/Compile
	@echo ">>>> Building CloudGate version $(PKG_VERSION) lib $(PKG_LIB_VERSION)"
	$(MAKE) -C $(PKG_BUILD_DIR) \
		HOST_CC="$(HOSTCC) -m32" \
		CROSS=$(TARGET_CROSS) \
		TARGET_CFLAGS="$(TARGET_CFLAGS)" \
		HOST_CFLAGS="$(HOST_CFLAGS)" \
		XCFLAGS="$(XCFLAGS)" \
		TARGET_LDFLAGS="$(TARGET_LDFLAGS)" \
		HOST_LDFLAGS="$(HOST_LDFLAGS)" \
		DESTDIR="$(PKG_INSTALL_DIR)" \
		PREFIX=/usr \
		INSTALL_LMOD="/usr/share/lua" \
		INSTALL_CMOD="/usr/share/lua" \
		STAGING_DIR_HOST=$(STAGING_DIR_HOST) \
		BUILDMODE=dynamic \
		amalg
	rm -rf $(PKG_INSTALL_DIR)
	mkdir -p $(PKG_INSTALL_DIR)
	$(MAKE) PREFIX=$(PKG_BUILD_DIR)/usr -C $(PKG_BUILD_DIR) \
		HOST_CC="$(HOSTCC)-m32" \
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
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libluajit* $(1)/usr/lib/
	-(cd $(1)/usr/lib ;ln -s libluajit-5.1.so libluajit.so)
	$(CP) $(PKG_INSTALL_DIR)/usr/include/luajit-$(PKG_MAJOR_VERSION).$(PKG_MINOR_VERSION)/* $(1)/usr/include/luajit-$(PKG_MAJOR_VERSION).$(PKG_MINOR_VERSION)
	# clean up (default) header files that might exist in SDK
	-$(RM) $(STAGING_DIR)/usr/include/lua.h
	-$(RM) $(STAGING_DIR)/usr/include/lua.hpp
	-$(RM) $(STAGING_DIR)/usr/include/luaconf.h
	-$(RM) $(STAGING_DIR)/usr/include/lualib.h
	-$(RM) $(STAGING_DIR)/usr/include/luajit.h
	-$(RM) $(STAGING_DIR)/usr/include/lauxlib.h
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/pkgconfig/luajit.pc $(1)/usr/lib/pkgconfig/luajit.pc
endef

define Host/Patch
endef

define Host/Compile
	@echo ">>>> Building host version $(PKG_VERSION) lib $(PKG_LIB_VERSION)"
	$(MAKE) -C $(HOST_BUILD_DIR) \
		XCFLAGS="$(XCFLAGS)" \
		PREFIX="$(STAGING_DIR_HOST)" \
		INSTALL_LMOD="" \
		INSTALL_CMOD="" \
		STAGING_DIR_HOST=$(STAGING_DIR_HOST) amalg
endef

define Host/Install
	$(INSTALL_DIR) $(STAGING_DIR_HOST)/bin
	$(MAKE) -C $(HOST_BUILD_DIR) \
		PREFIX="$(STAGING_DIR_HOST)" \
		INSTALL_LMOD="" \
		INSTALL_CMOD="" \
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
