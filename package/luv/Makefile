include $(TOPDIR)/rules.mk

PKG_NAME:=luv
PKG_VERSION:=1.26
PKG_RELEASE:=1
PKG_SOURCE_PROTO:=git

PKG_SOURCE_URL:=git://github.com/luvitred/luv.git
PKG_REV:=b1957a965de2f603cda0b840f27755e6dfe3e19b
PKG_BRANCH:=v$(PKG_VERSION)
PKG_DATE:="2018-10-01"

PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION)-$(PKG_REV).tar.gz
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_SOURCE_SUBDIR)

include $(INCLUDE_DIR)/package.mk

define Package/luv
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:=luv - Bare libuv bindings for lua
  URL:=https://github.com/luvit/luv
  MAINTAINER:=Option <support@option.com>
  DEPENDS:=+luajit +libuv
endef

define Package/luv/description
  This is lua bindings for libuv, the evented library that powers node.js.
endef

MAKE_FLAGS+=VERBOSE=1

ifeq ($(CONFIG_CCACHE),)
 ifeq ($(CONFIG_EXTERNAL_TOOLCHAIN),)
  CMAKE_C_COMPILER:=$(TOOLCHAIN_DIR)/bin/$(TARGET_CC)
  CMAKE_C_COMPILER_ARG1:=
 else
  CMAKE_C_COMPILER:=$(shell which $(TARGET_CC))
  CMAKE_C_COMPILER_ARG1:=
 endif
else
  CCACHE:=$(shell which ccache)
  ifeq ($(CCACHE),)
    CCACHE:=$(STAGING_DIR_HOST)/bin/ccache
  endif
  CMAKE_C_COMPILER:=$(CCACHE)
  CMAKE_C_COMPILER_ARG1:=$(filter-out ccache,$(TARGET_CC))
endif
CMAKE_OPTIONS += \
	-H. -Bbuild \
	-DBUILD_MODULE=OFF \
	-DBUILD_SHARED_LIBS=OFF \
	-DWITH_SHARED_LIBUV=ON \
	-DLUA_BUILD_TYPE=System \
	-DLUA_COMPAT53_DIR="$(PKG_BUILD_DIR)/deps/lua-compat-5.3" \
	-DLUAJIT_INCLUDE_DIR="$(STAGING_DIR)/usr/include/luajit-$(shell luajit -v | grep -oP '(?<=LuaJIT )[0-9]+\.[0-9]+')"

# Do not attempt default downloads. Directly clone from git.
define Download/Defaults
endef

# Redefine DownloadMethod/git. A git clone --recursive is required to fetch
# the submodules.
define DownloadMethod/git
    $(call wrap_mirror, \
        echo "Checking out files from the git repository..."; \
        mkdir -p $(TMP_DIR)/dl && \
        cd $(TMP_DIR)/dl && \
        rm -rf $(SUBDIR) && \
        [ \! -d $(SUBDIR) ] && \
        git clone -b $(PKG_BRANCH) --shallow-since=$(PKG_DATE) $(URL) $(SUBDIR) && \
        (cd $(SUBDIR) && git checkout $(VERSION) && git submodule update --init deps/lua-compat-5.3) && \
        echo "Packing checkout..." && \
        rm -rf $(SUBDIR)/.git && \
        $(call dl_pack,$(TMP_DIR)/dl/$(FILE),$(SUBDIR)) && \
        mv $(TMP_DIR)/dl/$(FILE) $(DL_DIR)/ && \
        rm -rf $(SUBDIR); \
    )
endef

define Build/Configure
	(cd $(PKG_BUILD_DIR); echo -e "CFLAGS ....... $(TARGET_CFLAGS) $(EXTRA_CFLAGS) $(CMAKE_OPTIONS)\n"; \
		CFLAGS="$(TARGET_CFLAGS) $(EXTRA_CFLAGS)" \
		CXXFLAGS="$(TARGET_CFLAGS) $(EXTRA_CFLAGS)" \
		LDFLAGS="$(TARGET_LDFLAGS) $(EXTRA_LDFLAGS)" \
		cmake \
			$(CMAKE_OPTIONS) \
			-DCMAKE_SYSTEM_NAME=Linux \
			-DCMAKE_SYSTEM_VERSION=1 \
			-DCMAKE_SYSTEM_PROCESSOR=$(ARCH) \
			-DCMAKE_C_FLAGS_RELEASE="-DNDEBUG" \
			-DCMAKE_C_COMPILER="$(CMAKE_C_COMPILER)" \
			-DCMAKE_C_COMPILER_ARG1="$(CMAKE_C_COMPILER_ARG1)" \
			-DCMAKE_FIND_ROOT_PATH=$(STAGING_DIR) \
			-DCMAKE_FIND_ROOT_PATH_MODE_PROGRAM=BOTH \
			-DCMAKE_FIND_ROOT_PATH_MODE_LIBRARY=ONLY \
			-DCMAKE_FIND_ROOT_PATH_MODE_INCLUDE=ONLY \
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
	$(CP) $(PKG_BUILD_DIR)/src/*.h $(STAGING_DIR)/usr/include/luv/
	$(INSTALL_DIR) $(STAGING_DIR)/usr/include/c-api
	$(CP) $(PKG_BUILD_DIR)/deps/lua-compat-5.3/c-api/* $(STAGING_DIR)/usr/include/c-api
	$(CP) $(PKG_BUILD_DIR)/deps/lua-compat-5.3/lprefix.h $(STAGING_DIR)/usr/include/
	$(CP) $(PKG_BUILD_DIR)/deps/lua-compat-5.3/lstrlib.c $(STAGING_DIR)/usr/include/
	$(CP) $(PKG_BUILD_DIR)/deps/lua-compat-5.3/ltablib.c $(STAGING_DIR)/usr/include/
	$(CP) $(PKG_BUILD_DIR)/deps/lua-compat-5.3/lutf8lib.c $(STAGING_DIR)/usr/include/
	$(CP) $(TOOLCHAIN_DIR)/include/errno.h $(STAGING_DIR)/include/
	$(INSTALL_DIR) $(STAGING_DIR)/usr/lib
	$(CP) $(PKG_BUILD_DIR)/build/libluv.a $(STAGING_DIR)/usr/lib/
endef

$(eval $(call BuildPackage,luv))
