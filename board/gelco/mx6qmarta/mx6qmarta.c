/*
 * Copyright (C) 2012 Freescale Semiconductor, Inc.
 *
 * Author: Fabio Estevam <fabio.estevam@freescale.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <asm/io.h>
#include <asm/arch/clock.h>
#include <asm/arch/imx-regs.h>
#include <asm/arch/iomux.h>
#include <asm/arch/mx6-pins.h>
#include <linux/errno.h>
#include <asm/gpio.h>
#include <asm/imx-common/iomux-v3.h>
#include <asm/imx-common/mxc_i2c.h>
#include <asm/imx-common/boot_mode.h>
#include <asm/imx-common/spi.h>
#include <mmc.h>
#include <fsl_esdhc.h>
#include <miiphy.h>
#include <netdev.h>
#include <asm/arch/sys_proto.h>
#include <i2c.h>
#include <asm/arch/mxc_hdmi.h>
#include <asm/imx-common/video.h>
#include <asm/arch/crm_regs.h>
#include <pca953x.h>
#include <power/pmic.h>
#include <power/pfuze100_pmic.h>
#include "../common/pfuze.h"
#include <exports.h>
#include <version.h>

DECLARE_GLOBAL_DATA_PTR;

#define UART_PAD_CTRL  (PAD_CTL_PUS_100K_UP |			\
	PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm |			\
	PAD_CTL_SRE_FAST  | PAD_CTL_HYS)

#define GPIO_PAD_CTRL (PAD_CTL_PKE | PAD_CTL_PUE |          \
	PAD_CTL_PUS_100K_UP | PAD_CTL_SPEED_MED |               \
	PAD_CTL_DSE_40ohm   | PAD_CTL_SRE_FAST)
	
#define USDHC_PAD_CTRL (PAD_CTL_PUS_22K_UP |			\
	PAD_CTL_SPEED_LOW | PAD_CTL_DSE_120ohm |			\
	PAD_CTL_SRE_FAST  | PAD_CTL_HYS)

#define ENET_PAD_CTRL  (PAD_CTL_PUS_100K_UP |			\
	PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm | PAD_CTL_HYS)
/*
#define I2C_PAD_CTRL	(PAD_CTL_PUS_100K_UP |			\
	PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm | PAD_CTL_HYS |	\
	PAD_CTL_ODE | PAD_CTL_SRE_FAST)
*/
#define GPMI_PAD_CTRL0 (PAD_CTL_PKE | PAD_CTL_PUE | PAD_CTL_PUS_100K_UP)
#define GPMI_PAD_CTRL1 (PAD_CTL_DSE_40ohm | PAD_CTL_SPEED_MED | \
			PAD_CTL_SRE_FAST)
#define GPMI_PAD_CTRL2 (GPMI_PAD_CTRL0 | GPMI_PAD_CTRL1)

/*#define PC MUX_PAD_CTRL(I2C_PAD_CTRL)*/
/*
#define WEIM_NOR_PAD_CTRL (PAD_CTL_PKE | PAD_CTL_PUE |          \
	PAD_CTL_PUS_100K_UP | PAD_CTL_SPEED_MED |               \
	PAD_CTL_DSE_40ohm   | PAD_CTL_SRE_FAST)
*/
//#define I2C_PMIC	1

int dram_init(void)
{
	gd->ram_size = imx_ddr_size();
	
	return 0;
}

static iomux_v3_cfg_t const uart3_pads[] = {
	MX6_PAD_EIM_D24__UART3_TX_DATA | MUX_PAD_CTRL(UART_PAD_CTRL),
	MX6_PAD_EIM_D25__UART3_RX_DATA | MUX_PAD_CTRL(UART_PAD_CTRL)
};

static iomux_v3_cfg_t const gpio_pads[] = {
	MX6_PAD_NANDF_D1__GPIO2_IO01 | MUX_PAD_CTRL(GPIO_PAD_CTRL),
	MX6_PAD_EIM_CS1__GPIO2_IO24 | MUX_PAD_CTRL(GPIO_PAD_CTRL),
	MX6_PAD_EIM_WAIT__GPIO5_IO00 | MUX_PAD_CTRL(GPIO_PAD_CTRL),
	MX6_PAD_DISP0_DAT17__GPIO5_IO11 | MUX_PAD_CTRL(GPIO_PAD_CTRL),
	MX6_PAD_DISP0_DAT14__GPIO5_IO08 | MUX_PAD_CTRL(GPIO_PAD_CTRL),
	MX6_PAD_NANDF_D6__GPIO2_IO06 | MUX_PAD_CTRL(GPIO_PAD_CTRL),
	MX6_PAD_EIM_A22__GPIO2_IO16 | MUX_PAD_CTRL(GPIO_PAD_CTRL),
	MX6_PAD_EIM_A21__GPIO2_IO17 | MUX_PAD_CTRL(GPIO_PAD_CTRL),
	MX6_PAD_EIM_A20__GPIO2_IO18 | MUX_PAD_CTRL(GPIO_PAD_CTRL),
	MX6_PAD_EIM_A19__GPIO2_IO19 | MUX_PAD_CTRL(GPIO_PAD_CTRL),
	MX6_PAD_EIM_A18__GPIO2_IO20 | MUX_PAD_CTRL(GPIO_PAD_CTRL),
	MX6_PAD_EIM_A17__GPIO2_IO21 | MUX_PAD_CTRL(GPIO_PAD_CTRL),
	MX6_PAD_EIM_A16__GPIO2_IO22 | MUX_PAD_CTRL(GPIO_PAD_CTRL),
	MX6_PAD_EIM_BCLK__GPIO6_IO31 | MUX_PAD_CTRL(GPIO_PAD_CTRL),
	MX6_PAD_NANDF_D7__GPIO2_IO07 | MUX_PAD_CTRL(GPIO_PAD_CTRL),
	MX6_PAD_EIM_A25__GPIO5_IO02 | MUX_PAD_CTRL(GPIO_PAD_CTRL),
	MX6_PAD_DISP0_DAT13__GPIO5_IO07 | MUX_PAD_CTRL(GPIO_PAD_CTRL),
	MX6_PAD_GPIO_19__GPIO4_IO05 | MUX_PAD_CTRL(GPIO_PAD_CTRL)
};

