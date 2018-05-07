# Copyright (C) 2018 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=pickle
PKG_RELEASE:=4.1d

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
  Darron Broads Microchip PIC Programmer 'pickle'
  Only LVP PICs are supported
endef

define Build/Prepare
	$(CP) ./src/* $(PKG_BUILD_DIR)
endef

TARGET_CFLAGS+= -I$(STAGING_DIR)/usr/include/libftdi1

ifneq (,$(findstring banana,$(CONFIG_TARGET_PROFILE)))
	PICKLE_BPI:=1
	export PICKLE_BPI
	TARGET_CFLAGS_EXTENDED+=-DALLWINNER
endif

ifneq (,$(findstring rpi,$(CONFIG_TARGET_PROFILE)))
	PICKLE_RPI:=1
	export PICKLE_RPI
	TARGET_CFLAGS_EXTENDED+=-DRPI
endif

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(TARGET_CFLAGS) $(TARGET_CPPFLAGS) -std=gnu99 -DP12 -DP14 -DN14 -DP16 -DP24 -DP32 -DPIO \
		        -DPCTRL -DPTEST -DBITBANG -DFTDI -DCP2104 -DTTY -DQSORT $(TARGET_CFLAGS_EXTENDED)"
endef

define Package/pickle/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/pickle $(1)/usr/bin/
	$(LN) pickle $(1)/usr/bin/p12
	$(LN) pickle $(1)/usr/bin/p14
	$(LN) pickle $(1)/usr/bin/n14
	$(LN) pickle $(1)/usr/bin/p16
	$(LN) pickle $(1)/usr/bin/n16
	$(LN) pickle $(1)/usr/bin/p24
	$(LN) pickle $(1)/usr/bin/p32
	$(LN) pickle $(1)/usr/bin/pctrl
	$(LN) pickle $(1)/usr/bin/pload
	$(LN) pickle $(1)/usr/bin/ptest
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/pio $(1)/usr/bin/
	$(INSTALL_DIR) $(1)/root/.pickle
	$(INSTALL_BIN) ./config/RIPE_06_000201.hex $(1)/root/.pickle
	$(INSTALL_BIN) ./config/RIPE_11_000301.hex $(1)/root/.pickle
	$(INSTALL_BIN) ./config/config* $(1)/root/.pickle
	$(INSTALL_BIN) ./config/*.sh $(1)/root/.pickle
ifneq (,$(findstring wr841,$(CONFIG_TARGET_PROFILE)))
	$(INSTALL_BIN) ./config/config_wr841 $(1)/root/.pickle/config
endif
ifneq (,$(findstring mt7688,$(CONFIG_TARGET_SUBTARGET)))
	$(INSTALL_BIN) ./config/config_srse2 $(1)/root/.pickle/config
endif
endef

$(eval $(call BuildPackage,pickle))
