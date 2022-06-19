# ASRock H370M-ITX/ac Mini-ITX Motherboard

This page describes how to run coreboot on the [ASRock H370m-ITX/ac].

## Technology

```{eval-rst}
+---------+---------------------------------------------------------------+
| CPU     | | Intel 8th/9th Gen (Coffeelake-S) Core Processors (LGA-1151) |
|         | | CPUs over 95W will be limited due to power design           |
+---------+---------------------------------------------------------------+
| DRAM    | 2 DIMM slots, DDR4 2666/2400/2133 MT/s                        |
+---------+---------------------------------------------------------------+
| Chipset | Intel H370                                                    |
+---------+---------------------------------------------------------------+
| SuperIO | Nuvoton NCT5567D-B                                            |
+---------+---------------------------------------------------------------+
| Boot    | USB, SATA, NVMe                                               |
+---------+---------------------------------------------------------------+
```

## Required proprietary blobs

To build full working image of coreboot, the following blobs are required:

```{eval-rst}
+-----------------+-------------------------------------------+-------------------------+
| Binary file     | Apply                                     | Required/Optional       |
+=================+===========================================+=========================+
| FSP-M & FSP-S   | Intel Firmware Support Package 2.1        | Required                |
|                 | 9th Generation Intel® Core™ processors    |                         |
|                 | and chipsets (formerly Coffee Lake)       |                         |
+-----------------+-------------------------------------------+-------------------------+
| IFD             | Intel Flash Descriptor                    | Required                |
+-----------------+-------------------------------------------+-------------------------+
| ME              | Intel Management Engine                   | Required                |
+-----------------+-------------------------------------------+-------------------------+
| GBE             | Gigabit Ethernet Configuration            | Optional                |
|                 |                                           | (MAC for LAN2)          |
+-----------------+-------------------------------------------+-------------------------+
```

### FSP

Intel company provides [Firmware Support Package (2.1)](../../soc/intel/fsp/index.md)
to initialize this generation silicon. Please see this
[document](../../soc/intel/code_development_model/code_development_model.md).

### IFD, ME, GBE