static iomux_v3_cfg_t const enet_pads[] = {
	MX6_PAD_ENET_MDC__ENET_MDC		| MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_ENET_MDIO__ENET_MDIO		| MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_ENET_REF_CLK__ENET_TX_CLK	| MUX_PAD_CTRL(ENET_PAD_CTRL),	//???????
	//DD0
	//MX6_PAD_ENET_CRS_DV__GPIO1_IO25		| MUX_PAD_CTRL(NO_PAD_CTRL),	//reset
	//DD1
	MX6_PAD_DISP0_DAT15__GPIO5_IO09		| MUX_PAD_CTRL(NO_PAD_CTRL),	//DISP0_DAT15__GPIO5_IO09  T22  RGMII_RST_PHY
	
	//DD0
	//MX6_PAD_ENET_RXD1__GPIO1_IO26		| MUX_PAD_CTRL(NO_PAD_CTRL),	//interrupt
	//DD1
	MX6_PAD_NANDF_CLE__GPIO6_IO07	| MUX_PAD_CTRL(NO_PAD_CTRL),		//NANDF_CLE_GPIO6_IO07  C15  RGMII_INT_PHY
	
	MX6_PAD_RGMII_TXC__RGMII_TXC	| MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_RGMII_TD0__RGMII_TD0	| MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_RGMII_TD1__RGMII_TD1	| MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_RGMII_TD2__RGMII_TD2	| MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_RGMII_TD3__RGMII_TD3	| MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_RGMII_TX_CTL__RGMII_TX_CTL	| MUX_PAD_CTRL(ENET_PAD_CTRL),
	
	MX6_PAD_RGMII_RXC__RGMII_RXC	| MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_RGMII_RD0__RGMII_RD0	| MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_RGMII_RD1__RGMII_RD1	| MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_RGMII_RD2__RGMII_RD2	| MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_RGMII_RD3__RGMII_RD3	| MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_RGMII_RX_CTL__RGMII_RX_CTL	| MUX_PAD_CTRL(ENET_PAD_CTRL)
};

/* I2C2 PMIC, iPod, Tuner, Codec, Touch, HDMI EDID, MIPI CSI2 card */
//static struct i2c_pads_info i2c_pad_info1 = {
//	.scl = {
//		.i2c_mode = MX6_PAD_EIM_EB2__I2C2_SCL | PC,
//		.gpio_mode = MX6_PAD_EIM_EB2__GPIO2_IO30 | PC,
//		.gp = IMX_GPIO_NR(2, 30)
//	},
//	.sda = {
//		.i2c_mode = MX6_PAD_KEY_ROW3__I2C2_SDA | PC,
//		.gpio_mode = MX6_PAD_KEY_ROW3__GPIO4_IO13 | PC,
//		.gp = IMX_GPIO_NR(4, 13)
//	}
//};

//#ifndef CONFIG_SYS_FLASH_CFI
/*
 * I2C3 MLB, Port Expanders (A, B, C), Video ADC, Light Sensor,
 * Compass Sensor, Accelerometer, Res Touch
 */
//static struct i2c_pads_info i2c_pad_info2 = {
//	.scl = {
//		.i2c_mode = MX6_PAD_GPIO_3__I2C3_SCL | PC,
//		.gpio_mode = MX6_PAD_GPIO_3__GPIO1_IO03 | PC,
//		.gp = IMX_GPIO_NR(1, 3)
//	},
//	.sda = {
//		.i2c_mode = MX6_PAD_EIM_D18__I2C3_SDA | PC,
//		.gpio_mode = MX6_PAD_EIM_D18__GPIO3_IO18 | PC,
//		.gp = IMX_GPIO_NR(3, 18)
//	}
//};
//#endif

//static iomux_v3_cfg_t const i2c3_pads[] = {
//	MX6_PAD_EIM_A24__GPIO5_IO04		| MUX_PAD_CTRL(NO_PAD_CTRL),
//};

//static iomux_v3_cfg_t const port_exp[] = {
//	MX6_PAD_SD2_DAT0__GPIO1_IO15		| MUX_PAD_CTRL(NO_PAD_CTRL),
//};

/*Define for building port exp gpio, pin starts from 0*/
/*#define PORTEXP_IO_NR(chip, pin) \*/
/*	((chip << 5) + pin)*/

/*Get the chip addr from a ioexp gpio*/
/*#define PORTEXP_IO_TO_CHIP(gpio_nr) \*/
/*	(gpio_nr >> 5)*/

/*Get the pin number from a ioexp gpio*/
/*#define PORTEXP_IO_TO_PIN(gpio_nr) \*/
/*	(gpio_nr & 0x1f)*/

//static int port_exp_direction_output(unsigned gpio, int value)
//{
//	int ret;

//	i2c_set_bus_num(2);
//	ret = i2c_probe(PORTEXP_IO_TO_CHIP(gpio));
//	if (ret)
//		return ret;

//	ret = pca953x_set_dir(PORTEXP_IO_TO_CHIP(gpio),
//		(1 << PORTEXP_IO_TO_PIN(gpio)),
//		(PCA953X_DIR_OUT << PORTEXP_IO_TO_PIN(gpio)));

//	if (ret)
//		return ret;

//	ret = pca953x_set_val(PORTEXP_IO_TO_CHIP(gpio),
//		(1 << PORTEXP_IO_TO_PIN(gpio)),
//		(value << PORTEXP_IO_TO_PIN(gpio)));

//	if (ret)
//		return ret;

