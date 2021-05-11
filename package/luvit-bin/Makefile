# Makefile for luvit-bin package

include $(TOPDIR)/rules.mk

PKG_NAME:=luvit-bin
PKG_VERSION:=1.0
PKG_RELEASE:=0

include $(INCLUDE_DIR)/package.mk

define Package/luvit-bin
  SECTION:= base
  CATEGORY:= Base system
  TITLE:= Luvit-red (binary)
  DEPENDS:=+snmpconfig +libuv +luv +luvi +owire +luvit-red-config +leveldb +luajit +libexpat +pthsem +knxd +qpid-proton +lora +lora_sx1302 +ble_fw_loader +net-snmp +vsftpd-tls
endef

define Build/Prepare
endef

define Build/Compile
endef

define Build/InstallDev
endef

define Package/luvit-bin/install
	tar -xvzf files/luvit-bin.tar.gz -C $(1)
endef

$(eval $(call BuildPackage,luvit-bin))
