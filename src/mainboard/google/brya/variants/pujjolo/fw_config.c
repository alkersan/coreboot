/* SPDX-License-Identifier: GPL-2.0-or-later */

#include <baseboard/gpio.h>
#include <baseboard/variants.h>
#include <console/console.h>
#include <fw_config.h>

static const struct pad_config ish_disable_pads[] = {
	/* B3  : ISH_IMU_INT_L */
	PAD_NC_LOCK(GPP_B3, NONE, LOCK_CONFIG),
	/* B4  : ISH_ACC_INT_L */
	PAD_NC_LOCK(GPP_B4, NONE, LOCK_CONFIG),
	/* B5  : EC_I2C_SENSOR_SDA_SOC */
	PAD_NC_LOCK(GPP_B5, NONE, LOCK_CONFIG),
	/* B6  : EC_I2C_SENSOR_SCL_SOC */
	PAD_NC_LOCK(GPP_B6, NONE, LOCK_CONFIG),
};

static const struct pad_config wwan_disable_pads[] = {
	/* A8  : WWAN_RF_DISABLE_ODL */
	PAD_NC(GPP_A8, NONE),
	/* B8  : WWAN_SAR_DETECT_2_ODL */
	PAD_NC(GPP_B8, NONE),
	/* D5  : SRCCLKREQ0# ==> WWAN_CLKREQ_ODL */
	PAD_NC(GPP_D5, NONE),
	/* D6  : WWAN_EN */
	PAD_NC(GPP_D6, NONE),
	/* E16 : WWAN_PCIE_WAKE_ODL */
	PAD_NC(GPP_E16, NONE),
	/* E17 : WWAN_RST_L */
	PAD_NC_LOCK(GPP_E17, NONE, LOCK_CONFIG),
	/* H19 : SOC_I2C_SUB_INT_ODL */
	PAD_NC(GPP_H19, NONE),
	/* H21  : WCAM_MCLK_R ==> WWAN_PERST_L */
	PAD_NC_LOCK(GPP_H21, NONE, LOCK_CONFIG),
	/* H23 : WWAN_SAR_DETECT_ODL */
	PAD_NC(GPP_H23, NONE),
};

void fw_config_gpio_padbased_override(struct pad_config *padbased_table)
{
	if (fw_config_probe(FW_CONFIG(TABLET_MODE, TABLET_MODE_DISABLE))) {
		printk(BIOS_INFO, "Disable ISH-related GPIO pins.\n");
		gpio_padbased_override(padbased_table, ish_disable_pads,
						ARRAY_SIZE(ish_disable_pads));
	}

	if (fw_config_probe(FW_CONFIG(WWAN, WWAN_ABSENT))) {
		printk(BIOS_INFO, "Disable WWAN-related GPIO pins.\n");
		gpio_padbased_override(padbased_table, wwan_disable_pads,
						ARRAY_SIZE(wwan_disable_pads));
	}
}
