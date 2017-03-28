#
# Copyright (C) 2017 OpenWrt
# See LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=fast-gpio
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/fast-gpio
  SECTION:=utils
  CATEGORY:=Utilities
  DEPENDS:=
  TITLE:= Fast GPIO for Omega boards
  DEPENDS:= +libstdcpp
  MAINTAINER:=Gerhard Bertelsmann
endef

define Package/fast-gpio/description
  Fast GPIO access for Omega boards
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	make -C $(PKG_BUILD_DIR)		\
		$(TARGET_CONFIGURE_OPTS)	\
		CFLAGS="$(TARGET_CFLAGS) $(TARGET_CPPFLAGS)"	\
		LIBS="$(TARGET_LDFLAGS)"
endef

define Package/fast-gpio/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/fast-gpio $(1)/usr/bin/
endef

$(eval $(call BuildPackage,fast-gpio))
