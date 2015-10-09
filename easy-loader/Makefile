#
# Copyright (C) 2015 OpenWrt
# See LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=easy-loader
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/easy-loader
  SECTION:=utils
  CATEGORY:=Utilities
  DEPENDS:=
  TITLE:= Wellington Bootlaoer CLI
  DEPENDS:=
  MAINTAINER:=Gerhard Bertelsmann
endef

define Package/easy-loader/description
  Command line interface for Wellington Bootlaoer
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

define Package/easy-loader/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/easy-loader $(1)/usr/bin/
endef

$(eval $(call BuildPackage,easy-loader))
