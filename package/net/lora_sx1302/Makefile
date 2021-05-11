include $(TOPDIR)/rules.mk

PKG_NAME:=lora_sx1302
PKG_VERSION:=2.0.1

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
  SECTION:=net
  CATEGORY:=Network
  SUBMENU:=LoRaWAN
  TITLE:=Lora Gateway sx1302
  MAINTAINER:=Option <support@option.com>
  DEPENDS:= +libpthread +librt
endef

define Package/$(PKG_NAME)/config
	menu "Configuration"
		depends on PACKAGE_lora_sx1302

	config LORA_SX1302_TEST
		bool "Install test executables"
		default n

	endmenu
endef

define Build/Prepare
	$(call Build/Prepare/Default)
	mkdir -p $(PKG_BUILD_DIR)/libloragw/
	$(CP) -l libloragw/* $(PKG_BUILD_DIR)/libloragw
	mkdir -p $(PKG_BUILD_DIR)/libtools/
	$(CP) -l libtools/* $(PKG_BUILD_DIR)/libtools
	mkdir -p $(PKG_BUILD_DIR)/packet_forwarder
	$(CP) -l packet_forwarder/* $(PKG_BUILD_DIR)/packet_forwarder
	$(CP) -l reset_lgw.sh $(PKG_BUILD_DIR)/
endef

CFLAG_INCLUDES:=-I$(PKG_BUILD_DIR)/libtools/inc -I$(PKG_BUILD_DIR)/libloragw/inc -I$(PKG_BUILD_DIR)/packet_forwarder/inc

OPTS:=$(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(TARGET_CFLAGS) $(CFLAG_INCLUDES) $(FPIC) -Wfatal-errors -Wall -Wextra -std=c99" \
		LIBS="$(TARGET_LDFLAGS) -lloragw -ltinymt32 -lbase64 -lparson -lrt -lm -lpthread" \
		VERSION="$(PKG_VERSION)"

define Build/Compile
	$(MAKE) all -C $(PKG_BUILD_DIR)/libtools $(OPTS)
	$(MAKE) all -C $(PKG_BUILD_DIR)/libloragw $(OPTS)
	$(MAKE) all -C $(PKG_BUILD_DIR)/packet_forwarder $(OPTS)
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/bin/lora
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/packet_forwarder/lora_pkt_fwd $(1)/usr/bin/lora/lora_pkt_fwd_sx1302
	$(CP) $(PKG_BUILD_DIR)/packet_forwarder/config/global_conf_sx1302_*.json $(1)/usr/bin/lora
ifeq ($(CONFIG_LORA_SX1302_TEST),y)
	$(INSTALL_DIR) $(1)/usr/bin/lora/test
	$(CP) $(PKG_BUILD_DIR)/libloragw/test_loragw_* $(1)/usr/bin/lora/test
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/reset_lgw.sh $(1)/usr/bin/lora/test
endif
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
