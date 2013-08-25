# Copyright (C) 2013 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=picprog
PKG_RELEASE:=0.1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)
PKG_BUILD_PARALLEL:=1

include $(INCLUDE_DIR)/package.mk

define Package/picprog
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=Microchip PIC Programmer
endef

define Package/picprog/description
  Microchip PIC Programmer based on MC ICSP modul from Eran Duchan.
  This is a modifed version of Darron Broads k8048 PIC Programmer.
  Only some of PIC18F are supported so far.
endef

define Build/Prepare
	$(CP) ./src/* $(PKG_BUILD_DIR)
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(TARGET_CFLAGS) $(TARGET_CPPFLAGS)" \
		all
endef

define Package/picprog/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/k8048 $(1)/usr/bin/
endef

define Package/picprog/postinst
	ln -sf /usr/bin/k16 $${IPKG_INSTROOT}/k8048
endef


$(eval $(call BuildPackage,picprog))
