/* SPDX-License-Identifier: GPL-2.0-only */

#include <acpi/acpi.h>
#include <bootblock_common.h>
#include <device/pnp_ops.h>
#include <superio/nuvoton/common/nuvoton.h>
#include <superio/nuvoton/nct5539d/nct5539d.h>

#define GLOBAL_DEV PNP_DEV(0x2e, 0)
#define SERIAL_DEV PNP_DEV(0x2e, NCT5539D_SP1)
#define ACPI_DEV   PNP_DEV(0x2e, NCT5539D_ACPI)

void bootblock_mainboard_early_init(void)
{
	nuvoton_pnp_enter_conf_state(GLOBAL_DEV);

	u8 cr26 = pnp_read_config(GLOBAL_DEV, 0x26);
	pnp_write_config(GLOBAL_DEV, 0x26, cr26 | (1 << 4));
	pnp_write_config(GLOBAL_DEV, 0x13, 0xff); // Device IRQ Polarity for Channel<15:8>
	pnp_write_config(GLOBAL_DEV, 0x14, 0xff); // Device IRQ Polarity for Channel<7:0>
	pnp_write_config(GLOBAL_DEV, 0x26, cr26); // restore

	pnp_write_config(GLOBAL_DEV, 0x1a, 0x38); // Pin39, Pin23
	pnp_write_config(GLOBAL_DEV, 0x1b, 0x76); // Pin41, Pin46, Pin22
	pnp_write_config(GLOBAL_DEV, 0x1c, 0x90); // AUXFANIN2
	pnp_write_config(GLOBAL_DEV, 0x24, 0xff); // *FANOUT*
	pnp_write_config(GLOBAL_DEV, 0x2a, 0x40); // Pin13,...Pin20
	pnp_write_config(GLOBAL_DEV, 0x2c, 0x40); // Pin34, Pin58, Pin60
	pnp_write_config(GLOBAL_DEV, 0x2d, 0x80); // Pin42

	if (!acpi_is_wakeup_s3()) {
		pnp_set_logical_device(ACPI_DEV);
		pnp_write_config(ACPI_DEV, 0xe1, 0x07); // KBC Wake-Up Index Register
		pnp_write_config(ACPI_DEV, 0xe2, 0x02); // KBC Wake-Up Index Register
		pnp_write_config(ACPI_DEV, 0xe4, 0x38); // Power-loss control, always turn on
		pnp_write_config(ACPI_DEV, 0xe6, 0xa1); // 300ms delay
		pnp_write_config(ACPI_DEV, 0xe7, 0x01); // Hardware Monitor RESET, LRESET#
		pnp_write_config(ACPI_DEV, 0xec, 0x04); // ACPI Control and Status Register
		pnp_write_config(ACPI_DEV, 0xf0, 0x30); // DEEP_S5_1, 3VSBSW
		pnp_write_config(ACPI_DEV, 0xf7, 0xc0); // PSIN_BLK_EN, RSTOUT1# Push-Pull
	}

	pnp_set_logical_device(SERIAL_DEV);

	nuvoton_pnp_exit_conf_state(GLOBAL_DEV);
	nuvoton_enable_serial(SERIAL_DEV, CONFIG_TTYS0_BASE);
}
