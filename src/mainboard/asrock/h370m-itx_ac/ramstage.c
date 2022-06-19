/* SPDX-License-Identifier: GPL-2.0-only */

#include <device/device.h>
#include <intelblocks/cse.h>
#include <soc/ramstage.h>
#include "include/gpio.h"

void mainboard_silicon_init_params(FSPS_UPD *params)
{
	/*
	 * Enable Intel PTT (fTPM) via HECI FW_FEATURE_SHIPMENT_OVERRIDE.
	 * This must happen before FSP Silicon Init / End-of-Post, while
	 * HECI is still available. If PTT state changes, cse_enable_ptt()
	 * triggers a global reset; on the subsequent boot PTT is active
	 * and the CRB TPM at 0xfed40000 is usable. If PTT is already
	 * enabled this is a no-op.
	 */
	cse_enable_ptt(true);
}

static void init_mainboard(void *chip_info)
{
	mainboard_configure_gpios();
}

struct chip_operations mainboard_ops = {
	.init = init_mainboard,
};
