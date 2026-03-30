/* SPDX-License-Identifier: GPL-2.0-or-later */

#ifndef _SOC_NOVALAKE_TCSS_H_
#define _SOC_NOVALAKE_TCSS_H_

/* Thunderbolt firmware IMR status */
#define IOM_CSME_IMR_TBT_STATUS			0x14
#define TBT_VALID_AUTHENTICATION		BIT(30)

/* IOM aux bias control registers in REGBAR MMIO space */
#define IOM_AUX_BIAS_CTRL_PULLUP_OFFSET_0	0x1070
#define IOM_AUX_BIAS_CTRL_PULLUP_OFFSET(x)	(IOM_AUX_BIAS_CTRL_PULLUP_OFFSET_0 + (x) * 4)
#define IOM_AUX_BIAS_CTRL_PULLDOWN_OFFSET_0	0x1088
#define IOM_AUX_BIAS_CTRL_PULLDOWN_OFFSET(x)	(IOM_AUX_BIAS_CTRL_PULLDOWN_OFFSET_0 + (x) * 4)

#define BIAS_CTRL_VW_INDEX_SHIFT	16
#define BIAS_CTRL_BIT_POS_SHIFT		8

/*
 * The PCI-SIG engineering change requirement provides the ACPI additions for firmware latency
 * optimization. Both of FW_RESET_TIME and FW_D3HOT_TO_D0_TIME are applicable to the upstream
 * port of the USB4/TBT topology.
 */
/* Number of microseconds to wait after a conventional reset */
#define FW_RESET_TIME				50000

/* Number of microseconds to wait after data link layer active report */
#define FW_DL_UP_TIME				1

/* Number of microseconds to wait after a function level reset */
#define FW_FLR_RESET_TIME			1

/* Number of microseconds to wait from D3 hot to D0 transition */
#define FW_D3HOT_TO_D0_TIME			50000

/* Number of microseconds to wait after setting the VF enable bit  */
#define FW_VF_ENABLE_TIME			1

enum {
	TCSS_TYPE_C_PORT_USBC = 0x0,
	TCSS_TYPE_C_PORT_HDMI = 0x1,
	TCSS_TYPE_C_PORT_EDP = 0x2,
	TCSS_TYPE_C_PORT_USBA = 0x3,
	TCSS_TYPE_C_PORT_DISABLE = 0xF,
	TCSS_TYPE_C_PORT_DP_ONLY = 0x10,
	TCSS_TYPE_C_PORT_NO_PCIE = 0x11,
	TCSS_TYPE_C_PORT_FULL_FUN = 0x12
};

#endif /* _SOC_NOVALAKE_TCSS_H_ */
