# list actions
help:
  just --list --unsorted

build:
  make -j

flash:
  flashrom -pch347_spi:spispeed=60M -w build/coreboot.rom -i bios -l binaries/rom.layout

stock:
  flashrom -pch347_spi:spispeed=60M -w binaries/cwwk-stock.bin
