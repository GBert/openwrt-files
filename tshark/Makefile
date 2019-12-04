#
# Copyright (C) 2019 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=wireshark
PKG_VERSION:=2.2.17
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2
PKG_MD5SUM:=fad6b60e570e7cd60331a780988a02ba
PKG_SOURCE_URL:=https://www.wireshark.org/download/src/all-versions

PKG_LICENSE:=MIT
PKG_LICENSE_FILES:=COPYING

PKG_FIXUP:=autoreconf


include $(INCLUDE_DIR)/package.mk

define Package/tshark
  SECTION:=net
  CATEGORY:=Network
  DEPENDS:=+librt +libcap +libpcap +glib2 +libgcrypt +libcares
  URL:=http://www.wireshark.org/
  TITLE:=Network monitoring and data tool
endef

CONFIGURE_ARGS+= \
	--enable-tshark \
	--enable-dumpcap \
	--enable-setuid-install \
 	--disable-gtk2 \
 	--disable-androiddump \
	--disable-randpktdump \
	--disable-ipv6 \
	--without-lua \
	--disable-glibtest \
	--without-plugins \
	--disable-wireshark \
	--disable-gtktest \
	--disable-editcap \
	--disable-capinfos \
	--disable-mergecap \
	--disable-text2pcap \
	--disable-idl2wrs \
	--disable-dftest \
	--disable-randpkt

TARGET_CFLAGS += -ffunction-sections -fdata-sections
TARGET_LDFLAGS += -Wl,--gc-sections

CONFIGURE_VARS += \
	BUILD_CC="$(TARGET_CC)" \
	HOSTCC="$(HOSTCC)" \
	td_cv_buggygetaddrinfo=no \
	ac_cv_linux_vers=$(LINUX_VERSION) \
	ac_cv_header_rpc_rpcent_h=no \
	ac_cv_lib_rpc_main=no \
	ac_cv_path_PCAP_CONFIG=""

MAKE_FLAGS += \
	CCOPT="$(TARGET_CFLAGS)" INCLS="-I. $(TARGET_CPPFLAGS)"

define Build/Compile
	cd $(PKG_BUILD_DIR)/tools/lemon&&make
	$(MAKE) $(PKG_JOBS) -C $(PKG_BUILD_DIR) \
		DESTDIR="$(PKG_INSTALL_DIR)" \
		CC="$(TARGET_CC)" \
		install
endef
	
define Package/tshark/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/tshark $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/captype $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/dumpcap $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/rawshark $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/reordercap $(1)/usr/bin/
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/lib*.so* $(1)/usr/lib
#	$(CP) $(PKG_INSTALL_DIR)/usr/lib/wireshark/plugins/$(PKG_VERSION)/*.so $(1)/usr/lib
endef

$(eval $(call BuildPackage,tshark))
