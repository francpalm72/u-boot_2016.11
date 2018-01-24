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

#define CONFIG_MMCROOT	"/dev/mmcblk3p1"	//mmcbkl2p1=SD3   mmcblk3p1=SD4

/*#define CONFIG_MMC_TRACE*/

#include "mx6qmarta_common.h"

#define CONFIG_SYS_FSL_USDHC_NUM	2
#if defined(CONFIG_ENV_IS_IN_MMC)
#define CONFIG_SYS_MMC_ENV_DEV		1	//0=SD3   1=SD4
#endif

#ifdef CONFIG_DEFAULT_FDT_FILE
#undef CONFIG_DEFAULT_FDT_FILE
#endif

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
	"console=ttymxc2\0" \
	"mmcdev=" __stringify(CONFIG_SYS_MMC_ENV_DEV) "\0" \
	"mmcpart=1\0" \
	"mmcroot=" CONFIG_MMCROOT " ro rootwait lpj=7905280 quiet\0" \
	"mmcautodetect=yes\0" \
	"mmcargs=setenv bootargs console=${console},${baudrate} root=${mmcroot} \0" \
	"netargs=setenv bootargs console=${console},${baudrate} root=/dev/nfs nfsroot=${serverip}:${nfsroot},v3,tcp\0" \
	"image=/boot/zImage\0" \
	"fdt_file=/boot/imx6q-marta.dtb\0" \
	"fdt_addr=0x18000000\0" \
	"netdev=eth0\0" \
	"ethprime=FEC\0" \
	"ethact=FEC\0" \
	"fec_addr=00:50:C2:7E:90:F0\0" \
	"ethaddr=00:00:11:22:33:44\0" \
	"loadimage=ext2load mmc ${mmcdev}:${mmcpart} ${loadaddr} ${image}\0" \
	"loadfdt=ext2load mmc ${mmcdev}:${mmcpart} ${fdt_addr} ${fdt_file}\0" \
	"mmcboot=echo Booting from mmc ...; " \
		 "run mmcargs; " \
		 "run loadimage;" \
		 "run loadfdt; " \
		 "bootz ${loadaddr} - ${fdt_addr}; \0" \
	"netboot=echo Booting from net ...; " \
		"run netargs; " \
		"tftp ${image}; " \
		"tftp ${fdt_addr} ${fdt_file};" \
		"bootz ${loadaddr} - ${fdt_addr}; \0" \
	"ipaddr=192.168.2.100\0" \
	"serverip=192.168.2.101\0" \
	"development=0\0" \
	"silentconsole=0\0" \
	"loadaddrmd5=0x11018000\0" \
	"loadaddrbin=0x11020000\0" \
	"operativedir=/boot/\0" \
	"operativeimg=img_dd1.boot\0" \
	"operativeimgmd5=img_dd1.md5\0" \
	"maintimg=img_dd1.boot\0" \
	"ubootimg=u-boot.imx\0" \
	"updatebin=tftpboot ${loadaddrmd5} ${serverip}:${operativeimgmd5}; " \
			"if test ${filesize} = 10; then " \
				"tftpboot ${loadaddrbin} ${serverip}:${operativeimg}; " \
				"if test md5sum -v ${loadaddrbin} ${filesize} *${loadaddrmd5}; then " \
					"echo md5sum check OK; " \
					"ext4write mmc ${mmcdev}:${mmcpart} ${loadaddrbin} ${operativedir}${operativeimg} ${filesize}; " \
					"ext4write mmc ${mmcdev}:${mmcpart} ${loadaddrmd5} ${operativedir}${operativeimgmd5} 10; " \
				"else " \
					"echo md5sum check FAILED: ; " \
					"md5sum ${loadaddrbin} ${filesize}; " \
				"fi; " \
			"else " \
				"echo md5 file error length; " \
				"echo ${filesize}; " \
			"fi; \0" \
	"updateboot=tftpboot ${loadaddrbin} ${serverip}:${ubootimg}; " \
			"mmc dev ${mmcdev}; " \
			"mmc write ${loadaddrbin} 0x2 4000;\0" \
	"bootcmd_maint=tftpboot ${loadaddrbin} ${serverip}:${maintimg}; bootm ${loadaddrbin}\0" \
	"bootcmd_operdev=tftpboot ${loadaddrbin} ${serverip}:${operativeimg}; bootm ${loadaddrbin}\0" \
	"bootcmd_oper=ext4load mmc ${mmcdev}:${mmcpart} ${loadaddrmd5} ${operativedir}${operativeimgmd5}; " \
			"if test ${filesize} = 10; then " \
				"ext4load mmc ${mmcdev}:${mmcpart} ${loadaddrbin} ${operativedir}${operativeimg}; " \
				"if test md5sum -v ${loadaddrbin} ${filesize} *${loadaddrmd5}; then " \
					"echo md5sum check OK; " \
					"bootm ${loadaddrbin}; " \
				"else " \
					"echo md5sum check FAILED: ; " \
					"md5sum ${loadaddrbin} ${filesize}; " \
				"fi; " \
			"else " \
				"echo md5 file error length; " \
				"echo ${filesize}; " \
			"fi; \0" \
	
	
	

#define CONFIG_BOOTCOMMAND \
  "mmc dev ${mmcdev};" \
  "run mmcboot; "

/*BOOT.3*/
#define INPUT_STAT_MAINT_SEL_CPU IMX_GPIO_NR(1, 0)	//GPIO_0__GPIO1_IO00 T5 MAINT_SEL_CPU
/*BOOT.9*/
#ifdef CONFIG_BOOTDELAY
#undef CONFIG_BOOTDELAY
#endif
#define CONFIG_BOOTDELAY 1

#define CONFIG_MD5
#define CONFIG_MD5SUM_VERIFY
#define CONFIG_CMD_MD5SUM

#define CONFIG_PHY_MARVELL
#define CONFIG_ETHADDR 00:11:22:00:11:22

#endif                         /* __MX6QMARTA_CONFIG_H */
