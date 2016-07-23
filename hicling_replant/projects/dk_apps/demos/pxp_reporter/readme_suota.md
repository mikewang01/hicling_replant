SUOTA in Proximity Reporter {#pxp_suota}
======================

## Overview

This application, besides its main functionality described [here](@ref pxp), can be built with Software Update over the Air (SUOTA) support.

Features:

- Advertising starts automatically once the application starts, advertising contains:
 - Proximity Reporter Profile information.
 - SUOTA service information.
 - DIS information
- Standard applications for Proximity Reporter profile can be used to connect to device.
- The Dialog dedicated applications for `Android` and `iOS` `Dialog SUOTA` can connect to device to perform software update.
Those applications can be found in _Play Store_ or _Apple App Store_.

## Installation procedure

### Import projects

Import the following projects using 'SmartSnippets Studio'.

 * `mkimage` from `utilities/mkimage`
 * `bin2image` from `utilities/bin2image`
 * `uartboot` from `sdk/bsp/system/loaders/uartboot`
 * `libprogrammer` (Linux only) from `utilities/cli_programmer/libprogrammer`
 * `cli_programmer` (Linux only) from `utilities/cli_programmer/cli`
 * `scripts` from `utilities/scripts`
 * `suota_1_1_loader` from `sdk/bsp/system/loaders/suota_1_1_loader`
 * `pxp_reporter` from `projects/dk_apps/demos/pxp_reporter`

### Build

Build the following projects in Eclipse

- `mkimage`
- `bin2image`
- `uartboot` in `Release` configuration
- `libprogrammer` and `cli_programmer` in `Release_static` configuration (Linux only)
- `suota_1_1_loader` in `Release_QSPI` configuration
- `pxp_reporter` in `Release_QSPI_SUOTA` configuration

### Prepare SUOTA image

SUOTA image is a binary file with a proper header that can be sent to a target device from Android or iOS device.
To create an image, open command prompt, navigate to a `projects/dk_apps/demos/pxp_reporter` folder
and run script to create the image file.

To build an image in Windows run:

> `mkimage.bat <build_configuration>`

Where build_configuration may be DA14681-01-Release_QSPI_SUOTA, DA14681-01-Debug_QSPI_SUOTA, etc


To build an image in Linux run:

> `./mkimage.sh <build_configuration>`

Where build_configuration may be DA14681-01-Release_QSPI_SUOTA, DA14681-01-Debug_QSPI_SUOTA, etc

It prepares a `pxp_reporter.1.0.0.1.img` image file. The file name contains
a version number taken from a `sw_version.h`.

### Copy Image to Android or iOS device

The image file should be copied to Android phone or tablet or to iOS device inside the SUOTA folder of the device.
For iOS devices use iTunes.
For Android a USB cable can be used to connect the Android device with pc in order to copy the img file or the following command can be used to send the image:

> `adb push Debug_QSPI_SUOTA/pxp_reporter.1.0.0.1.img /sdcard/suota`

### Install bootloader and initial image

The bootloader and pxp reporter demo require a partition table that contains partitions for image storage.
If such partitions were not present on the target device the easiest way to fix this is to erase the existing
partition table sector. The bootloader will recreate it with correct values.
The following command writes the bootloader image and  the PXP image to the target device.

To program flash in Windows run:

> `initial_flash.bat <build_configuration>`

Where build_configuration may be DA14681-01-Release_QSPI_SUOTA, DA14681-01-Debug_QSPI_SUOTA, etc

To program flash in Linux run:

> `./initial_flash.sh <build_configuration>`

Where build_configuration may be DA14681-01-Release_QSPI_SUOTA, DA14681-01-Debug_QSPI_SUOTA, etc

When the initial flash is finished a device reboots and the application starts from the executable partition.

### Install bootloader only through eclipse and download the image through SUOTA
If only the bootloader needs to be programmed, this can be done through Eclipse by the following steps:
- Click on `suota_1_1_loader` in Project Explorer of eclipse so that `suota_1_1_loader` becomes the active project.
- Press program_qspi_jtag_win, program_qspi_jtag_linux (when J-Link debugger will be used to
  programming) or program_qspi_serial_win, program _qspi_serial_linux (when UART will be used to
  programming) button (it is under Run > External Tools > External Tool Configurations) to download
  the `suota_1_1_loader` to QSPI. If something goes wrong, reconnect the platform and repeat.

Then download the image through SUOTA by the following steps:

- Run the `./mkimage.sh` or the `mkimage.bat` to create a new image.
- Transfer the image to Android or iOS device.
- Find the device using SUOTA Application. The name of device shall be **Dialog PX Reporter**.
- Proceed with a software update in SUOTA Application.
- When programming is finished, the device reboots automatically and advertising starts again.

## Manual testing

- Build `pxp_reporter` `(Release_QSPI_SUOTA)` and `suota_1_1_loader` using eclipse.
- Program flash with the suota image using the `inital_flash.bat` or the `./initial_flash.sh` script.
- Change vesion number in the `sw_version.h` file and rebuild the `pxp_reporter` project.
- Run the `./mkimage.sh` or the `mkimage.bat` to create a new image.
- Transfer the image to Android or iOS device.
- Find the device using SUOTA Application. The name of device shall be **Dialog PX Reporter**.
- Proceed with a software update in SUOTA Application.
- When programming is finished, the device reboots automatically and advertising starts again.
- The device should be visible as **Dialog PX Reporter** with the new version number.
- When the application is running verify that Proximity Reporter profile is available and working.
- When there are other active connections, SUOTA should fail.
