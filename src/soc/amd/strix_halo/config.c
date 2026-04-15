/* SPDX-License-Identifier: GPL-2.0-only */

/* TODO: Update for Strix Halo */

#include <amdblocks/chip.h>
#include <device/device.h>
#include <static.h>
#include "chip.h"

const struct soc_amd_common_config *soc_get_common_config(void)
{
	const struct soc_amd_strix_halo_config *cfg = config_of_soc();
	return &cfg->common_config;
}