//	return 0;
//}
/*
static iomux_v3_cfg_t const eimnor_pads[] = {
	MX6_PAD_EIM_D16__EIM_DATA16	| MUX_PAD_CTRL(WEIM_NOR_PAD_CTRL),
	MX6_PAD_EIM_D17__EIM_DATA17	| MUX_PAD_CTRL(WEIM_NOR_PAD_CTRL),
	MX6_PAD_EIM_D18__EIM_DATA18	| MUX_PAD_CTRL(WEIM_NOR_PAD_CTRL),
	MX6_PAD_EIM_D19__EIM_DATA19	| MUX_PAD_CTRL(WEIM_NOR_PAD_CTRL),
	MX6_PAD_EIM_D20__EIM_DATA20	| MUX_PAD_CTRL(WEIM_NOR_PAD_CTRL),
	MX6_PAD_EIM_D21__EIM_DATA21	| MUX_PAD_CTRL(WEIM_NOR_PAD_CTRL),
	MX6_PAD_EIM_D22__EIM_DATA22	| MUX_PAD_CTRL(WEIM_NOR_PAD_CTRL),
	MX6_PAD_EIM_D23__EIM_DATA23	| MUX_PAD_CTRL(WEIM_NOR_PAD_CTRL),
	MX6_PAD_EIM_D24__EIM_DATA24	| MUX_PAD_CTRL(WEIM_NOR_PAD_CTRL),
	MX6_PAD_EIM_D25__EIM_DATA25	| MUX_PAD_CTRL(WEIM_NOR_PAD_CTRL),
	MX6_PAD_EIM_D26__EIM_DATA26	| MUX_PAD_CTRL(WEIM_NOR_PAD_CTRL),
	MX6_PAD_EIM_D27__EIM_DATA27	| MUX_PAD_CTRL(WEIM_NOR_PAD_CTRL),
	MX6_PAD_EIM_D28__EIM_DATA28	| MUX_PAD_CTRL(WEIM_NOR_PAD_CTRL),
	MX6_PAD_EIM_D29__EIM_DATA29	| MUX_PAD_CTRL(WEIM_NOR_PAD_CTRL),
	MX6_PAD_EIM_D30__EIM_DATA30	| MUX_PAD_CTRL(WEIM_NOR_PAD_CTRL),
	MX6_PAD_EIM_D31__EIM_DATA31	| MUX_PAD_CTRL(WEIM_NOR_PAD_CTRL),
	MX6_PAD_EIM_DA0__EIM_AD00	| MUX_PAD_CTRL(WEIM_NOR_PAD_CTRL),
	MX6_PAD_EIM_DA1__EIM_AD01	| MUX_PAD_CTRL(WEIM_NOR_PAD_CTRL),
	MX6_PAD_EIM_DA2__EIM_AD02	| MUX_PAD_CTRL(WEIM_NOR_PAD_CTRL),
	MX6_PAD_EIM_DA3__EIM_AD03	| MUX_PAD_CTRL(WEIM_NOR_PAD_CTRL),
	MX6_PAD_EIM_DA4__EIM_AD04	| MUX_PAD_CTRL(WEIM_NOR_PAD_CTRL),
	MX6_PAD_EIM_DA5__EIM_AD05	| MUX_PAD_CTRL(WEIM_NOR_PAD_CTRL),
	MX6_PAD_EIM_DA6__EIM_AD06	| MUX_PAD_CTRL(WEIM_NOR_PAD_CTRL),
	MX6_PAD_EIM_DA7__EIM_AD07	| MUX_PAD_CTRL(WEIM_NOR_PAD_CTRL),
	MX6_PAD_EIM_DA8__EIM_AD08	| MUX_PAD_CTRL(WEIM_NOR_PAD_CTRL),
	MX6_PAD_EIM_DA9__EIM_AD09	| MUX_PAD_CTRL(WEIM_NOR_PAD_CTRL),
	MX6_PAD_EIM_DA10__EIM_AD10	| MUX_PAD_CTRL(WEIM_NOR_PAD_CTRL),
	MX6_PAD_EIM_DA11__EIM_AD11	| MUX_PAD_CTRL(WEIM_NOR_PAD_CTRL) ,
	MX6_PAD_EIM_DA12__EIM_AD12	| MUX_PAD_CTRL(WEIM_NOR_PAD_CTRL),
	MX6_PAD_EIM_DA13__EIM_AD13	| MUX_PAD_CTRL(WEIM_NOR_PAD_CTRL),
	MX6_PAD_EIM_DA14__EIM_AD14	| MUX_PAD_CTRL(WEIM_NOR_PAD_CTRL),
	MX6_PAD_EIM_DA15__EIM_AD15	| MUX_PAD_CTRL(WEIM_NOR_PAD_CTRL),
	MX6_PAD_EIM_A16__EIM_ADDR16	| MUX_PAD_CTRL(WEIM_NOR_PAD_CTRL),
	MX6_PAD_EIM_A17__EIM_ADDR17	| MUX_PAD_CTRL(WEIM_NOR_PAD_CTRL),
	MX6_PAD_EIM_A18__EIM_ADDR18	| MUX_PAD_CTRL(WEIM_NOR_PAD_CTRL),
	MX6_PAD_EIM_A19__EIM_ADDR19	| MUX_PAD_CTRL(WEIM_NOR_PAD_CTRL),
	MX6_PAD_EIM_A20__EIM_ADDR20	| MUX_PAD_CTRL(WEIM_NOR_PAD_CTRL),
	MX6_PAD_EIM_A21__EIM_ADDR21	| MUX_PAD_CTRL(WEIM_NOR_PAD_CTRL),
	MX6_PAD_EIM_A22__EIM_ADDR22	| MUX_PAD_CTRL(WEIM_NOR_PAD_CTRL),
	MX6_PAD_EIM_A23__EIM_ADDR23	| MUX_PAD_CTRL(WEIM_NOR_PAD_CTRL),
	MX6_PAD_EIM_OE__EIM_OE_B	| MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_EIM_RW__EIM_RW		| MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_EIM_CS0__EIM_CS0_B	| MUX_PAD_CTRL(NO_PAD_CTRL),
};
*/
//static void eimnor_cs_setup(void)
//{
//	struct weim *weim_regs = (struct weim *)WEIM_BASE_ADDR;

//	writel(0x00020181, &weim_regs->cs0gcr1);
//	writel(0x00000001, &weim_regs->cs0gcr2);
//	writel(0x0a020000, &weim_regs->cs0rcr1);
//	writel(0x0000c000, &weim_regs->cs0rcr2);
//	writel(0x0804a240, &weim_regs->cs0wcr1);
//	writel(0x00000120, &weim_regs->wcr);

//	set_chipselect_size(CS0_128);
//}

//static void setup_iomux_eimnor(void)
//{
//	imx_iomux_v3_setup_multiple_pads(eimnor_pads, ARRAY_SIZE(eimnor_pads));

//	gpio_direction_output(IMX_GPIO_NR(5, 4), 0);

//	eimnor_cs_setup();
//}

static void setup_iomux_enet(void)
{
	imx_iomux_v3_setup_multiple_pads(enet_pads, ARRAY_SIZE(enet_pads));
	
	//DD0
	//gpio_direction_output(IMX_GPIO_NR(1, 25) , 0);
	//DD1
	gpio_direction_output(IMX_GPIO_NR(5, 9) , 0);	//DISP0_DAT15__GPIO5_IO09  T22  RGMII_RST_PHY
	
	mdelay(10);
	
	//DD0
	//gpio_set_value(IMX_GPIO_NR(1, 25), 1);
	//DD1
	gpio_set_value(IMX_GPIO_NR(5, 9), 1);	//DISP0_DAT15__GPIO5_IO09  T22  RGMII_RST_PHY
	
	udelay(100);
}

static iomux_v3_cfg_t const usdhc3_pads[] = {
	MX6_PAD_SD3_CLK__SD3_CLK	| MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD3_CMD__SD3_CMD	| MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD3_DAT0__SD3_DATA0	| MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD3_DAT1__SD3_DATA1	| MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD3_DAT2__SD3_DATA2	| MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD3_DAT3__SD3_DATA3	| MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD3_DAT4__SD3_DATA4	| MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD3_DAT5__SD3_DATA5	| MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD3_DAT6__SD3_DATA6	| MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD3_DAT7__SD3_DATA7	| MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD3_RST__SD3_RESET   | MUX_PAD_CTRL(NO_PAD_CTRL),
};

iomux_v3_cfg_t const usdhc4_pads[] = {
	MX6_PAD_SD4_CLK__SD4_CLK   | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD4_CMD__SD4_CMD   | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD4_DAT0__SD4_DATA0 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD4_DAT1__SD4_DATA1 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD4_DAT2__SD4_DATA2 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD4_DAT3__SD4_DATA3 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD4_DAT4__SD4_DATA4 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD4_DAT5__SD4_DATA5 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD4_DAT6__SD4_DATA6 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD4_DAT7__SD4_DATA7 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_NANDF_ALE__SD4_RESET | MUX_PAD_CTRL(NO_PAD_CTRL),
};

static void setup_iomux_uart(void)
{
	imx_iomux_v3_setup_multiple_pads(uart3_pads, ARRAY_SIZE(uart3_pads));
}

static void setup_iomux_gpio(void)
{
	imx_iomux_v3_setup_multiple_pads(gpio_pads, ARRAY_SIZE(gpio_pads));
}

