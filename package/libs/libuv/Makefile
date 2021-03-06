
include $(TOPDIR)/rules.mk

PKG_NAME:=libuv
PKG_VERSION:=1.40
PKG_RELEASE:=1
PKG_SOURCE_PROTO:=git

PKG_SOURCE_URL:=https://github.com/luvitred/libuv.git
PKG_REV:=8d646e1440ea137947547d0bfab49f478e7e071c
PKG_BRANCH:=v$(PKG_VERSION)
PKG_DATE:="2020-09-20"

PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION)-$(PKG_REV).tar.gz
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_SOURCE_SUBDIR)

CMAKE_INSTALL:=0
CMAKE_OPTIONS+=-DLIBUV_BUILD_TESTS=OFF

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/cmake.mk

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

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include
	$(INSTALL_DIR) $(1)/usr/include/uv
	$(CP) $(PKG_BUILD_DIR)/include/uv/linux.h $(1)/usr/include/uv/
	$(CP) $(PKG_BUILD_DIR)/include/uv/unix.h $(1)/usr/include/uv/
	$(CP) $(PKG_BUILD_DIR)/include/uv/errno.h $(1)/usr/include/uv/
	$(CP) $(PKG_BUILD_DIR)/include/uv/version.h $(1)/usr/include/uv
	$(CP) $(PKG_BUILD_DIR)/include/uv/threadpool.h $(1)/usr/include/uv
	$(CP) $(PKG_BUILD_DIR)/include/uv.h $(1)/usr/include/
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_BUILD_DIR)/libuv_a.a $(1)/usr/lib/libuv.a
endef

$(eval $(call BuildPackage,libuv))
