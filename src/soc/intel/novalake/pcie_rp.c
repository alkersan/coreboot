/* SPDX-License-Identifier: GPL-2.0-only */

#include <intelblocks/pcie_rp.h>
#include <soc/pci_devs.h>
#include <soc/pcie.h>

static const struct pcie_rp_group tbt_rp_groups[] = {
	{ .slot = PCI_DEV_SLOT_TBT, .count = CONFIG_MAX_TBT_ROOT_PORTS, .lcap_port_base = 21 },
	{ 0 }
};

static const struct pcie_rp_group nvlp_rp_groups[] = {
	{ .slot = PCI_DEV_SLOT_PCIE_1,	.count = 8, .lcap_port_base = 1  },
	{ .slot = PCI_DEV_SLOT_PCIE_2,	.count = 6, .lcap_port_base = 1  },
	{ 0 }
};


const struct pcie_rp_group *get_pcie_rp_table(void)
{
	return nvlp_rp_groups;
}

const struct pcie_rp_group *get_tbt_pcie_rp_table(void)
{
	return tbt_rp_groups;
}

enum pcie_rp_type soc_get_pcie_rp_type(const struct device *dev)
{
	return PCIE_RP_PCH;
}

int soc_get_cpu_rp_vw_idx(const struct device *dev)
{
	return -1;
}