#ifdef CONFIG_FSL_ESDHC
static struct fsl_esdhc_cfg usdhc_cfg[2] = {
	{USDHC3_BASE_ADDR},
	{USDHC4_BASE_ADDR},
};

int board_mmc_getcd(struct mmc *mmc)
{
	struct fsl_esdhc_cfg *cfg = (struct fsl_esdhc_cfg *)mmc->priv;
	int ret = 0;

	switch (cfg->esdhc_base) {
	//case USDHC2_BASE_ADDR:
	//	ret = 0; /* USD2 is never present*/
	//	break;
	case USDHC3_BASE_ADDR:
		ret = 1;/* eMMC/uSDHC3 is always present */
		break;
	case USDHC4_BASE_ADDR:
		ret = 1; /* eMMC/uSDHC4 is always present */
		break;
	}
	return ret;
}

int board_mmc_init(bd_t *bis)
{
	s32 status = 0;
//	int i;
	
	imx_iomux_v3_setup_multiple_pads(usdhc3_pads, ARRAY_SIZE(usdhc3_pads));
	imx_iomux_v3_setup_multiple_pads(usdhc4_pads, ARRAY_SIZE(usdhc4_pads));
	
	usdhc_cfg[0].sdhc_clk = mxc_get_clock(MXC_ESDHC_CLK);
	usdhc_cfg[1].sdhc_clk = mxc_get_clock(MXC_ESDHC_CLK);
	
	status = fsl_esdhc_initialize(bis, &usdhc_cfg[0]);
	//printf("esdhc0_init: %d\n", status);
	status |= fsl_esdhc_initialize(bis, &usdhc_cfg[1]);
	//printf("esdhc1_init: %d\n", status);

/*	
	for (i = 0; i < CONFIG_SYS_FSL_USDHC_NUM; i++) {
		switch (i) {
		case 0:
			imx_iomux_v3_setup_multiple_pads(usdhc3_pads, ARRAY_SIZE(usdhc3_pads));
			usdhc_cfg[0].sdhc_clk = mxc_get_clock(MXC_ESDHC3_CLK);
			usdhc_cfg[0].esdhc_base = USDHC3_BASE_ADDR;
			break;
		case 1:
			imx_iomux_v3_setup_multiple_pads(usdhc4_pads, ARRAY_SIZE(usdhc4_pads));
			usdhc_cfg[1].sdhc_clk = mxc_get_clock(MXC_ESDHC4_CLK);
			usdhc_cfg[1].esdhc_base = USDHC4_BASE_ADDR;
			break;
		default:
			printf("Warning: you configured more USDHC controllers"
			       "(%d) then supported by the board (%d)\n",
			       i + 1, CONFIG_SYS_FSL_USDHC_NUM);
			return status;
		}

		status |= fsl_esdhc_initialize(bis, &usdhc_cfg[i]);
	}
*/


	return status;
}
#endif

//#ifdef CONFIG_NAND_MXS
//static iomux_v3_cfg_t gpmi_pads[] = {
//	MX6_PAD_NANDF_CLE__NAND_CLE		| MUX_PAD_CTRL(GPMI_PAD_CTRL2),
//	MX6_PAD_NANDF_ALE__NAND_ALE		| MUX_PAD_CTRL(GPMI_PAD_CTRL2),
//	MX6_PAD_NANDF_WP_B__NAND_WP_B	| MUX_PAD_CTRL(GPMI_PAD_CTRL2),
//	MX6_PAD_NANDF_RB0__NAND_READY_B	| MUX_PAD_CTRL(GPMI_PAD_CTRL0),
//	MX6_PAD_NANDF_CS0__NAND_CE0_B	| MUX_PAD_CTRL(GPMI_PAD_CTRL2),
//	MX6_PAD_SD4_CMD__NAND_RE_B		| MUX_PAD_CTRL(GPMI_PAD_CTRL2),
//	MX6_PAD_SD4_CLK__NAND_WE_B		| MUX_PAD_CTRL(GPMI_PAD_CTRL2),
//	MX6_PAD_NANDF_D0__NAND_DATA00	| MUX_PAD_CTRL(GPMI_PAD_CTRL2),
//	MX6_PAD_NANDF_D1__NAND_DATA01	| MUX_PAD_CTRL(GPMI_PAD_CTRL2),
//	MX6_PAD_NANDF_D2__NAND_DATA02	| MUX_PAD_CTRL(GPMI_PAD_CTRL2),
//	MX6_PAD_NANDF_D3__NAND_DATA03	| MUX_PAD_CTRL(GPMI_PAD_CTRL2),
//	MX6_PAD_NANDF_D4__NAND_DATA04	| MUX_PAD_CTRL(GPMI_PAD_CTRL2),
//	MX6_PAD_NANDF_D5__NAND_DATA05	| MUX_PAD_CTRL(GPMI_PAD_CTRL2),
//	MX6_PAD_NANDF_D6__NAND_DATA06	| MUX_PAD_CTRL(GPMI_PAD_CTRL2),
//	MX6_PAD_NANDF_D7__NAND_DATA07	| MUX_PAD_CTRL(GPMI_PAD_CTRL2),
//	MX6_PAD_SD4_DAT0__NAND_DQS		| MUX_PAD_CTRL(GPMI_PAD_CTRL1),
//};

//static void setup_gpmi_nand(void)
//{
//	struct mxc_ccm_reg *mxc_ccm = (struct mxc_ccm_reg *)CCM_BASE_ADDR;

	/* config gpmi nand iomux */
//	imx_iomux_v3_setup_multiple_pads(gpmi_pads, ARRAY_SIZE(gpmi_pads));

//	setup_gpmi_io_clk((MXC_CCM_CS2CDR_ENFC_CLK_PODF(0) |
//			MXC_CCM_CS2CDR_ENFC_CLK_PRED(3) |
//			MXC_CCM_CS2CDR_ENFC_CLK_SEL(3)));

	/* enable apbh clock gating */
//	setbits_le32(&mxc_ccm->CCGR0, MXC_CCM_CCGR0_APBHDMA_MASK);
//}
//#endif

static void setup_fec(void)
{
	if (is_mx6dqp()) {
		/*
		 * select ENET MAC0 TX clock from PLL
		 */
		imx_iomux_set_gpr_register(5, 9, 1, 1);
		enable_fec_anatop_clock(1, ENET_125MHZ);
	}

	setup_iomux_enet();
}

int board_eth_init_old(bd_t *bis)
{
	setup_fec();

	return cpu_eth_init(bis);
}

int board_phy_config(struct phy_device *phydev)
{
	if (phydev->drv->config)
		phydev->drv->config(phydev);

	return 0;
}

