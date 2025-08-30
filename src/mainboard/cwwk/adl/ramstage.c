/* SPDX-License-Identifier: GPL-2.0-or-later */

#include <mainboard/gpio.h>
#include <pc80/i8254.h>
#include <soc/ramstage.h>

static void mainboard_init(void *chip_info)
{
	mainboard_configure_gpios();
}

static void mainboard_final(void *chip_info)
{
	beep(1500, 100);
}

struct chip_operations mainboard_ops = {
	.init = mainboard_init,
	.final = mainboard_final,
};
