# Copyright (C) 2013 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=jalv2
PKG_VERSION:=4p
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)$(PKG_VERSION)src.zip
PKG_SOURCE_URL:=http://www.casadeyork.com/jalv2/archive
PKG_MD5SUM:=5b99f723011aedcc26dffcf7e6655970
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)$(PKG_VERSION)

PKG_INSTALL:=1
PKG_BUILD_PARALLEL:=1

include $(INCLUDE_DIR)/package.mk

PKG_UNPACK:=unzip -o $(DL_DIR)/$(PKG_SOURCE)

# Sart make install at build time

define Package/jalv2
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=jalv2
  DEPENDS:=
endef

define Package/jalv2/description
  JALv2 is a high-level language for a number of Microchip PIC
endef


define Build/Prepare
	$(PKG_UNPACK)
	mkdir -p $(PKG_BUILD_DIR)/bin
	$(CP) $(PKG_NAME)$(PKG_VERSION)/src $(PKG_BUILD_DIR)
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR)/src \
		$(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(TARGET_CFLAGS) $(TARGET_CPPFLAGS) -ansi"
endef

define Package/jalv2/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/bin/jalv2 $(1)/usr/bin/
endef


$(eval $(call BuildPackage,jalv2))