int board_eth_init(bd_t *bis)
{
	uint32_t base = IMX_FEC_BASE;
	struct mii_dev *bus = NULL;
	struct phy_device *phydev = NULL;
	int ret;

	setup_iomux_enet();

#ifdef CONFIG_FEC_MXC
	bus = fec_get_miibus(base, -1);
	if (!bus)
		return 0;

	/* scan phy */
	phydev = phy_find_by_mask(bus, (0xFF), PHY_INTERFACE_MODE_RGMII);
	if (!phydev) {
		free(bus);
		return 0;
	}

	//printf("%s\n", phydev->drv->name);
	//printf("using phy at: %d\n", phydev->addr);
	
	ret  = fec_probe(bis, -1, base, bus, phydev);
	if (ret) {
		printf("FEC MXC: %s:failed\n", __func__);
		free(phydev);
		free(bus);
	}

	//printf("phy dev id: %X\n", phydev->phy_id);
	//printf("phy drv name: %s\n", phydev->drv->name);
	//printf("phy dev speed: %d\n", phydev->speed);


#endif

return 0;
}








#define BOARD_REV_B  0x200
#define BOARD_REV_A  0x100

static int mx6sabre_rev(void)
{
	/*
	 * Get Board ID information from OCOTP_GP1[15:8]
	 * i.MX6Q ARD RevA: 0x01
	 * i.MX6Q ARD RevB: 0x02
	 */
	struct ocotp_regs *ocotp = (struct ocotp_regs *)OCOTP_BASE_ADDR;
	struct fuse_bank *bank = &ocotp->bank[4];
	struct fuse_bank4_regs *fuse =
			(struct fuse_bank4_regs *)bank->fuse_regs;
	int reg = readl(&fuse->gp1);
	int ret;

	switch (reg >> 8 & 0x0F) {
	case 0x02:
		ret = BOARD_REV_B;
		break;
	case 0x01:
	default:
		ret = BOARD_REV_A;
		break;
	}

	return ret;
}

u32 get_board_rev(void)
{
	int rev = mx6sabre_rev();

	return (get_cpu_rev() & ~(0xF << 8)) | rev;
}

#if defined(CONFIG_VIDEO_IPUV3)
//static void disable_lvds(struct display_info_t const *dev)
//{
//	struct iomuxc *iomux = (struct iomuxc *)IOMUXC_BASE_ADDR;

//	clrbits_le32(&iomux->gpr[2],
//		     IOMUXC_GPR2_LVDS_CH0_MODE_MASK |
//		     IOMUXC_GPR2_LVDS_CH1_MODE_MASK);
//}

//static void do_enable_hdmi(struct display_info_t const *dev)
//{
//	disable_lvds(dev);
//	imx_enable_hdmi_phy();
//}

struct display_info_t const displays[] = {{
	.bus	= -1,
	.addr	= 0,
	.pixfmt	= IPU_PIX_FMT_RGB666,
	.detect	= NULL,
	.enable	= NULL,
	.mode	= {
		.name           = "Hannstar-XGA",
		.refresh        = 60,
		.xres           = 1024,
		.yres           = 768,
		.pixclock       = 15385,
		.left_margin    = 220,
		.right_margin   = 40,
		.upper_margin   = 21,
		.lower_margin   = 7,
		.hsync_len      = 60,
		.vsync_len      = 10,
		.sync           = FB_SYNC_EXT,
		.vmode          = FB_VMODE_NONINTERLACED
} } };
size_t display_count = ARRAY_SIZE(displays);

//iomux_v3_cfg_t const backlight_pads[] = {
//	MX6_PAD_SD4_DAT1__GPIO2_IO09 | MUX_PAD_CTRL(ENET_PAD_CTRL),
//};

//static void setup_iomux_backlight(void)
//{
//	gpio_direction_output(IMX_GPIO_NR(2, 9), 1);
//	imx_iomux_v3_setup_multiple_pads(backlight_pads,
//					 ARRAY_SIZE(backlight_pads));
//}
/*
static void setup_display(void)
{
	struct mxc_ccm_reg *mxc_ccm = (struct mxc_ccm_reg *)CCM_BASE_ADDR;
	struct iomuxc *iomux = (struct iomuxc *)IOMUXC_BASE_ADDR;
	int reg;

	setup_iomux_backlight();
	enable_ipu_clock();
	imx_setup_hdmi();

	// Turn on LDB_DI0 and LDB_DI1 clocks
	reg = readl(&mxc_ccm->CCGR3);
	reg |= MXC_CCM_CCGR3_LDB_DI0_MASK | MXC_CCM_CCGR3_LDB_DI1_MASK;
	writel(reg, &mxc_ccm->CCGR3);

	// Set LDB_DI0 and LDB_DI1 clk select to 3b'011
	reg = readl(&mxc_ccm->cs2cdr);
	reg &= ~(MXC_CCM_CS2CDR_LDB_DI0_CLK_SEL_MASK |
		 MXC_CCM_CS2CDR_LDB_DI1_CLK_SEL_MASK);
	reg |= (3 << MXC_CCM_CS2CDR_LDB_DI0_CLK_SEL_OFFSET) |
	       (3 << MXC_CCM_CS2CDR_LDB_DI1_CLK_SEL_OFFSET);
	writel(reg, &mxc_ccm->cs2cdr);

	reg = readl(&mxc_ccm->cscmr2);
	reg |= MXC_CCM_CSCMR2_LDB_DI0_IPU_DIV | MXC_CCM_CSCMR2_LDB_DI1_IPU_DIV;
	writel(reg, &mxc_ccm->cscmr2);

	reg = readl(&mxc_ccm->chsccdr);
	reg |= (CHSCCDR_CLK_SEL_LDB_DI0
		<< MXC_CCM_CHSCCDR_IPU1_DI0_CLK_SEL_OFFSET);
	reg |= (CHSCCDR_CLK_SEL_LDB_DI0 <<
		MXC_CCM_CHSCCDR_IPU1_DI1_CLK_SEL_OFFSET);
	writel(reg, &mxc_ccm->chsccdr);

	reg = IOMUXC_GPR2_DI1_VS_POLARITY_ACTIVE_LOW |
	      IOMUXC_GPR2_DI0_VS_POLARITY_ACTIVE_LOW |
	      IOMUXC_GPR2_BIT_MAPPING_CH1_SPWG |
	      IOMUXC_GPR2_DATA_WIDTH_CH1_18BIT |
	      IOMUXC_GPR2_BIT_MAPPING_CH0_SPWG |
	      IOMUXC_GPR2_DATA_WIDTH_CH0_18BIT |
	      IOMUXC_GPR2_LVDS_CH0_MODE_ENABLED_DI0 |
	      IOMUXC_GPR2_LVDS_CH1_MODE_DISABLED;
	writel(reg, &iomux->gpr[2]);

	reg = readl(&iomux->gpr[3]);
	reg &= ~(IOMUXC_GPR3_LVDS0_MUX_CTL_MASK |
		 IOMUXC_GPR3_HDMI_MUX_CTL_MASK);
	reg |= (IOMUXC_GPR3_MUX_SRC_IPU1_DI0 <<
		IOMUXC_GPR3_LVDS0_MUX_CTL_OFFSET) |
	       (IOMUXC_GPR3_MUX_SRC_IPU1_DI0 <<
		IOMUXC_GPR3_HDMI_MUX_CTL_OFFSET);
	writel(reg, &iomux->gpr[3]);
}
*/
#endif /* CONFIG_VIDEO_IPUV3 */

