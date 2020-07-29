
include $(TOPDIR)/rules.mk

PKG_NAME:=libuv
PKG_VERSION:=1.32
PKG_RELEASE:=1
PKG_SOURCE_PROTO:=git

PKG_SOURCE_URL:=https://github.com/luvitred/libuv.git
PKG_REV:=bf381d3c41cb37bb7c980a16979ea6456cdd1f37
PKG_BRANCH:=v$(PKG_VERSION)
PKG_DATE:="2019-07-01"

PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION)-$(PKG_REV).tar.gz
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_SOURCE_SUBDIR)

include $(INCLUDE_DIR)/package.mk

define Package/libuv
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:=libuv is a new platform layer for Node
  URL:=https://github.com/libuv/libuv
  MAINTAINER:=Option <support@option.com>
  DEPENDS:=+libpthread +librt
endef

define Package/libuv/description
  libuv is a new platform layer for Node. Its purpose is to abstract libev
  on Unix systems.
endef

# Do not attempt default downloads. Directly clone from git.
define Download/Defaults
endef

define DownloadMethod/git
    $(call wrap_mirror, \
        echo "Checking out files from the git repository..."; \
        mkdir -p $(TMP_DIR)/dl && \
        cd $(TMP_DIR)/dl && \
        rm -rf $(SUBDIR) && \
        [ \! -d $(SUBDIR) ] && \
        git clone -b $(PKG_BRANCH) --shallow-since=$(PKG_DATE) $(URL) $(SUBDIR) && \
        echo "Packing checkout..." && \
        rm -rf $(SUBDIR)/.git && \
        $(call dl_pack,$(TMP_DIR)/dl/$(FILE),$(SUBDIR)) && \
        mv $(TMP_DIR)/dl/$(FILE) $(DL_DIR)/ && \
        rm -rf $(SUBDIR); \
    )
endef

# CONFIGURE_ARGS += --enable-static --disable-shared
TARGET_CFLAGS += -pthread
TARGET_LDFLAGS += -pthread

define Build/Configure
	(cd $(PKG_BUILD_DIR); echo "Running autogen.sh ..."; ./autogen.sh );
	$(call Build/Configure/Default)
endef

define Build/Compile
	$(MAKE_VARS) \
	$(MAKE) -C $(PKG_BUILD_DIR)/$(MAKE_PATH) \
		$(MAKE_FLAGS) \
		libuv.la
endef

define Build/InstallDev
	$(INSTALL_DIR) $(STAGING_DIR)/usr/include
	$(INSTALL_DIR) $(STAGING_DIR)/usr/include/uv
	$(CP) $(PKG_BUILD_DIR)/include/uv/linux.h $(STAGING_DIR)/usr/include/uv/
	$(CP) $(PKG_BUILD_DIR)/include/uv/unix.h $(STAGING_DIR)/usr/include/uv/
	$(CP) $(PKG_BUILD_DIR)/include/uv/errno.h $(STAGING_DIR)/usr/include/uv/
	$(CP) $(PKG_BUILD_DIR)/include/uv/version.h $(STAGING_DIR)/usr/include/uv
	$(CP) $(PKG_BUILD_DIR)/include/uv/threadpool.h $(STAGING_DIR)/usr/include/uv
	$(CP) $(PKG_BUILD_DIR)/include/uv.h $(STAGING_DIR)/usr/include/
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
