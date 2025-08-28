/* SPDX-License-Identifier: GPL-2.0-or-later */

#include <soc/ramstage.h>

void mainboard_silicon_init_params(FSP_S_CONFIG *params)
{
	/* Max payload 256B PCH_RP 1,7,9,10 */
	// params->PcieRpMaxPayload[0] = 1;
	// params->PcieRpMaxPayload[6] = 1;
	// params->PcieRpMaxPayload[8] = 1;
	// params->PcieRpMaxPayload[9] = 1;
}