/*
 * Do not overwrite the console
 * Use always serial for U-Boot console
 */
int overwrite_console(void)
{
	return 1;
}

int board_early_init_f(void)
{
	setup_iomux_uart();
	setup_iomux_gpio();
//#ifdef CONFIG_NAND_MXS
//	setup_gpmi_nand();
//#endif

	return 0;
}

int board_init(void)
{
	/* address of boot parameters */
	gd->bd->bi_boot_params = PHYS_SDRAM + 0x100;

	/* I2C 2 and 3 setup - I2C 3 hw mux with EIM */
//	setup_i2c(1, CONFIG_SYS_I2C_SPEED, 0x7f, &i2c_pad_info1);
	/* I2C 3 Steer */
//	gpio_direction_output(IMX_GPIO_NR(5, 4), 1);
//	imx_iomux_v3_setup_multiple_pads(i2c3_pads, ARRAY_SIZE(i2c3_pads));
//#ifndef CONFIG_SYS_FLASH_CFI
//	setup_i2c(2, CONFIG_SYS_I2C_SPEED, 0x7f, &i2c_pad_info2);
//#endif
//	gpio_direction_output(IMX_GPIO_NR(1, 15), 1);
//	imx_iomux_v3_setup_multiple_pads(port_exp, ARRAY_SIZE(port_exp));

//#ifdef CONFIG_VIDEO_IPUV3
//	setup_display();
//#endif
//	setup_iomux_eimnor();
	
	//Nuovo setup GPIO per boot MBDA
	/*BOOT.3*/
	gpio_direction_output(IMX_GPIO_NR(2, 1), 0);	//NANDF_D1__GPIO2_IO01  C17  MAINT_SK_CPU
	gpio_direction_output(IMX_GPIO_NR(2, 24), 0);	//EIM_CS1__GPIO2_IO24  J23  SKR_PWR_CTR_CPU
	gpio_direction_output(IMX_GPIO_NR(5, 0), 0);	//EIM_WAIT__GPIO5_IO00 M25  BIT_L_CPU
	gpio_direction_output(IMX_GPIO_NR(5, 11), 0);	//DISP0_DAT17__GPIO5_IO11  U24  RESET_L_CPU
	gpio_direction_output(IMX_GPIO_NR(5, 8), 0);	//DISP0_DAT14__GPIO5_IO08  U25  GO_SW_CPU
	gpio_direction_output(IMX_GPIO_NR(2, 6), 0);	//NANDF_D6__GPIO2_IO06  E17  ESA_CPU
	gpio_direction_output(IMX_GPIO_NR(2, 16), 0);	//EIM_A22__GPIO2_IO16  F24  EAB_CPU
	gpio_direction_output(IMX_GPIO_NR(2, 17), 0);	//EIM_A21__GPIO2_IO17  H23  EFB_CPU
	gpio_direction_output(IMX_GPIO_NR(2, 18), 0);	//EIM_A20__GPIO2_IO18  H22  EDB_CPU
	gpio_direction_output(IMX_GPIO_NR(2, 19), 0);	//EIM_A19__GPIO2_IO19  G25  EPA_CPU
	gpio_direction_output(IMX_GPIO_NR(2, 20), 0);	//EIM_A18__GPIO2_IO20  J22  ESS_CPU
	gpio_direction_output(IMX_GPIO_NR(2, 21), 0);	//EIM_A17__GPIO2_IO21  G24  EEO_CPU
	gpio_direction_output(IMX_GPIO_NR(2, 22), 0);	//EIM_A16__GPIO2_IO22  H25  EASAU_CPU
	gpio_direction_output(IMX_GPIO_NR(6, 31), 0);	//EIM_BCLK__GPIO6_IO31 N22  EBT_SK_CPU
	gpio_direction_output(IMX_GPIO_NR(2, 7), 0);	//NANDF_D7__GPIO2_IO07  C18  ESF_CPU
	gpio_direction_output(IMX_GPIO_NR(5, 2), 0);	//EIM_A25__GPIO5_IO02  H19  EBT_CPU
	gpio_direction_output(IMX_GPIO_NR(5, 7), 0);	//DISP0_DAT13__GPIO5_IO07  R20  OK_CPU
	gpio_direction_output(IMX_GPIO_NR(4, 5), 0);	//GPIO_19__GPIO4_IO05  P5  IMX6_SPARE_LED
	
	/*BOOT.4*/
	//gpio_set_value(IMX_GPIO_NR(2, 1), 0);	//NANDF_D1__GPIO2_IO01  C17  MAINT_SK_CPU
	//gpio_set_value(IMX_GPIO_NR(2, 24), 0);	//EIM_CS1__GPIO2_IO24  J23  SKR_PWR_CTR_CPU
	//gpio_set_value(IMX_GPIO_NR(5, 0), 0);	//EIM_WAIT__GPIO5_IO00 M25  BIT_L_CPU
	//gpio_set_value(IMX_GPIO_NR(5, 11), 0);	//DISP0_DAT17__GPIO5_IO11  U24  RESET_L_CPU
	//gpio_set_value(IMX_GPIO_NR(5, 8), 0);	//DISP0_DAT14__GPIO5_IO08  U25  GO_SW_CPU
	//gpio_set_value(IMX_GPIO_NR(2, 6), 0);	//NANDF_D6__GPIO2_IO06  E17  ESA_CPU
	//gpio_set_value(IMX_GPIO_NR(2, 16), 0);	//EIM_A22__GPIO2_IO16  F24  EAB_CPU
	//gpio_set_value(IMX_GPIO_NR(2, 17), 0);	//EIM_A21__GPIO2_IO17  H23  EFB_CPU
	//gpio_set_value(IMX_GPIO_NR(2, 18), 0);	//EIM_A20__GPIO2_IO18  H22  EDB_CPU
	//gpio_set_value(IMX_GPIO_NR(2, 19), 0);	//EIM_A19__GPIO2_IO19  G25  EPA_CPU
	//gpio_set_value(IMX_GPIO_NR(2, 20), 0);	//EIM_A18__GPIO2_IO20  J22  ESS_CPU
	//gpio_set_value(IMX_GPIO_NR(2, 21), 0);	//EIM_A17__GPIO2_IO21  G24  EEO_CPU
	//gpio_set_value(IMX_GPIO_NR(2, 22), 0);	//EIM_A16__GPIO2_IO22  H25  EASAU_CPU
	//gpio_set_value(IMX_GPIO_NR(6, 31), 0);	//EIM_BCLK__GPIO6_IO31 N22  EBT_SK_CPU
	//gpio_set_value(IMX_GPIO_NR(2, 7), 0);	//NANDF_D7__GPIO2_IO07  C18  ESF_CPU
	//gpio_set_value(IMX_GPIO_NR(5, 2), 0);	//EIM_A25__GPIO5_IO02  H19  EBT_CPU
	//gpio_set_value(IMX_GPIO_NR(5, 7), 0);	//DISP0_DAT13__GPIO5_IO07  R20  OK_CPU
	//gpio_set_value(IMX_GPIO_NR(4, 5), 0);	//GPIO_19__GPIO4_IO05  P5  IMX6_SPARE_LED
	
	
	
	/*
	//NUOVA IMPOSTAZIONE GPIO PER DD1
	gpio_direction_output(IMX_GPIO_NR(2, 27), 1);	//EIM_LBA__GPIO2_IO27  K22  SPARE_1_OUT_CPU
	gpio_direction_output(IMX_GPIO_NR(2, 25), 1);	//EIM_OE__GPIO2_IO25   J24  SPARE_3_OUT_CPU
	gpio_direction_output(IMX_GPIO_NR(2, 26), 1);	//EIM_RW__GPIO2_IO26   K20  SPARE_2_OUT_CPU
	
	gpio_direction_output(IMX_GPIO_NR(2, 23), 0);	//EIM_CS0__GPIO2_IO23  H24  RESET_ADC_CPU
	gpio_direction_output(IMX_GPIO_NR(2, 24), 0);	//EIM_CS1__GPIO2_IO24  J23  SKR_PWR_CTR_CPU
	
	gpio_direction_output(IMX_GPIO_NR(2, 22), 0);	//EIM_A16__GPIO2_IO22  H25  EASAU_CPU
	gpio_direction_output(IMX_GPIO_NR(2, 21), 0);	//EIM_A17__GPIO2_IO21  G24  EEO_CPU
	gpio_direction_output(IMX_GPIO_NR(2, 20), 0);	//EIM_A18__GPIO2_IO20  J22  ESS_CPU
	gpio_direction_output(IMX_GPIO_NR(2, 19), 0);	//EIM_A19__GPIO2_IO19  G25  EPA_CPU
	gpio_direction_output(IMX_GPIO_NR(2, 18), 0);	//EIM_A20__GPIO2_IO18  H22  EDB_CPU
	gpio_direction_output(IMX_GPIO_NR(2, 17), 0);	//EIM_A21__GPIO2_IO17  H23  EFB_CPU
	gpio_direction_output(IMX_GPIO_NR(2, 16), 0);	//EIM_A22__GPIO2_IO16  F24  EAB_CPU
	gpio_direction_output(IMX_GPIO_NR(6, 6), 0);	//EIM_A23__GPIO6_IO06  J21  GPIO6_06_UNUSED
	gpio_direction_output(IMX_GPIO_NR(5, 4), 0);	//EIM_A24__GPIO5_IO04  F25  INH_SORVOLO_CPU
	
	gpio_direction_output(IMX_GPIO_NR(5, 2), 0);	//EIM_A25__GPIO5_IO02  H19  EBT_CPU
	
	gpio_direction_output(IMX_GPIO_NR(3, 23), 0);	//EIM_D23__GPIO3_IO23  D25  UART3_CTS_B_CPU
	
	gpio_direction_output(IMX_GPIO_NR(2, 30), 0);	//EIM_EB2__GPIO2_IO30  E22  CMD_SPIRA_CPU
	gpio_direction_output(IMX_GPIO_NR(2, 31), 0);	//EIM_EB3__GPIO2_IO31  F23  INH_G_SWITCH_CPU
	
	gpio_direction_output(IMX_GPIO_NR(6, 31), 0);	//EIM_BCLK__GPIO6_IO31 N22  EBT_SK_CPU
	gpio_direction_output(IMX_GPIO_NR(5, 0), 0);	//EIM_WAIT__GPIO5_IO00 M25  BIT_L_CPU
	
	gpio_direction_output(IMX_GPIO_NR(2, 28), 0);	//EIM_EB0__GPIO2_IO28  K21  GPIO2_IO28_UNUSED
	
	gpio_direction_output(IMX_GPIO_NR(1, 2), 0);	//GPIO_2__GPIO1_IO02     T1  IT_UC
				
	gpio_direction_output(IMX_GPIO_NR(5, 18), 0);	//CSI0_PIXCLK__GPIO5_IO18  P1  ARM_EVENTO
	gpio_direction_output(IMX_GPIO_NR(5, 20), 0);	//CSI0_DATA_EN__GPIO5_IO20 P3  ARM_TRACE_CLK
	gpio_direction_output(IMX_GPIO_NR(5, 21), 0);	//CSI0_VSYNC__GPIO5_IO21   N2  ARM_TRACE00
	gpio_direction_output(IMX_GPIO_NR(5, 22), 0);	//CSI0_DAT4__GPIO5_IO22    N1  ARM_TRACE01
	gpio_direction_output(IMX_GPIO_NR(5, 23), 0);	//CSI0_DAT5__GPIO5_IO23    P2  ARM_TRACE02
	gpio_direction_output(IMX_GPIO_NR(5, 24), 0);	//CSI0_DAT6__GPIO5_IO24    N4  ARM_TRACE03
	gpio_direction_output(IMX_GPIO_NR(5, 25), 0);	//CSI0_DAT7__GPIO5_IO25    N3  ARM_TRACE04
	gpio_direction_output(IMX_GPIO_NR(5, 26), 0);	//CSI0_DAT8__GPIO5_IO26    N6  ARM_TRACE05
	gpio_direction_output(IMX_GPIO_NR(5, 27), 0);	//CSI0_DAT9__GPIO5_IO27    N5  ARM_TRACE06
	gpio_direction_output(IMX_GPIO_NR(5, 28), 0);	//CSI0_DAT10__GPIO5_IO28   M1  ARM_TRACE07
	
	
	gpio_direction_output(IMX_GPIO_NR(4, 29), 0);	//DISP0_DAT8__GPIO4_IO29  R22  CMD_CONSENSOFUOCO_CPU
	gpio_direction_output(IMX_GPIO_NR(4, 30), 0);	//DISP0_DAT9__GPIO4_IO30  T25  CMD_FMP_INT_CPU
	
	gpio_direction_output(IMX_GPIO_NR(5, 5), 0);	//DISP0_DAT11__GPIO5_IO05  T23  SENS_D00_CPU
	gpio_direction_output(IMX_GPIO_NR(5, 6), 0);	//DISP0_DAT12__GPIO5_IO06  T24  SENS_D01_CPU
	gpio_direction_output(IMX_GPIO_NR(5, 7), 0);	//DISP0_DAT13__GPIO5_IO07  R20  OK_CPU
	gpio_direction_output(IMX_GPIO_NR(5, 8), 0);	//DISP0_DAT14__GPIO5_IO08  U25  GO_SW_CPU
	
	gpio_direction_output(IMX_GPIO_NR(5, 10), 0);	//DISP0_DAT16__GPIO5_IO10  T21  SEL_ANT_CPU
	gpio_direction_output(IMX_GPIO_NR(5, 11), 0);	//DISP0_DAT17__GPIO5_IO11  U24  RESET_L_CPU
	
	
	gpio_direction_output(IMX_GPIO_NR(4, 15), 0);	//KEY_ROW4__GPIO4_IO15  V5  USB_OTGPWR_EN
	gpio_direction_output(IMX_GPIO_NR(4, 5), 0);	//GPIO_19__GPIO4_IO05  P5  IMX6_SPARE_LED
	
	gpio_direction_output(IMX_GPIO_NR(2, 0), 0);	//NANDF_D0__GPIO2_IO00  A18  TFUEL_RANGE_SLC
	gpio_direction_output(IMX_GPIO_NR(2, 1), 0);	//NANDF_D1__GPIO2_IO01  C17  MAINT_SK_CPU
	gpio_direction_output(IMX_GPIO_NR(2, 2), 0);	//NANDF_D2__GPIO2_IO02  F16  ID_00_CPU
	gpio_direction_output(IMX_GPIO_NR(2, 3), 0);	//NANDF_D3__GPIO2_IO03  D17  ID_01_CPU
	gpio_direction_output(IMX_GPIO_NR(2, 4), 0);	//NANDF_D4__GPIO2_IO04  A19  SAFE_SPOLETTA_CPU
	gpio_direction_output(IMX_GPIO_NR(2, 5), 0);	//NANDF_D5__GPIO2_IO05  B18  ENABLE_DL_CPU
	gpio_direction_output(IMX_GPIO_NR(2, 6), 0);	//NANDF_D6__GPIO2_IO06  E17  ESA_CPU
	gpio_direction_output(IMX_GPIO_NR(2, 7), 0);	//NANDF_D7__GPIO2_IO07  C18  ESF_CPU
	
	
	gpio_direction_output(IMX_GPIO_NR(4, 9), 1);	//KEY_ROW1__GPIO4_IO09   ECSPI1_UC_SS0
	
	gpio_direction_output(IMX_GPIO_NR(4, 24), 1);	//DISP0_DAT3__GPIO4_IO24  P21  ECSPI3_SS0
	gpio_direction_output(IMX_GPIO_NR(4, 25), 1);	//DISP0_DAT4__GPIO4_IO25  P20  ECSPI3_SS1
	gpio_direction_output(IMX_GPIO_NR(4, 26), 1);	//DISP0_DAT5__GPIO4_IO26  R25  ECSPI3_SS2
	gpio_direction_output(IMX_GPIO_NR(4, 27), 1);	//DISP0_DAT6__GPIO4_IO27  R23  ECSPI3_SS3
	
	gpio_direction_output(IMX_GPIO_NR(3, 20), 1);	//EIM_D20__GPIO3_IO20  G20  ECSPI4_SS0
	
	gpio_direction_output(IMX_GPIO_NR(1, 17), 1);	//SD1_DAT1__GPIO1_IO17  C20  ECSPI5_SS0
	gpio_direction_output(IMX_GPIO_NR(1, 19), 1);	//SD1_DAT2__GPIO1_IO19  E19  ECSPI5_SS1
	gpio_direction_output(IMX_GPIO_NR(1, 21), 1);	//SD1_DAT3__GPIO1_IO21  F18  ECSPI5_SS2
	gpio_direction_output(IMX_GPIO_NR(1, 12), 1);	//SD2_DAT3__GPIO1_IO12  B22  ECSPI5_SS3
	*/
	
	return 0;
}

