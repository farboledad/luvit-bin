include $(TOPDIR)/rules.mk

PKG_NAME:=lora
PKG_VERSION:=1.0

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
  SECTION:=net
  CATEGORY:=Network
  SUBMENU:=LoRaWAN
  TITLE:=Lora Gateway
  MAINTAINER:=Option <support@option.com>
  DEPENDS:=+argp-standalone +libpthread +librt
endef

define Package/$(PKG_NAME)/config
	source "$(SOURCE)/Config.in"
endef

define Build/Prepare
	$(call Build/Prepare/Default)
	mkdir -p $(PKG_BUILD_DIR)/lora_gateway
	$(CP) -l lora_gateway/* $(PKG_BUILD_DIR)/lora_gateway
	mkdir -p $(PKG_BUILD_DIR)/packet_forwarder
	$(CP) -l packet_forwarder/* $(PKG_BUILD_DIR)/packet_forwarder
endef

OPTS = CC="$(TARGET_CC)" \
		LD="$(TARGET_LD)" \
		CFLAGS="$(TARGET_CFLAGS) $(TARGET_CPPFLAGS) $(FPIC) -Wfatal-errors -Wall -Wextra -std=c99 -Iinc -I." \
		LIBS="$(TARGET_LDFLAGS) -lloragw -lrt -lm -lpthread -largp -lcg"

define Build/Compile
	# compile test tools
ifeq ($(CONFIG_LORA_TEST),y)
	$(MAKE) -C $(PKG_BUILD_DIR)/lora_gateway $(OPTS)
	$(MAKE) -C $(PKG_BUILD_DIR)/packet_forwarder $(OPTS)
else
	$(MAKE) gw  -C $(PKG_BUILD_DIR)/lora_gateway $(OPTS)
	$(MAKE) fwd -C $(PKG_BUILD_DIR)/packet_forwarder $(OPTS)
endif
endef

define Package/$(PKG_NAME)/install/bin
	$(INSTALL_DIR) $(1)/usr/bin/lora
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/packet_forwarder/lora_pkt_fwd/lora_pkt_fwd $(1)/usr/bin/lora/lora_pkt_fwd_sx1301
	$(CP) $(PKG_BUILD_DIR)/packet_forwarder/lora_pkt_fwd/global_conf_*.json $(1)/usr/bin/lora
endef

define Package/$(PKG_NAME)/install/test
	$(INSTALL_DIR) $(1)/usr/bin/lora
	# lora_gateway test tools
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/lora_gateway/libloragw/test_loragw_hal $(1)/usr/bin/lora
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/lora_gateway/libloragw/test_loragw_reg $(1)/usr/bin/lora
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/lora_gateway/libloragw/test_loragw_cal $(1)/usr/bin/lora
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/lora_gateway/libloragw/test_loragw_spi $(1)/usr/bin/lora
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/lora_gateway/libloragw/test_loragw_gps $(1)/usr/bin/lora
	# lora_gateway utils
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/lora_gateway/util_catch_pkt/util_catch_pkt $(1)/usr/bin/lora
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/lora_gateway/util_pkt_logger/util_pkt_logger $(1)/usr/bin/lora
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/lora_gateway/util_spi_stress/util_spi_stress $(1)/usr/bin/lora
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/lora_gateway/util_tx_continuous/util_tx_continuous $(1)/usr/bin/lora
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/lora_gateway/util_tx_test/util_tx_test $(1)/usr/bin/lora
	# packet_forwarder test tools
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/packet_forwarder/util_ack/util_ack $(1)/usr/bin/lora/fwd_util_ack
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/packet_forwarder/util_sink/util_sink $(1)/usr/bin/lora/fwd_util_sink
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/packet_forwarder/util_tx_test/util_tx_test $(1)/usr/bin/lora/fwd_util_tx_test
endef

define Package/$(PKG_NAME)/install
	$(call Package/lora/install/bin, $(1))
ifeq ($(CONFIG_LORA_TEST),y)
	$(call Package/lora/install/test, $(1))
endif
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
