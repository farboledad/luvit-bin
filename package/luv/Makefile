include $(TOPDIR)/rules.mk

PKG_NAME:=luv
PKG_VERSION:=1.9.0-3
PKG_RELEASE:=1
PKG_SOURCE_PROTO:=git

PKG_SOURCE_URL:=git://github.com/luvitred/luv.git
PKG_REV:=50f58b3900acf426630bcab3a910b3cf59a0e75f

PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE:=$(PKG_NAME)-$(PKG_REV).tar.gz
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_SOURCE_SUBDIR)

include $(INCLUDE_DIR)/package.mk

MAKE_FLAGS+=VERBOSE=1

ifeq ($(CONFIG_CCACHE),)
 ifeq ($(CONFIG_EXTERNAL_TOOLCHAIN),)
  CMAKE_C_COMPILER:=$(TOOLCHAIN_DIR)/bin/$(TARGET_CC)
  CMAKE_C_COMPILER_ARG1:=
  CMAKE_CXX_COMPILER:=$(TOOLCHAIN_DIR)/bin/$(TARGET_CXX)
  CMAKE_CXX_COMPILER_ARG1:=
 else
  CMAKE_C_COMPILER:=$(shell which $(TARGET_CC))
  CMAKE_C_COMPILER_ARG1:=
  CMAKE_CXX_COMPILER:=$(shell which $(TARGET_CXX))
  CMAKE_CXX_COMPILER_ARG1:=
 endif
else
  CCACHE:=$(shell which ccache)
  ifeq ($(CCACHE),)
    CCACHE:=$(STAGING_DIR_HOST)/bin/ccache
  endif
  CMAKE_C_COMPILER:=$(CCACHE)
  CMAKE_C_COMPILER_ARG1:=$(filter-out ccache,$(TARGET_CC))
  CMAKE_CXX_COMPILER:=$(CCACHE)
  CMAKE_CXX_COMPILER_ARG1:=$(filter-out ccache,$(TARGET_CXX))
endif
CMAKE_OPTIONS += \
	-H. -Bbuild \
	-DBUILD_MODULE=ON \
	-DBUILD_SHARED_LIBS=OFF \
	-DWITH_SHARED_LIBUV=ON \
	-DLUA_BUILD_TYPE=System \
	-DLUAJIT_INCLUDE_DIR="$(STAGING_DIR)/usr/include/luajit-$(shell luajit -v | grep -oP '(?<=LuaJIT )[0-9]+\.[0-9]+')"

define Package/luv
  SUBMENU:=Lua
  SECTION:=lang
  CATEGORY:=Languages
  TITLE:=Luv - Bare libuv bindings for lua
  URL:=https://github.com/luvit/luv
  DEPENDS:=+luajit +libuv
endef

define Package/luv/description
 This is lua/luajit bindings for libuv, the evented library that powers node.js.
endef

# Do not attempt default downloads. Directly clone from git.
define Download/Defaults
endef

define Build/Configure
	(cd $(PKG_BUILD_DIR); echo "CFLAGS ....... $(TARGET_CFLAGS) $(EXTRA_CFLAGS) $(CMAKE_OPTIONS)"; \
		CFLAGS="$(TARGET_CFLAGS) $(EXTRA_CFLAGS)" \
		CXXFLAGS="$(TARGET_CFLAGS) $(EXTRA_CFLAGS)" \
		LDFLAGS="$(TARGET_LDFLAGS) $(EXTRA_LDFLAGS)" \
		cmake \
			-H. -Bbuild \
			-DCMAKE_SYSTEM_NAME=Linux \
			-DCMAKE_SYSTEM_VERSION=1 \
			-DCMAKE_SYSTEM_PROCESSOR=$(ARCH) \
			-DCMAKE_C_FLAGS_RELEASE="-DNDEBUG" \
			-DCMAKE_CXX_FLAGS_RELEASE="-DNDEBUG" \
			-DCMAKE_C_COMPILER="$(CMAKE_C_COMPILER)" \
			-DCMAKE_C_COMPILER_ARG1="$(CMAKE_C_COMPILER_ARG1)" \
			-DCMAKE_CXX_COMPILER="$(CMAKE_CXX_COMPILER)" \
			-DCMAKE_CXX_COMPILER_ARG1="$(CMAKE_CXX_COMPILER_ARG1)" \
			-DCMAKE_EXE_LINKER_FLAGS:STRING="$(TARGET_LDFLAGS)" \
			-DCMAKE_MODULE_LINKER_FLAGS:STRING="$(TARGET_LDFLAGS)" \
			-DCMAKE_SHARED_LINKER_FLAGS:STRING="$(TARGET_LDFLAGS)" \
			-DCMAKE_FIND_ROOT_PATH=$(STAGING_DIR) \
			-DCMAKE_FIND_ROOT_PATH_MODE_PROGRAM=BOTH \
			-DCMAKE_FIND_ROOT_PATH_MODE_LIBRARY=ONLY \
			-DCMAKE_FIND_ROOT_PATH_MODE_INCLUDE=ONLY \
			-DBUILD_MODULE=OFF \
			-DBUILD_SHARED_LIBS=OFF \
			-DWithSharedOpenSSL=ON \
			-DWithOpenSSL=ON \
			-DWITH_SHARED_LIBUV=ON \
			-DWITH_SHARED_LUAJIT=ON \
			-DLUA_BUILD_TYPE=System \
			-DLUAJIT_INCLUDE_DIR="$(STAGING_DIR)/usr/include/luajit-$(shell luajit -v | grep -oP '(?<=LuaJIT )[0-9]+\.[0-9]+')" \
	)
endef

define Build/Compile
	(cd $(PKG_BUILD_DIR); \
		cmake --build build --config Release \
	)
endef

define Build/InstallDev
	$(INSTALL_DIR) $(STAGING_DIR)/usr/include
	$(INSTALL_DIR) $(STAGING_DIR)/usr/include/luv
	$(CP) $(PKG_BUILD_DIR)/src/*.h $(STAGING_DIR)/usr/include/luv
	$(INSTALL_DIR) $(STAGING_DIR)/usr/lib
	$(CP) $(PKG_BUILD_DIR)/build/libluv.a $(STAGING_DIR)/usr/lib/libluv.a
endef

# $(INSTALL_DIR) $(1)/usr
# $(INSTALL_DIR) $(1)/usr/lib
# $(INSTALL_BIN) $(PKG_BUILD_DIR)/build/luv.so $(1)/usr/lib

$(eval $(call BuildPackage,luv))
