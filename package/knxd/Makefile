include $(TOPDIR)/rules.mk

PKG_NAME:=knxd
PKG_VERSION:=1.18
PKG_RELEASE:=1

PKG_SOURCE_PROTO:=git

PKG_SOURCE_URL:=https://github.com/knxd/knxd.git
PKG_REV:=1fae3f6618d812ccc280a266b5c2287adcd0596d

PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

PKG_MIRROR:=0

include $(INCLUDE_DIR)/package.mk

define Package/knxd
	SECTION:=net
	CATEGORY:=Network
	TITLE:=Eib KNX deamon
	DEPENDS:=+pthsem +argp-standalone +libstdcpp
endef

define Package/knxd/description
  EIB KNX Daemon
endef

define Package/knxd-utils
	SECTION:=net
	CATEGORY:=Network
	TITLE:=Eib KNX Utils
	DEPENDS:=+pthsem +libstdcpp
endef

define Package/knxd-utils/description
  EIB KNX Utils
endef
define Build/Prepare
	$(call Build/Prepare/Default)
#	( cd $(PKG_BUILD_DIR); autoreconf --install --force )
	( cd $(PKG_BUILD_DIR); libtoolize --copy --force --install && aclocal -I m4 --force && autoheader && automake --add-missing --copy --force-missing && autoconf )
endef

define Build/Configure
	$(call Build/Configure/Default,\
		--enable-onlyeibd \
		--disable-ft12 \
		--enable-eibnetip \
		--enable-eibnetiptunnel \
		--enable-eibnetipserver \
		--enable-usb \
		--disable-tpuart \
		--enable-tpuarts \
		--disable-pei16s \
		--disable-java \
		--enable-groupcache \
		--without-pth-test \
		--without-libstdc )
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		LIBS="-L$(STAGING_DIR)/usr/lib -fno-builtin -nostdinc++ -nodefaultlibs -lc -lm -lgcc -lstdc++ -largp -lpthsem " \
		CPPFLAGS="-I$(STAGING_DIR)/include -I$(STAGING_DIR)/usr/include"
endef

define Package/knxd/install
	mkdir -p $(1)/usr/bin
	$(CP) $(PKG_BUILD_DIR)/src/server/.libs/knxd $(1)/usr/bin/
endef

define Package/knxd-utils/install	
	mkdir -p $(1)/usr/lib
endef

#$(eval $(call RequireHeader,/usr/include/gnu/stubs-32.h,$(PKG_NAME) requires a 32-bits toolchain. ))

$(eval $(call BuildPackage,knxd))
$(eval $(call BuildPackage,knxd-utils))
