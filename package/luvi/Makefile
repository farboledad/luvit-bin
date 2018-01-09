#
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk

PKG_NAME:=luvi
PKG_VERSION:=2.7.6
PKG_RELEASE:=3
PKG_SOURCE_PROTO:=git

PKG_SOURCE_URL:=git://github.com/luvitred/luvi.git
PKG_REV:=2c195ec92b86a296b403f228dbecad6c08cf1160

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE:=$(PKG_NAME)-$(PKG_REV)-$(PKG_RELEASE).tar.gz
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_SOURCE_SUBDIR)

include $(INCLUDE_DIR)/package.mk

define Package/luvi
  SUBMENU:=Lua
  SECTION:=lang
  CATEGORY:=Languages
  TITLE:=A project in-between luv and luvit
  URL:=https://github.com/luvit/luvi
  DEPENDS:=+libopenssl +luv
endef

define Package/luvi/config
	source "$(SOURCE)/Config.in"
endef

TARGET_CFLAGS += -I$(STAGING_DIR)/usr/include

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
        git clone --recursive $(URL) $(SUBDIR) && \
        (cd $(SUBDIR) && git checkout $(VERSION) && git submodule update --checkout --recursive) && \
        echo "Packing checkout..." && \
        rm -rf $(SUBDIR)/.git && \
        $(call dl_pack,$(TMP_DIR)/dl/$(FILE),$(SUBDIR)) && \
        mv $(TMP_DIR)/dl/$(FILE) $(DL_DIR)/ && \
        rm -rf $(SUBDIR); \
    )
endef

LUVI_OPTS=

ifeq ($(CONFIG_LUVI_EXCLUDE_PCRE),y)
  LUVI_OPTS += -DWithPCRE=OFF
else
  LUVI_OPTS += -DWithPCRE=ON
endif

ifeq ($(CONFIG_LUVI_EXCLUDE_LPEG),y)
  LUVI_OPTS += -DWithLPEG=OFF
else
  LUVI_OPTS += -DWithLPEG=ON
endif

ifeq ($(CONFIG_LUVI_EXCLUDE_SSL),y)
  LUVI_OPTS += -DWithOpenSSL=OFF
else
  LUVI_OPTS += -DWithOpenSSL=ON -DWithSharedOpenSSL=ON
endif

ifeq ($(CONFIG_LUVI_EXCLUDE_ZLIB),y)
  LUVI_OPTS += -DWithZLIB=OFF
else
  LUVI_OPTS += -DWithZLIB=ON -DWithSharedZLIB=ON
endif

# Luvi will first check for a VERSION file to set its ${LUVI_VERSION} variable.
# The "git describe --tags" alternative will not work for CloudGate (no more .git directory).
define Build/Configure
	@echo $(PKG_VERSION) > $(PKG_BUILD_DIR)/VERSION
	(cd $(PKG_BUILD_DIR); \
		CFLAGS="$(TARGET_CFLAGS)" \
		CXXFLAGS="$(TARGET_CFLAGS)" \
		LDFLAGS="$(TARGET_LDFLAGS)" \
		LUA_PATH="?;?.lua;$(STAGING_DIR_HOST)/share/luajit/?/?.lua" \
		LUA_COMMAND="$(STAGING_DIR_HOST)/bin/luajit" \
		cmake \
			-H. -DCMAKE_BUILD_TYPE=Release \
			-DCMAKE_SYSTEM_NAME=Linux \
			-DCMAKE_SYSTEM_VERSION=1 \
			-DCMAKE_SYSTEM_PROCESSOR=$(ARCH) \
			-DCMAKE_C_FLAGS_RELEASE="-DNDEBUG" \
			-DCMAKE_C_COMPILER="$(CMAKE_C_COMPILER)" \
			-DCMAKE_C_COMPILER_ARG1="$(CMAKE_C_COMPILER_ARG1)" \
			-DCMAKE_EXE_LINKER_FLAGS:STRING="$(TARGET_LDFLAGS) -Wl,-E" \
			-DCMAKE_FIND_ROOT_PATH=$(STAGING_DIR) \
			-DCMAKE_FIND_ROOT_PATH_MODE_PROGRAM=BOTH \
			-DCMAKE_FIND_ROOT_PATH_MODE_LIBRARY=ONLY \
			-DCMAKE_FIND_ROOT_PATH_MODE_INCLUDE=ONLY \
			$(LUVI_OPTS) \
			-DWithSharedLibluv=ON \
			-DLUAJIT_INCLUDE_DIR="$(STAGING_DIR)/usr/include/luajit-$(shell luajit -v | grep -oP '(?<=LuaJIT )[0-9]+\.[0-9]+')" \
		. \
	)
endef

define Build/InstallDev
	$(INSTALL_DIR) $(STAGING_DIR)/usr/share
	$(CP) $(PKG_BUILD_DIR)/luvi $(STAGING_DIR)/usr/share/luvi
endef

ifeq ($(CONFIG_LUVI_NO_LUVI),y)
  define Package/luvi/install
  endef
else
  define Package/luvi/install
	$(INSTALL_DIR) $(1)/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/luvi $(1)/bin/luvi
  endef
endif

$(eval $(call BuildPackage,luvi))
