#
# Copyright (C) 2019 OpenWrt
# See LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=gpiod-test
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/gpiod-test
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:= GPIOD toggle test
  DEPENDS:= +libgpiod
  MAINTAINER:=Gerhard Bertelsmann
endef

define Package/gpiod-test/description
  GPIOD test
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

define Package/gpiod-test/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/gpiod-test $(1)/usr/bin/
endef

$(eval $(call BuildPackage,gpiod-test))
