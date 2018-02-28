# Makefile for ble_fw_loader package

include $(TOPDIR)/rules.mk

PKG_NAME:=ble_fw_loader
PKG_VERSION:=1.0

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/nls.mk

define Package/ble_fw_loader
  SECTION:=base
  CATEGORY:=Base system
  TITLE:=BLE firmware loader
  MAINTAINER:=Option <support@option.com>
  DEPENDS:=+libcg +libjson-c
endef

define Build/Prepare
	$(call Build/Prepare/Default)
	$(CP) -l src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) CC="$(TARGET_CC)" LD="$(TARGET_LD)" CFLAGS="$(TARGET_CFLAGS) $(TARGET_CPPFLAGS) $(FPIC)" LDFLAGS="$(TARGET_LDFLAGS)"
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/bin
	$(CP) $(PKG_BUILD_DIR)/ble_fw_loader $(1)/usr/bin/
endef

define Package/ble_fw_loader/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(CP) $(PKG_BUILD_DIR)/ble_fw_loader $(1)/usr/bin/
endef

$(eval $(call BuildPackage,ble_fw_loader))
