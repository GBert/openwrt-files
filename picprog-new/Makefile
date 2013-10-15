# Copyright (C) 2013 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=picprog-new
PKG_RELEASE:=2.15-dev

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)
PKG_BUILD_PARALLEL:=1

include $(INCLUDE_DIR)/package.mk

define Package/picprog-new
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=Microchip PIC Programmer
  DEPENDS:=+kmod-mc-icsp +kmod-mc-icsp-gpio-custom
endef

define Package/picprog-new/description
  Microchip PIC Programmer based on MC ICSP modul from Eran Duchan.
  This is a modifed version of Darron Broads k8048 PIC Programmer.
  Only LVP PICs are supported
endef

define Build/Prepare
	$(CP) ./k8048/Src/* $(PKG_BUILD_DIR)
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(TARGET_CFLAGS) $(TARGET_CPPFLAGS) -std=gnu99 -DK14 -DK16 -DKIO -DKTEST -DLPICP" \
		build
endef

define Package/picprog-new/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/k8048 $(1)/usr/bin/
	$(INSTALL_DIR) $(1)/root
	$(INSTALL_BIN) ./config/k8048 $(1)/root/.k8048
endef

define Package/picprog-new/postinst
	ln -sf /usr/bin/k8048 $${IPKG_INSTROOT}/usr/bin/k14
	ln -sf /usr/bin/k8048 $${IPKG_INSTROOT}/usr/bin/k16
	ln -sf /usr/bin/k8048 $${IPKG_INSTROOT}/usr/bin/kio
	ln -sf /usr/bin/k8048 $${IPKG_INSTROOT}/usr/bin/ktest
endef


$(eval $(call BuildPackage,picprog-new))
