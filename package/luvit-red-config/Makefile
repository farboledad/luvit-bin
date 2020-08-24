# Makefile for luvit-red sdk config

include $(TOPDIR)/rules.mk

PKG_NAME:=luvit-red-config
PKG_VERSION:=1.0

include $(INCLUDE_DIR)/package.mk

define Package/luvit-red-config
  SECTION:=base
  CATEGORY:=Base system
  TITLE:=luvit-red-config
  DEPENDS:=
endef

define Build/Prepare
	$(call Build/Prepare/Default)
	$(CP) -r config/ $(PKG_BUILD_DIR)/
endef

define Build/Compile
endef

define Package/luvit-red-config/install
	rm -rf $(TOPDIR)/config/
	$(INSTALL_DIR) $(TOPDIR)/config/config/
	$(CP) config/* $(TOPDIR)/config/config/
endef

$(eval $(call BuildPackage,luvit-red-config))
