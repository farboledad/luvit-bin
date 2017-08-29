
include $(TOPDIR)/rules.mk

PKG_NAME:=libuv
PKG_VERSION:=1.12.0
PKG_RELEASE:=1

PKG_SOURCE_PROTO:=git

PKG_SOURCE_URL:=https://github.com/luvitred/libuv.git
PKG_REV:=88cbbbbbb603d040ab94d9c505f446542b5edb06

PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE:=$(PKG_NAME)-$(PKG_REV).tar.gz
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_SOURCE_SUBDIR)

include $(INCLUDE_DIR)/package.mk

# CONFIGURE_ARGS += --enable-static --disable-shared
TARGET_CFLAGS += -O0 -g -pthread
TARGET_LDFLAGS += -pthread

define Package/libuv
  SECTION:=libs
  CATEGORY:=Libraries
  URL:=https://github.com/libuv/libuv
  TITLE:=libuv is a new platform layer for Node
endef

define Package/libuv/description
  libuv is a new platform layer for Node. Its purpose is to abstract libev
  on Unix systems.
endef

# Do not attempt default downloads. Directly clone from git.
define Download/Defaults
endef

define Build/Configure
	(cd $(PKG_BUILD_DIR); echo "Running autogen.sh ..."; ./autogen.sh );
	$(call Build/Configure/Default)
endef

define Build/Compile
	$(MAKE_VARS) \
	$(MAKE) -C $(PKG_BUILD_DIR)/$(MAKE_PATH) \
		$(MAKE_FLAGS) \
		libuv.la test/run-tests;
endef

define Build/InstallDev
	$(INSTALL_DIR) $(STAGING_DIR)/usr/include
	$(INSTALL_DIR) $(STAGING_DIR)/usr/include/uv
#	$(CP) -r $(PKG_BUILD_DIR)/include/* $(STAGING_DIR)/usr/include
	$(CP) $(PKG_BUILD_DIR)/include/uv-linux.h $(STAGING_DIR)/usr/include
	$(CP) $(PKG_BUILD_DIR)/include/uv-unix.h $(STAGING_DIR)/usr/include
	$(CP) $(PKG_BUILD_DIR)/include/uv-errno.h $(STAGING_DIR)/usr/include
	$(CP) $(PKG_BUILD_DIR)/include/uv-version.h $(STAGING_DIR)/usr/include
	$(CP) $(PKG_BUILD_DIR)/include/uv.h $(STAGING_DIR)/usr/include
	$(CP) $(PKG_BUILD_DIR)/include/uv-threadpool.h $(STAGING_DIR)/usr/include
	$(INSTALL_DIR) $(STAGING_DIR)/usr/lib
	$(CP) $(PKG_BUILD_DIR)/.libs/libuv.a $(STAGING_DIR)/usr/lib/
	$(CP) $(PKG_BUILD_DIR)/.libs/libuv.so* $(STAGING_DIR)/usr/lib/
endef

define Package/libuv/install
	$(INSTALL_DIR) $(1)/usr
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_BUILD_DIR)/.libs/libuv.so* $(1)/usr/lib
endef

$(eval $(call BuildPackage,libuv))
