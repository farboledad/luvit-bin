include $(TOPDIR)/rules.mk

PKG_NAME:=snmpconfig
PKG_VERSION:=1.0

include $(INCLUDE_DIR)/package.mk

define Package/snmpconfig
  SECTION:=base
  CATEGORY:=Base system
  TITLE:=SNMP config screen
  DEPENDS:=+snmpd-static
endef

define Build/Prepare
	$(call Build/Prepare/Default)
	$(CP) -l src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) CC="$(TARGET_CC)" LD="$(TARGET_LD)" CFLAGS="$(TARGET_CFLAGS) $(TARGET_CPPFLAGS) $(FPIC)" LDFLAGS="$(TARGET_LDFLAGS)"
endef

define Package/snmpconfig/install
	$(INSTALL_DIR) $(1)/bin
	$(CP) $(PKG_BUILD_DIR)/snmpconfig $(1)/bin/
	$(INSTALL_DIR) $(1)/www
	$(INSTALL_DIR) $(1)/www/snmp
	$(INSTALL_DIR) $(1)/www/snmp/js
	$(CP) www/snmp.html $(1)/www/snmp
	$(CP) www/*MIB $(1)/www/snmp
	$(CP) www/snmp.js $(1)/www/snmp/js
endef

$(eval $(call BuildPackage,snmpconfig))
