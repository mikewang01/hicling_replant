Attribute permission API test application {#ble_att_perm_test}
===============================

## Overview

This application is used to test the funcionality of the BLE SDK API that
controls the permissions of attribute database entries. It implements a custom
service, which contains characteristics with different properties and
permissions.

Features:

- No tunable features exist.

## Installation procedure

The project is located in folder projects/dk_apps/high_level_tests/ble_att_perm_test

Choose one of the two independent ways:

### Console method

- Using 'SmartSnippets Studio' import the project and build it for chosen configuration
  (Release_QSPI or Debug_QSPI).
- Connect the platform (USB2 connector) to the PC with using USB cable.

  * QSPI

    Import scripts, uartboot, cli_programmer and libprogrammer projects.
    Build uartboot in Release mode and cli_programmer in Release_static.
    Make step 2 without running yet. Click on Debug_QSPI or Release_QSPI (it depends on how flash_test
    had been built) folder. Press program_qspi_win or program_qspi_linux button to download Flash test
    demo to QSPI (follow the instructions in console). When something went wrong reconnect the platform
    and repeat. Run from Eclipse (flash_test_qspi).

          Note for Windows users: cli_programmer can be built as a Visual Studio project.

- When demo menu will be visible in terminal it means that application was started. Choose the right
  option to run proper test.

### Eclipse method

Using 'SmartSnippets Studio' import the project and build it.
This project is available for QSPI mode so you need to import two more projects (scripts & uartboot).
Build uartboot on Release mode. Then press the Run program_qspi_win button
(it is under Run > External Tools > External Tool Configurations) to download the SPS demo to QSPI.

## Attribute database used

Application will register a custom service
in ATT database with a UUID of 91a7608d-4456-479d-b9b1-4706e8711cf8.
Then the following characteristics are set up under this service:
- Characteristic 1 with UUID 25047e64-657c-4856-afcf-e315048a965d
- Characteristic 2 with UUID 25047e64-657c-4856-afcf-e315048a965e
- Characteristic 3 with UUID 25047e64-657c-4856-afcf-e315048a965f
- Characteristic 4 with UUID 25047e64-657c-4856-afcf-e315048a9660
- Characteristic 5 with UUID 25047e64-657c-4856-afcf-e315048a9661
- Characteristic 6 with UUID 25047e64-657c-4856-afcf-e315048a9662
- Characteristic 7 with UUID 25047e64-657c-4856-afcf-e315048a9663
- Characteristic 8 with UUID 25047e64-657c-4856-afcf-e315048a9664

Every characteristic is one byte long.

Characteristics 1 to 3 are read only.

Characteristic 1 can be read unconditionaly.

Characteristic 2 requires encryption (so any bonding would suffice).

Characteristic 3 requires an authenticated bonding.

Characteristics 4 to 7 are read/write. They can be read unconditionaly.

Characteristic 4 can be written unconditionaly.

Characteristic 5 requires encryption for writing (so any bonding would suffice).

Characteristic 6 requires an authenticated bonding for writing.

Characteristic 7 requires an authenticated bonding for both reading and writing actions.

Characteristic 8 can be read and produces notifications. Every 5 seconds the
value of the characteristic is increased by one modulo 255 and a notification
is produced with the new value.

## Testing method

The test that confirms the correctr behavior of our device should attempt
read and write operations on each characteristic. The operations should be
repeated three times: Once without any security enabled, once after a simple bonding
and once after an authenticated bonding.


