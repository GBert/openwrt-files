#
# Copyright (C) 2017 OpenWrt
# See LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=websockify
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/websockify
  SECTION:=utils
  CATEGORY:=Utilities
  SUBMENU:=Network
  TITLE:=simple TCP Websocket Gatway
  DEPENDS:=+libopenssl
  MAINTAINER:=Gerhard Bertelsmann
endef

define Package/websockify/description
  simple Websocket TCP Gateway from https://github.com/novnc/websockify/tree/master/other
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

define Package/websockify/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/websockify $(1)/usr/bin/
endef

$(eval $(call BuildPackage,websockify))