//#ifdef CONFIG_MXC_SPI
//int board_spi_cs_gpio(unsigned bus, unsigned cs)
//{
//	return (bus == 0 && cs == 0) ? (IMX_GPIO_NR(4, 9)) : -1;
//}
//#endif

int power_init_board(void)
{
//	struct pmic *p;
//	unsigned int value;

//	p = pfuze_common_init(I2C_PMIC);
//	if (!p)
//		return -ENODEV;

//	if (is_mx6dqp()) {
		/* set SW2 staby volatage 0.975V*/
//		pmic_reg_read(p, PFUZE100_SW2STBY, &value);
//		value &= ~0x3f;
//		value |= 0x17;
//		pmic_reg_write(p, PFUZE100_SW2STBY, value);
//	}

//	return pfuze_mode_init(p, APS_PFM);
	return 0;
}

#ifdef CONFIG_CMD_BMODE

static const struct boot_mode board_boot_modes[] = {
	/* 4 bit bus width */
	{"mmc0", MAKE_CFGVAL(0x60, 0x50, 0x00, 0x00)},
	{"mmc1", MAKE_CFGVAL(0x60, 0x58, 0x00, 0x00)},
	{NULL,   0},
};
#endif

int board_late_init(void)
{
#ifdef CONFIG_CMD_BMODE
	add_board_boot_modes(board_boot_modes);
#endif

#ifdef CONFIG_ENV_VARS_UBOOT_RUNTIME_CONFIG
	
	setenv("boot_version", "P/N:16100043920.01 - (V05 Feb-13-2018)");
	setenv("boot_build", U_BOOT_VERSION_STRING);
	
	//if (is_mx6dqp())
	//	setenv("board_rev", "MX6QP");
	//else if (is_mx6dq())
	//	setenv("board_rev", "MX6Q");
	//else if (is_mx6sdl())
	//	setenv("board_rev", "MX6DL");
#endif

	return 0;
}

