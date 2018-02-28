# Makefile for startup script to run ISV files

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/package.mk

PKG_NAME:=startup
PKG_VERSION:=1.0

STARTUP_DEPENDS:=

define Package/startup/Default
  SUBMENU:=Combine CloudGate packages
  SECTION:=base
  CATEGORY:=Base system
  MAINTAINER:=Option <support@option.com>
endef

define Package/startup
  $(call Package/startup/Default)
  TITLE:=Include startup isv script
  DEPENDS:=$(STARTUP_DEPENDS)
endef

define Package/startup/config
	choice
		depends on PACKAGE_startup
		prompt "ISV file"

	config STARTUP_AUTOSTART_NODEBUG
		bool "start"

	config STARTUP_AUTOSTART_DEBUG
		bool "start/debug/telnet"

	config STARTUP_NOAUTOSTART_DEBUG
		bool "debug/telnet"

	endchoice
endef

define BuildModule
  define Package/startup-app-$(1)
    $(call Package/startup/Default)
	DEPENDS:=startup

    ifneq ($(3),)
      STARTUP_DEPENDS+=+PACKAGE_startup-app-$(1):$(3)
    endif

    TITLE:=Startup $(2)
  endef

  define Package/startup-app-$(1)/install
	$(INSTALL_DIR) $$(1)/
  endef

  $$(eval $$(call BuildPackage,startup-app-$(1)))
endef

define Build/Prepare
endef

define Build/Configure
endef

define Build/Compile
endef

ifeq ($(CONFIG_STARTUP_AUTOSTART_DEBUG),y)
  define Package/startup/install/isv
	$(INSTALL_BIN) files/isv.start_debug.sh $(1)/isv
  endef
else ifeq ($(CONFIG_STARTUP_NOAUTOSTART_DEBUG),y)
  define Package/startup/install/isv
	$(INSTALL_BIN) files/isv.nostart_debug.sh $(1)/isv
  endef
else ifeq ($(CONFIG_STARTUP_AUTOSTART_NODEBUG),y)
  define Package/startup/install/isv
	$(INSTALL_BIN) files/isv.start_nodebug.sh $(1)/isv
  endef
endif

define Package/startup/install
	$(call Package/startup/install/isv,$(1))
endef

$(eval $(call BuildModule,root_enabler,Root Enabler,root_enabler))
$(eval $(call BuildModule,luvit-red,LuvitRED,luvit-bin))
$(eval $(call BuildModule,sras,SRAS,sras))
$(eval $(call BuildModule,smartcity,Smart City,smartcity))
$(eval $(call BuildModule,iMonnit,iMonnit,CG9104-900MT))
$(eval $(call BuildPackage,startup))

# Execute the compile & install step on every 'make'
.PHONY: $(STAMP_BUILT) $(STAMP_INSTALLED)
