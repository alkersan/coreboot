/* SPDX-License-Identifier: GPL-2.0-or-later */

#include <intelblocks/gpio.h>
#include <intelblocks/pcr.h>
#include <soc/pcr_ids.h>
#include <soc/pmc.h>

static const struct reset_mapping rst_map[] = {
	{ .logical = PAD_RESET(PWROK), .chipset = 0U << 30 },
	{ .logical = PAD_RESET(DEEP), .chipset = 1U << 30 },
	{ .logical = PAD_RESET(PLTRST), .chipset = 2U << 30 },
	{ .logical = PAD_RESET(GLBRST), .chipset = 3U << 30 },
};

/*
 * Nova Lake adopts new pinctrl schema, where each GPIO community has its own
 * pinctrl instance and a group is not required to be aligned with 32 boundary.
 * SOC_INTEL_COMMON_BLOCK_GPIO_MULTI_ACPI_DEVICES must be selected and
 * the offset from its GPIO's community is used as the ACPI GPIO number directly.
 * Therefore, the .acpi_pad_base field in 'pad_group' structure is not used for
 * constructing APIC gpio number and INTEL_GPP() is used for all GPIO groups
 * instead. Please see function gpio_acpi_pin() in
 * src/soc/intel/common/block/gpio/gpio.c for more details.
 */
static const struct pad_group nvl_community0_groups[] = {
	INTEL_GPP(GPP_D00, GPP_D00, GPP_I3C_CLK_LPBK),	/* GPP_D */
	INTEL_GPP(GPP_D00, GPP_C00, GPP_C23),		/* GPP_C */
};

static const struct pad_group nvl_community1_groups[] = {
	INTEL_GPP(GPP_F00, GPP_F00, GPP_GSPI0_CLK_LOOPBK),		/* GPP_F */
	INTEL_GPP(GPP_F00, GPP_E00, GPP_THC0_GSPI0_I3C0_I3C3_CLK_LPBK),	/* GPP_E */
};

static const struct pad_group nvl_community3_groups[] = {
	INTEL_GPP(GPP_EPD_ON, GPP_EPD_ON, GPP_VDD2_PWRGD),			/* GPP_CPUJTAG1 */
	INTEL_GPP(GPP_EPD_ON, GPP_JTAG_MBPB0, GPP_DDSP_HPDALV),			/* GPP_CPUJTAG */
	INTEL_GPP(GPP_EPD_ON, GPP_H00, GPP_LPI3C3_CLK_LPBK),			/* GPP_H */
	INTEL_GPP(GPP_EPD_ON, GPP_A00, GPP_SPI0_CLK_LOOPBK),			/* GPP_A */
	INTEL_GPP(GPP_EPD_ON, GPP_VGPIO3_USB_OCB_RX_0, GPP_VGPIO3_EC_SCI1),	/* GPP_VGPIO3 */
};

static const struct pad_group nvl_community4_groups[] = {
	INTEL_GPP(GPP_S00, GPP_S00, GPP_S07),	/* GPP_S */
};

static const struct pad_group nvl_community5_groups[] = {
	INTEL_GPP(GPP_B00, GPP_B00, GPP_GSPI2_CLK_LPBK),	/* GPP_B */
	INTEL_GPP(GPP_B00, GPP_V00, GPP_V17),			/* GPP_V */
	INTEL_GPP(GPP_B00, GPP_VGPIO0, GPP_VGPIO47),		/* GPP_VGPIO */
};

