/*
 * Copyright (C) 2012 Freescale Semiconductor, Inc.
 *
 * Configuration settings for the Freescale i.MX6Q Marta board.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __MX6QMARTA_CONFIG_H
#define __MX6QMARTA_CONFIG_H

#define CONFIG_MACH_TYPE	3529
#define CONFIG_MXC_UART_BASE	UART3_BASE
#define CONSOLE_DEV		"ttymxc2"
#define CONFIG_MMCROOT			"/dev/mmcblk0p1"
#define CONFIG_MMC_TRACE

/* USB Configs */
/*#define CONFIG_USB_EHCI*/
/*#define CONFIG_USB_EHCI_MX6*/
/*#define CONFIG_USB_HOST_ETHER*/
/*#define CONFIG_USB_ETHER_ASIX*/
/*#define CONFIG_USB_MAX_CONTROLLER_COUNT 2*/
/*#define CONFIG_EHCI_HCD_INIT_AFTER_RESET*/	/* For OTG port */
/*#define CONFIG_MXC_USB_PORTSC	(PORT_PTS_UTMI | PORT_PTS_PTW)*/
/*#define CONFIG_MXC_USB_FLAGS	0*/

/*#define CONFIG_PCA953X*/
/*#define CONFIG_SYS_I2C_PCA953X_WIDTH	{ {0x30, 8}, {0x32, 8}, {0x34, 8} }*/

#include "mx6qmarta_common.h"

/*#undef CONFIG_SYS_NO_FLASH*/
/*#define CONFIG_SYS_FLASH_BASE           WEIM_ARB_BASE_ADDR*/
/*#define CONFIG_SYS_FLASH_SECT_SIZE      (128 * 1024)*/
/*#define CONFIG_SYS_MAX_FLASH_BANKS 1*/    /* max number of memory banks */
/*#define CONFIG_SYS_MAX_FLASH_SECT 256 */  /* max number of sectors on one chip */
/*#define CONFIG_SYS_FLASH_CFI*/            /* Flash memory is CFI compliant */
/*#define CONFIG_FLASH_CFI_DRIVER */        /* Use drivers/cfi_flash.c */
/*#define CONFIG_SYS_FLASH_USE_BUFFER_WRITE*/ /* Use buffered writes*/
/*#define CONFIG_SYS_FLASH_EMPTY_INFO*/

#define CONFIG_SYS_FSL_USDHC_NUM	2
#if defined(CONFIG_ENV_IS_IN_MMC)
#define CONFIG_SYS_MMC_ENV_DEV		0
#endif

/* I2C Configs */
/*#define CONFIG_SYS_I2C*/
/*#define CONFIG_SYS_I2C_MXC*/
/*#define CONFIG_SYS_I2C_MXC_I2C1*/		/* enable I2C bus 1 */
/*#define CONFIG_SYS_I2C_MXC_I2C2*/		/* enable I2C bus 2 */
/*#define CONFIG_SYS_I2C_MXC_I2C3*/		/* enable I2C bus 3 */
/*#define CONFIG_SYS_I2C_SPEED*/  /*100000*/

/* NAND flash command */
/*#define CONFIG_CMD_NAND*/
/*#define CONFIG_CMD_NAND_TRIMFFS*/

/* NAND stuff */
/*#define CONFIG_NAND_MXS*/
/*#define CONFIG_SYS_MAX_NAND_DEVICE     1*/
/*#define CONFIG_SYS_NAND_BASE           0x40000000*/
/*#define CONFIG_SYS_NAND_5_ADDR_CYCLE*/
/*#define CONFIG_SYS_NAND_ONFI_DETECTION*/

/* DMA stuff, needed for GPMI/MXS NAND support */
/*#define CONFIG_APBH_DMA*/
/*#define CONFIG_APBH_DMA_BURST*/
/*#define CONFIG_APBH_DMA_BURST8*/

/* PMIC */
/*#define CONFIG_POWER*/
/*#define CONFIG_POWER_I2C*/
/*#define CONFIG_POWER_PFUZE100*/
/*#define CONFIG_POWER_PFUZE100_I2C_ADDR	0x08*/

#ifdef CONFIG_DEFAULT_FDT_FILE
#undef CONFIG_DEFAULT_FDT_FILE
#endif

#define CONFIG_DEFAULT_FDT_FILE	"imx6q-marta.dtb"

#ifdef CONFIG_EXTRA_ENV_SETTINGS
#undef CONFIG_EXTRA_ENV_SETTINGS
#endif

#define CONFIG_MFG_ENV_SETTINGS \
        "mfgtool_args=setenv bootargs console=${console},${baudrate} " \
                "rdinit=/linuxrc " \
                "g_mass_storage.stall=0 g_mass_storage.removable=1 " \
                "g_mass_storage.idVendor=0x066F g_mass_storage.idProduct=0x37FF "\
                "g_mass_storage.iSerialNumber=\"\" "\
                "enable_wait_mode=off "\
                "\0" \
                "initrd_addr=0x12C00000\0" \
                "initrd_high=0xffffffff\0" \
                "bootcmd_mfg=run mfgtool_args;bootm ${loadaddr} ${initrd_addr} ${fdt_addr};\0" \
                
#define CONFIG_EXTRA_ENV_SETTINGS \
  CONFIG_MFG_ENV_SETTINGS \
  "fdt_addr=0x18000000\0" \
  "fdt_high=0xffffffff\0"   \
  "mmc_recovery_partition=1\0"\
  "mmc_normal_partition=1\0"\
  "uimage=/boot/uImage\0"\
  "fdt=/boot/" CONFIG_DEFAULT_FDT_FILE "\0"\
  "console=" CONSOLE_DEV "\0"\
  "mmcdev=0\0"\
  "bootargs_recovery=console=" CONSOLE_DEV ",115200 root=" CONFIG_MMCROOT " ro rootwait lpj=7905280 quiet\0"\
  "bootargs_normal=console=" CONSOLE_DEV ",115200 root=" CONFIG_MMCROOT " ro rootwait lpj=7905280 quiet\0"\
  "bootcmd_reset=mw.b 0x20000000 0 0x10000; mmc write 0x20000000 0x0 0x10000; reset\0"\
  "bootcmd_normal=setenv bootargs ${bootargs_normal}; ext2load mmc ${mmcdev}:${mmc_normal_partition} ${loadaddr} ${uimage}; ext2load mmc ${mmcdev}:${mmc_normal_partition} ${fdt_addr} ${fdt}; bootm ${loadaddr} - ${fdt_addr}\0"\
  "bootcmd_recovery= setenv bootargs ${bootargs_recovery}; ext2load mmc ${mmcdev}:${mmc_recovery_partition} ${loadaddr} ${uimage}; ext2load mmc ${mmcdev}:${mmc_recovery_partition} ${fdt_addr} ${fdt}; bootm ${loadaddr} - ${fdt_addr}\0"\
  "bootcmd=run bootcmd_normal\0" \
  "netdev=eth0\0" \
  "ethprime=FEC\0" \
  "ethact=FEC\0" \
  "fec_addr=00:50:C2:7E:90:F0\0" \
  "ethaddr=00:00:11:22:33:44\0"

#define CONFIG_PHY_MARVELL
#define CONFIG_IPADDR	192.168.168.200
#define CONFIG_ETHADDR 00:11:22:00:11:22

#endif                         /* __MX6QMARTA_CONFIG_H */
