#
# Copyright (C) 2013 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=mc-icsp
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

define KernelPackage/mc-icsp
  SUBMENU:=Other modules
  TITLE:=Microchip ICSP Interface
  DEPENDS:=@GPIO_SUPPORT
  FILES:=$(PKG_BUILD_DIR)/mc-icsp.ko
  KCONFIG:=
endef

define KernelPackage/mc-icsp/description
 Kernel module for register a Microchip ICSP Interface
 Kernel module for register a custom spi-gpio platform device.
endef

EXTRA_KCONFIG:= \
    MC_ICSP=m

EXTRA_CFLAGS:= \
    $(patsubst CONFIG_%, -DCONFIG_%=1, $(patsubst %=m,%,$(filter %=m,$(EXTRA_KCONFIG)))) \
    $(patsubst CONFIG_%, -DCONFIG_%=1, $(patsubst %=y,%,$(filter %=y,$(EXTRA_KCONFIG)))) \

MAKE_OPTS:= \
    ARCH="$(LINUX_KARCH)" \
    CROSS_COMPILE="$(TARGET_CROSS)" \
    SUBDIRS="$(PKG_BUILD_DIR)" \
    EXTRA_CFLAGS="$(EXTRA_CFLAGS)" \
    $(EXTRA_KCONFIG)

define Build/Prepare
    mkdir -p $(PKG_BUILD_DIR)
    $(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
    $(MAKE) -C "$(LINUX_DIR)" \
        $(MAKE_OPTS) \
        modules
endef

$(eval $(call KernelPackage,mc-icsp))