static const struct pad_community nvl_communities[] = {
	[COMM_0] = { /* GPP: D, C */
		.port = PID_GPIOCOM0,
		.first_pad = COM0_GRP_PAD_START,
		.last_pad = COM0_GRP_PAD_END,
		.num_gpi_regs = NUM_GPIO_COM0_GPI_REGS,
		.pad_cfg_base = PAD_CFG_BASE,
		.pad_cfg_lock_offset = PAD_CFG_LOCK_REG_0,
		.host_own_reg_0 = HOSTSW_OWN_REG_0,
		.gpi_int_sts_reg_0 = GPI_INT_STS_0,
		.gpi_int_en_reg_0 = GPI_INT_EN_0,
		.gpi_gpe_sts_reg_0 = GPI_GPE_STS_0,
		.gpi_gpe_en_reg_0 = GPI_GPE_EN_0,
		.gpi_smi_sts_reg_0 = GPI_SMI_STS_0,
		.gpi_smi_en_reg_0 = GPI_SMI_EN_0,
		.max_pads_per_group = GPIO_MAX_NUM_PER_GROUP,
		.name = "GPP_D_C",
		.acpi_path = "\\_SB.PCI0.GPI0",
		.reset_map = rst_map,
		.num_reset_vals = ARRAY_SIZE(rst_map),
		.groups = nvl_community0_groups,
		.num_groups = ARRAY_SIZE(nvl_community0_groups),
	},
	[COMM_1] = { /* GPP: F, E */
		.port = PID_GPIOCOM1,
		.first_pad = COM1_GRP_PAD_START,
		.last_pad = COM1_GRP_PAD_END,
		.num_gpi_regs = NUM_GPIO_COM1_GPI_REGS,
		.pad_cfg_base = PAD_CFG_BASE,
		.pad_cfg_lock_offset = PAD_CFG_LOCK_REG_0,
		.host_own_reg_0 = HOSTSW_OWN_REG_0,
		.gpi_int_sts_reg_0 = GPI_INT_STS_0,
		.gpi_int_en_reg_0 = GPI_INT_EN_0,
		.gpi_gpe_sts_reg_0 = GPI_GPE_STS_0,
		.gpi_gpe_en_reg_0 = GPI_GPE_EN_0,
		.gpi_smi_sts_reg_0 = GPI_SMI_STS_0,
		.gpi_smi_en_reg_0 = GPI_SMI_EN_0,
		.max_pads_per_group = GPIO_MAX_NUM_PER_GROUP,
		.name = "GPP_F_E",
		.acpi_path = "\\_SB.PCI0.GPI1",
		.reset_map = rst_map,
		.num_reset_vals = ARRAY_SIZE(rst_map),
		.groups = nvl_community1_groups,
		.num_groups = ARRAY_SIZE(nvl_community1_groups),
	},
	[COMM_3] = { /* GPP: CPUJTAG1, CPUJTAG, H, A, VGPIO3 */
		.port = PID_GPIOCOM3,
		.first_pad = COM3_GRP_PAD_START,
		.last_pad = COM3_GRP_PAD_END,
		.num_gpi_regs = NUM_GPIO_COM3_GPI_REGS,
		.pad_cfg_base = PAD_CFG_BASE,
		.pad_cfg_lock_offset = PAD_CFG_LOCK_REG_0,
		.host_own_reg_0 = HOSTSW_OWN_REG_0,
		.gpi_int_sts_reg_0 = GPI_INT_STS_0,
		.gpi_int_en_reg_0 = GPI_INT_EN_0,
		.gpi_gpe_sts_reg_0 = GPI_GPE_STS_0,
		.gpi_gpe_en_reg_0 = GPI_GPE_EN_0,
		.gpi_smi_sts_reg_0 = GPI_SMI_STS_0,
		.gpi_smi_en_reg_0 = GPI_SMI_EN_0,
		.max_pads_per_group = GPIO_MAX_NUM_PER_GROUP,
		.name = "GPP_CPUJTAG1_CPUJTAG_H_A_VGPIO3",
		.acpi_path = "\\_SB.PCI0.GPI3",
		.reset_map = rst_map,
		.num_reset_vals = ARRAY_SIZE(rst_map),
		.groups = nvl_community3_groups,
		.num_groups = ARRAY_SIZE(nvl_community3_groups),
	},
	[COMM_4] = { /* GPP: S */
		.port = PID_GPIOCOM4,
		.first_pad = COM4_GRP_PAD_START,
		.last_pad = COM4_GRP_PAD_END,
		.num_gpi_regs = NUM_GPIO_COM4_GPI_REGS,
		.pad_cfg_base = PAD_CFG_BASE,
		.pad_cfg_lock_offset = PAD_CFG_LOCK_REG_0,
		.host_own_reg_0 = HOSTSW_OWN_REG_0,
		.gpi_int_sts_reg_0 = GPI_INT_STS_0,
		.gpi_int_en_reg_0 = GPI_INT_EN_0,
		.gpi_gpe_sts_reg_0 = GPI_GPE_STS_0,
		.gpi_gpe_en_reg_0 = GPI_GPE_EN_0,
		.gpi_smi_sts_reg_0 = GPI_SMI_STS_0,
		.gpi_smi_en_reg_0 = GPI_SMI_EN_0,
		.max_pads_per_group = GPIO_MAX_NUM_PER_GROUP,
		.name = "GPP_S",
		.acpi_path = "\\_SB.PCI0.GPI4",
		.reset_map = rst_map,
		.num_reset_vals = ARRAY_SIZE(rst_map),
		.groups = nvl_community4_groups,
		.num_groups = ARRAY_SIZE(nvl_community4_groups),
	},
	[COMM_5] = { /* GPP: B, V, VGPIO */
		.port = PID_GPIOCOM5,
		.first_pad = COM5_GRP_PAD_START,
		.last_pad = COM5_GRP_PAD_END,
		.num_gpi_regs = NUM_GPIO_COM5_GPI_REGS,
		.pad_cfg_base = PAD_CFG_BASE,
		.pad_cfg_lock_offset = PAD_CFG_LOCK_REG_0,
		.host_own_reg_0 = HOSTSW_OWN_REG_0,
		.gpi_int_sts_reg_0 = GPI_INT_STS_0,
		.gpi_int_en_reg_0 = GPI_INT_EN_0,
		.gpi_gpe_sts_reg_0 = GPI_GPE_STS_0,
		.gpi_gpe_en_reg_0 = GPI_GPE_EN_0,
		.gpi_smi_sts_reg_0 = GPI_SMI_STS_0,
		.gpi_smi_en_reg_0 = GPI_SMI_EN_0,
		.max_pads_per_group = GPIO_MAX_NUM_PER_GROUP,
		.name = "GPP_B_V_VGPIO",
		.acpi_path = "\\_SB.PCI0.GPI5",
		.reset_map = rst_map,
		.num_reset_vals = ARRAY_SIZE(rst_map),
		.groups = nvl_community5_groups,
		.num_groups = ARRAY_SIZE(nvl_community5_groups),
	},
};

const struct pad_community *soc_gpio_get_community(size_t *num_communities)
{
	*num_communities = ARRAY_SIZE(nvl_communities);
	return nvl_communities;
}

const struct pmc_to_gpio_route *soc_pmc_gpio_routes(size_t *num)
{
	static const struct pmc_to_gpio_route routes[] = {
		{ PMC_GPP_V, GPP_V },
		{ PMC_GPP_C, GPP_C },
		{ PMC_GPP_F, GPP_F },
		{ PMC_GPP_E, GPP_E },
		{ PMC_GPP_A, GPP_A },
		{ PMC_GPP_H, GPP_H },
		{ PMC_GPP_VGPIO3, GPP_VGPIO3 },
		{ PMC_GPP_B, GPP_B },
		{ PMC_GPP_D, GPP_D },
		{ PMC_GPP_S, GPP_S },
	};
	*num = ARRAY_SIZE(routes);
	return routes;
}
