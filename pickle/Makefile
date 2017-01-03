# Copyright (C) 2014 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=pickle
PKG_RELEASE:=4.0e

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)
PKG_BUILD_PARALLEL:=1

include $(INCLUDE_DIR)/package.mk

define Package/pickle
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=Microchip PIC Programmer
  DEPENDS:=+libftdi1 +kmod-bit-bang-gpio
endef

define Package/pickle/description
  Darron Broad Microchip PIC Programmer 'pickle'
  Only LVP PICs are supported
endef

define Build/Prepare
	$(CP) ./src/* $(PKG_BUILD_DIR)
endef

TARGET_CFLAGS+= -I$(STAGING_DIR)/usr/include/libftdi1 \

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(TARGET_CFLAGS) $(TARGET_CPPFLAGS) -std=gnu99 -DP12 -DP14 -DP16 -DP24 -DP32 -DPIO -DPCTRL -DPTEST -DBITBANG -DFTDI -DTTY -DQSORT" \
		build
endef

define Package/pickle/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/pickle $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/pio $(1)/usr/bin/
	$(INSTALL_DIR) $(1)/root/.pickle
	$(INSTALL_BIN) ./config/RIPE_06_000201.hex $(1)/root/.pickle
	$(INSTALL_BIN) ./config/RIPE_11_000301.hex $(1)/root/.pickle
	$(INSTALL_BIN) ./config/config* $(1)/root/.pickle
	$(INSTALL_BIN) ./config/*.sh $(1)/root/.pickle
endef

define Package/pickle/postinst
	ln -sf /usr/bin/pickle $${IPKG_INSTROOT}/usr/bin/p12
	ln -sf /usr/bin/pickle $${IPKG_INSTROOT}/usr/bin/p14
	ln -sf /usr/bin/pickle $${IPKG_INSTROOT}/usr/bin/n14
	ln -sf /usr/bin/pickle $${IPKG_INSTROOT}/usr/bin/p16
	ln -sf /usr/bin/pickle $${IPKG_INSTROOT}/usr/bin/p24
	ln -sf /usr/bin/pickle $${IPKG_INSTROOT}/usr/bin/p32
	ln -sf /usr/bin/pickle $${IPKG_INSTROOT}/usr/bin/pctrl
	ln -sf /usr/bin/pickle $${IPKG_INSTROOT}/usr/bin/pload
	ln -sf /usr/bin/pickle $${IPKG_INSTROOT}/usr/bin/ptest
endef


$(eval $(call BuildPackage,pickle))
