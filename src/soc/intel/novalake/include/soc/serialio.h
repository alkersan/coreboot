/* SPDX-License-Identifier: GPL-2.0-only */

#ifndef _SOC_NOVALAKE_SERIALIO_H_
#define _SOC_NOVALAKE_SERIALIO_H_

enum {
	PchSerialIoDisabled,
	PchSerialIoPci,
	PchSerialIoHidden,
	PchSerialIoLegacyUart,
	PchSerialIoSkipInit
};

enum {
	PchSerialPio,
	PchSerialDma
};

enum {
	PchSerialIoIndexI2C0,
	PchSerialIoIndexI2C1,
	PchSerialIoIndexI2C2,
	PchSerialIoIndexI2C3,
	PchSerialIoIndexI2C4,
	PchSerialIoIndexI2C5
};

enum {
	PchSerialIoIndexI3C0,
	PchSerialIoIndexI3C1
};

enum {
	PchSerialIoIndexGSPI0,
	PchSerialIoIndexGSPI1,
	PchSerialIoIndexGSPI2,
};

enum {
	PchSerialIoIndexUART0,
	PchSerialIoIndexUART1,
	PchSerialIoIndexUART2
};

#endif /* _SOC_NOVALAKE_SERIALIO_H_ */
