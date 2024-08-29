# Minimal RZ/A1 build for Renesas GR-LYCHEE and GR-PEACH

Based on the `RZ_A1H_other_sample` code from Renesas, this repo aims to be a starting point for developers looking to write code for the [GR-LYCHEE](https://os.mbed.com/platforms/Renesas-GR-LYCHEE/) or [GR-PEACH](https://os.mbed.com/platforms/Renesas-GR-PEACH/) without the need to rely on Renesas's e2studio IDE or the IDE4GR. Unlike most of the RZA example code from Renesas and ARM/MBED, this code is not bound to FreeRTOS or MbedOS but is somewhat barebones and has been adapted to run successfully from SFLASH (`0x18000000`) on the GR-PEACH and GR-LYCHEE which are locked to boot mode 3 and boot mode 1, respectively.

While it's not all linked, most of the Renesas Sample code was left intact in the repo and will be adapted or cleared out over time.

This repository is not affiliated with or endorsed by Renesas. Copyrights belong to their respective holders and licenses contained in individual files supercede any overall license for this repository.

## Done

* Boots from SFLASH (`0x18000000`, with VBAR being redirected appropriately)
    * Some mangling of linker files and asm files was necessary
* Appropriate linker files being used based on board (meson configuration)
* Working L1 Cache
* Working L2 Cache
* Working interrupts with timer-based LED blinker working by default
* Working SCIF drivers with printf on serial (pins D0 & D1)

## To-do

* Working SPI (RSPI) driver
    * Working SDCard driver
    * Working fatfs driver on SDCard (without L2Cache issues...)
* Working ESP32 wireless (GR-LYCHEE only)
* Working Ethernet (GR-PEACH only)
* Working I2C (RIIC) driver with example (display?)
* Working PWM driver with example
* Working USB-CDC to replace hardware UART for printf
* Working audio driver

## Building

The Meson build system was used for this repo. Please ensure you have meson installed as well as the Arm GNU Toolchain on your path (I used 12.2).

To set up the build for the GR-LYCHEE and place binaries into the `build-lychee` subdirectory, run the following:

```meson setup --cross-file cross-file/rza1.ini -Dfirmware_target=gr-lychee build-lychee```

For the GR-PEACH, adjust to the following:

```meson setup --cross-file cross-file/rza1.ini -Dfirmware_target=gr-peach build-peach```

Then, to build just run the following and replace `build-directory` with the directory you pointed meson to above:

```meson compile -C build-directory```

## Flashing

Warning: This process overwrites the SFLASH on your GR device, wiping the `0x18000000` to `0x18004000` range where IDE4GR stores its MBED Arduino bootloader. If you want to go back to using IDE4GR, it *seems* to re-upload this bootloader as needed, but I can't make any guarantees about its future behaviour.

With the stock `main.c`, if, in addition to the power LED, you see one LED solid and one LED blinking every second, it probably worked.

### MBED/Daplink Mass Storage Device

If your GR device has a working MBED/Daplink firmware, just plug in the daplink's USB port to your computer, mount the `MBED` storage device, and copy the built `main_firmware.bin` file to the device. The drive will unmount itself and your firmware will be installed to SFLASH. You may need to reset or power cycle your GR board for the new firmware to run.

### SEGGER JLink, Black Magic Probe, and other debuggers

If you have an SWD header on your GR board, you can use a hardware debugger to `load` your firmware in GDB using the `main_firmware.elf` file. Note that your debugger must support writing to the GR device's particular flash chip.