Use the [vendor's firmware] version 1.80 to extract the IFD, ME, GBE blobs from it, according to
the [Intel IFD Binary Extraction Tutorial](../../util/ifdtool/binary_extraction.md).

```bash
wget --tries=5 "https://web.archive.org/web/20260412110351/https://download.asrock.com/BIOS/1151/H370M-ITXac(4.30E)ROM.zip"
unzip "H370M-ITXac\(4.30E\)ROM.zip"
ifdtool --platform cnl -x H37MIA4.30E
File H37MIA4.30E is 16777216 bytes
  Flash Region 0 (Flash Descriptor): 00000000 - 00000fff
  Flash Region 1 (BIOS): 00300000 - 00ffffff
  Flash Region 2 (Intel ME): 00003000 - 002fffff
  Flash Region 3 (GbE): 00001000 - 00002fff
```

## Building coreboot

The following commands will help quickly configure and build a project for this board:

```bash
make distclean && \
touch .config && \
./util/scripts/config --enable VENDOR_ASROCK --enable BOARD_ASROCK_H370M_ITX && \
make olddefconfig
make
```

## Payloads

```{eval-rst}
+---------------+------+---------+
| OS / Payload  | EDK2 | SeaBIOS |
+===============+======+=========+
| Ubuntu 24.04  |      |    V    |
+---------------+------+---------+
| Nix 23.11     |  V   |    V    |
+---------------+------+---------+
| Windows 10    |      |    V    |
+---------------+------+---------+
| Windows 11    |  V   |         |
+---------------+------+---------+
```

- SeaBIOS (1.17.0)
- EDK2 (mrchromebox, tag: uefipayload_2603)

### Additional information

- Ubuntu 24.04 (Linux 6.8.0-41-generic)
- NixOS 23.11 (Linux 6.19.2-cachyos-lto)
- Microsoft Windows 10 Pro (22H2 2022)
- Microsoft Windows 11 Pro (25H2)

## Flashing coreboot

```{eval-rst}
+---------------------+--------------------------+
| Type                | Value                    |
+=====================+==========================+
| Socketed flash      | no                       |
+---------------------+--------------------------+
| Model               | W25Q128JVSIQ             |
+---------------------+--------------------------+
| Size                | 16 MiB                   |
+---------------------+--------------------------+
| Package             | SOIC-8                   |
+---------------------+--------------------------+
| Write protection    | chipset PRR              |
+---------------------+--------------------------+
| Dual BIOS feature   | no                       |
+---------------------+--------------------------+
| Internal flashing   | after flashing coreboot  |
+---------------------+--------------------------+
```

The SPI flash can be accessed using [flashrom]. By default, only the
BIOS region of the flash is writable:

```bash
flashrom -p internal -N -w coreboot.rom --ifd -i bios
```

If you wish to change any other region, such as the Management Engine
or firmware descriptor, then an external programmer is required. More
information about this [here](../../tutorial/flashing_firmware/index.md).

## External flashing pinout

The SPI flash is an 8-pin SOIC device. A nearby 10-pin header (`PH1`) exposes the SPI signals.

**Orientation:** Top view, CPU socket is at the top.

```text
        CPU SOCKET (TOP)
        ────────────────

DI (IO0 / MOSI)   -  01      10 - GND
CLK               -  02      09 - NC
                     __      08 - DO (IO1 / MISO)
VCC (3.3V)        -  04      07 - /CS
HOLD#/RESET (IO3) -  05      06 - /WP (IO2)
```

**Notes:**

* Pin **3 is not present** on this header.
* Pin **9 is not connected (NC)**.
* Signals correspond directly to the SPI flash pins.
* Use a **3.3V-compatible programmer only**.
* Incorrect wiring may permanently damage the board.

## Working

- Dual Channel DDR4 2666/2400/2133 MHz
- Intel UHD Graphics
  - DP
  - HDMI
    - VGA Option ROM
    - libgfxinit
    - GOP
- PCIe x16 Slot (Gen3)
- SATA ports
- USB 2.0 ports
- USB 3.2 ports
- M.2 Key-E 2230 slot for Wireless (PCIe x1, USB 2.0)
- M.2 Key-M 2242/2260/2280 for SSD/NVMe (PCIE x4, SATA3, needs manual switching see next chapter)
- LAN1 Intel I211AT, 10/100/1000 Mbps
- LAN2 Intel I219V, 10/100/1000 Mbps
- Realtek ALC892 HD Audio (line-out, mic-in)
- S3 suspend and wake
- Wake on LAN on both ports
- fTPM
- disabling ME with me_cleaner [XutaxKamay fork] (v1.2-9-gf20532d)

## Unknown/untested

- external TPM

## Display output notes

### libgfxinit GMBUS pin patch required for HDMI 2

libgfxinit uses Broxton-era GMBUS pin assignments which are incorrect for the Cannon Point PCH (H370). Without patching, HDMI 2 (DDI D) fails to read EDID and produces no output in SeaBIOS. All ports work correctly under Linux since i915 has its own CNP-specific pin table.

A patch to `3rdparty/libgfxinit/common/hw-gfx-gma-i2c.adb` is required. In the `GMBUS_Alternative_Pins` branch of `GMBUS0_PIN_PAIR_SELECT`, apply the following:

```{eval-rst}
+-------+-----------------------+--------------------+
| DDI   | Correct GMBUS Pin     | libgfxinit Default |
+=======+=======================+====================+
| DDI B | 4 (GMBUS_PIN_4_CNP)   | 1 (BXT_B) ✗        |
+-------+-----------------------+--------------------+
| DDI C | 2 (GMBUS_PIN_2_BXT)   | 2 (BXT_C) ✓        |
+-------+-----------------------+--------------------+
| DDI D | 1 (GMBUS_PIN_1_BXT)   | 3 (BXT_D) ✗        |
+-------+-----------------------+--------------------+
```

This board's DDC wiring swaps DDI B and DDI D relative to the CNP reference design. The pin values match the VBT DDC pin fields from the vendor firmware.

### Display port mapping

```{eval-rst}
+---------------------+-------+---------------------+
| Physical Connector  | DDI   | libgfxinit Port     |
+=====================+=======+=====================+
| DisplayPort         | DDI B | DP1                 |
+---------------------+-------+---------------------+
| HDMI 1              | DDI C | HDMI2               |
+---------------------+-------+---------------------+
| HDMI 2              | DDI D | HDMI3               |
+---------------------+-------+---------------------+
```

DDI A (eDP) and DDI E are not routed to any physical connector.

### VBT patching

The vendor VBT declares two phantom child devices: an eDP panel on DDI A (LFP1, device type `0x78c6`) and a DisplayPort output on DDI E (EFP4, device type `0x68c6`). These can be removed by zeroing out the respective device type fields in the VBT binary.

## SATA/PCIe muxing on M.2 slot

The ASRock H370M-ITX features a single M.2 Key-M slot that supports both SATA and PCIe (NVMe) storage devices. Unlike some boards that use physical jumper pins or automatic hardware sensing, ASRock implements this switching via **Intel Soft Straps** within the Flash Descriptor (FD).

The PCH uses High Speed I/O (HSIO) lanes that are multiplexed. Depending on the values of specific soft straps, the HSIO lanes connected to the M.2 slot are routed either to the internal SATA controller or the PCIe controller.

### Configuration Straps

The protocol is determined by the following PCH Straps:

| Strap | Offset | PCIe (NVMe) Value | SATA Value |
|-------|--------|-------------------|------------|
| **PCHSTRP28** | `0x170` | `0x00000000` | `0x00010000` |
| **PCHSTRP81** | `0x244` | `0x77775555` | `0x77577555` |

*Note: In PCHSTRP81, the nibble for Lane 25 switches from 5 to 7, and Lane 27 switches from 7 to 5 when moving from PCIe to SATA.*

### Switching Protocols

Since these settings reside in the Flash Descriptor, they cannot be changed by the BIOS at runtime. To switch the protocol, the Flash Descriptor must be modified and reflashed.

You can use `ifdtool` from the coreboot tree to manually set these straps using the `-S` and `-V` flags

```bash
# To switch to SATA mode using ifdtool:
ifdtool -S 28 -V 0x00010000 -S 81 -V 0x77577555 your_bios.bin

# To switch to PCIe mode using ifdtool:
ifdtool -S 28 -V 0x00000000 -S 81 -V 0x77775555 your_bios.bin
```

> **Warning:** These straps are specific to the electrical routing of the ASRock H370M-ITX. Applying these values to other H370 boards may disable HSIO lanes used for USB, Ethernet, or other critical peripherals.

For more information on how the PCH handles these configurations, see the [Soft Straps](../../getting_started/gpio.md#soft-straps) section.

### SATA Port Conflict

**Important:** Due to HSIO lane sharing, using a SATA-type M.2 device has a hardware trade-off:
* If the M.2 slot is set to **SATA mode** and a device is present, physical port **SATA3_1** will be disabled.
* If using an **NVMe (PCIe) device**, all physical SATA ports remain functional.

## Extra links

[ASRock H370M-ITXac]: https://web.archive.org/web/20250504092857/https://www.asrock.com/mb/Intel/H370M-ITXac/index.de.asp
[vendor's firmware]: https://web.archive.org/web/20260412110351/https://download.asrock.com/BIOS/1151/H370M-ITXac(4.30E)ROM.zip
[flashrom]: https://flashrom.org/
[XutaxKamay fork]: https://github.com/XutaxKamay/me_cleaner