int checkboard(void)
{
	//int rev = mx6sabre_rev();
	//char *revname;

	//switch (rev) {
	//case BOARD_REV_B:
	//	revname = "B";
	//	break;
	//case BOARD_REV_A:
	//	revname = "B";
	//	break;
	//default:
	//	revname = "X";
	//	break;
	//}

	//printf("Board: MX6Q-Marte SD4 DD1 rev%s\n", revname);

	return 0;
}

#ifdef CONFIG_USB_EHCI_MX6
#define USB_HOST1_PWR     PORTEXP_IO_NR(0x32, 7)
#define USB_OTG_PWR       PORTEXP_IO_NR(0x34, 1)

iomux_v3_cfg_t const usb_otg_pads[] = {
	MX6_PAD_ENET_RX_ER__USB_OTG_ID | MUX_PAD_CTRL(NO_PAD_CTRL),
};

int board_ehci_hcd_init(int port)
{
	switch (port) {
	case 0:
		imx_iomux_v3_setup_multiple_pads(usb_otg_pads,
			ARRAY_SIZE(usb_otg_pads));

		/*
		  * Set daisy chain for otg_pin_id on 6q.
		 *  For 6dl, this bit is reserved.
		 */
		imx_iomux_set_gpr_register(1, 13, 1, 0);
		break;
	case 1:
		break;
	default:
		printf("MXC USB port %d not yet supported\n", port);
		return -EINVAL;
	}
	return 0;
}

int board_ehci_power(int port, int on)
{
	switch (port) {
	case 0:
//		if (on)
//			port_exp_direction_output(USB_OTG_PWR, 1);
//		else
//			port_exp_direction_output(USB_OTG_PWR, 0);
		break;
	case 1:
//		if (on)
//			port_exp_direction_output(USB_HOST1_PWR, 1);
//		else
//			port_exp_direction_output(USB_HOST1_PWR, 0);
		break;
	default:
		printf("MXC USB port %d not yet supported\n", port);
		return -EINVAL;
	}

	return 0;
}
#endif
