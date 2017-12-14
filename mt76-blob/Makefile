# Copyright (C) 2017 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=mt76-blob
PKG_RELEASE:=1.0

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)
PKG_BUILD_PARALLEL:=1

include $(INCLUDE_DIR)/package.mk

define Package/mt76-blob
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=Property WiFi driver for MT76xx
  DEPENDS:=
endef

define Package/mt76-blob/description
  Property WiFi driver and tools for MT76xx from vonger.cn
  Kernel 4.9.65 only
endef

define Build/Compile
endef

define Package/mt76-blob/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) bins/iwconfig $(1)/usr/sbin/
	$(INSTALL_DIR) $(1)/etc/wireless/mt7628
	$(INSTALL_BIN) ./etc/* $(1)/etc/wireless/mt7628
	$(INSTALL_DIR) $(1)/lib/modules/4.9.65
	$(INSTALL_BIN) bins/mt7628.ko $(1)/lib/modules/4.9.65
endef

define Package/mt76-blob/postinst
	ln -sf /usr/sbin/iwconfig $${IPKG_INSTROOT}/usr/sbin/iwgetid
	ln -sf /usr/sbin/iwconfig $${IPKG_INSTROOT}/usr/sbin/iwlist
	ln -sf /usr/sbin/iwconfig $${IPKG_INSTROOT}/usr/sbin/iwpriv
	ln -sf /usr/sbin/iwconfig $${IPKG_INSTROOT}/usr/sbin/iwspy
endef

$(eval $(call BuildPackage,mt76-blob))
