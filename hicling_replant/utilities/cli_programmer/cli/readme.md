CLI programmer {#cli_programmer}
================================

## Overview

`cli_programmer` is a command line tool for reading & writing to FLASH/OTP/RAM.
It also provides some extra functions like loading & executing an image from RAM. 
The tool communicates with the target device over uart port or jtag interface.
It executes on Windows and Linux platforms.


> Note:
> Writing an image to flash requires adding a header to the image. This process is handled by the 'bin2image' tool, 
> or the 'cli_programmer write_qspi_exec' command

## Usage

To run cli_programmer user has to specify interface (GDB server of serial port) and requested command.

    cli_programmer [<options>] <interface> <command> [<args>]

### Interface

* the serial port file name as presented by the operating system e.g. \b `COM5` (Windows), \b `/dev/ttyUSB0` (Linux), or
* \b `gdbserver`, if jtag interface is to be used (J-Link debugger with the GDB server)

### Commands and arguments

    write_qspi <address> <file> [<size>]

Writes up to `size` bytes of `file` into the FLASH at `address`. If `size` is omitted, a complete
file is written.

    write_qspi_bytes <address> <data1> [<data2> [...]]

Writes bytes specified on command line into the FLASH at `address`.

    write_qspi_exec <image_file>

Writes binary file (.bin) to flash at address 0, after adding header for execution in place (cached mode).

    write_suota_image <image_file> <version>

Writes SUOTA enabled `image_file` to executable partition. 
The user supplied `version` string goes to image header.
        
    read_qspi <address> <file> <size>

Reads `size` bytes from the FLASH memory, starting at `address` into `file`. If `file` is specified as
either '-' or '--', data is output to stdout as hexdump. The hexdump is either 16-bytes (-) or 32-bytes
(--) wide.

    erase_qspi <address> <size>

Erases `size` bytes of the FLASH, starting at `address`.
Note: an actual erased area may be different due to the size of an erase block.

    chip_erase_qspi

Erases the whole FLASH.

    copy_qspi <address_ram> <address_qspi> <size>

Copies `size` bytes from the RAM memory, starting at `address_ram` to FLASH at `address_flash`.
This is an advanced command an is not needed by end user.

    is_empty_qspi [start_address size]

Checks that FLASH contains only 0xFF values. If no arguments are specified starting address is 0 and size is 1M.
Command prints whether flash is empty and if offset of first non empty byte.

    read_partition_table

Reads the partition table (if any exists) and prints its contents.

    write <address> <file> [<size>]

Writes up to `size` bytes of `file` into the RAM memory at `address`. If `size` is omitted, a complete
`file` is written.

    read <address> <file> <size>

Reads `size` bytes from the RAM memory, starting at `address` into `file`. If `file` is specified as
either '-' or '--', data is output to stdout as hexdump. The hexdump is either 16-bytes (-) or 32-bytes
(--) wide.

    write_otp <address> <length> [<data> [<data> [...]]]

Writes `length` words to the OTP at `address`. `data` are 32-bit words to be written, if less than
`length` words are specified, remaining words are assumed to be 0x00.

    read_otp <address> <length>

Reads `length` 32-bit words from the OTP address `address`.

    write_otp_file <file>

Writes data to the OTP as defined in `file` (default specified values are written).

    read_otp_file <file>

Reads data from the OTP as defined in `file` (cells with default value provided are read) contents of
each cell is printed to stdout.

    write_tcs <length> [<reg_addr> <reg_data> [<reg_addr> <reg_data>  [...]]]

Writes `length` 64-bit words to the OTP TCS section at first available (filled with 0) section of
size `length`.

`reg_addr`: the register address. It will be written as a 64-bit word [`reg_addr`, `~reg_addr`]

`reg_data`: the register data. It will be written as a 64-bit word [`reg_data`, `~reg_data`]

    boot

Boots the 2nd stage bootloader or the application binary (defined with -b) and exits.

    read_chip_info 

Reads chip information from chip revision registers and otp header.

### General options

    -h

Prints help screen and exits.

    --save-ini

Saves CLI programmer configuration to the `cli_programmer.ini` file and exits.

    -b file

Filename of 2nd stage bootloader or an application binary.

### GDB server specific options

    -p <port_num>

TCP port number that GDB server listens to. The Default value is 2331.

    -r <host>

GNU server host. The default is `localhost`.

    --no-kill

Don't stop already running GDB server instance.

    --gdb-cmd <cmd>

GDB server command used for executing and passing the right parameters to GDB server.
Without this parameter no GDB server instance will be started or stopped. As GDB server command line can
be quite long, it is recommended that it is stored in cli_programmer.ini file using --save-ini command line option.

### Serial port specific options

    -s <baudrate>

Baud rate used for UART by uartboot. The parameter is patched to the uploaded uartboot binary
(in that way passed as a parameter). This can be 9600, 19200, 57600 (default), 115200, 230400,
500000, 1000000.

    -i <baudrate>

Initial baud rate used for uploading the `uartboot` or a user supplied binary. This depends on
the rate used by the bootloader of the device. The default behavior is to use the value passed by
'-s' or its default, if the parameter is not given. The argument is ignored by the `boot` command.
'-s' option should be used in this case.

    --tx-port <port_num>

GPIO port used for UART Tx by the `uartboot`. This parameter is patched to the uploaded uartboot
binary (in that way passed as a parameter). The default value is 1. This argument is ignored when
the `boot` command is given.

    --tx-pin <pin_num>

GPIO pin used for UART Tx by uartboot. This parameter is patched to the uploaded uartboot binary
(in that way passed as a parameter). The default value is 3. The argument is ignored when the `boot`
command is given.

    --rx-port <port_num>

GPIO port used for UART Rx by uartboot. This parameter is patched to the uploaded uartboot binary
(in that way passed as a parameter). The default value is 2. The argument is ignored when the `boot`
command is given.

    --rx-pin <pin_num>

GPIO pin used for UART Rx by uartboot. This parameter is patched to the uploaded uartboot binary
(in that way passed as a parameter). The default value is 3. The argument is ignored when the `boot`
command is given.

    -w timeout

Serial port communication timeout is used only during download of uartboot binary, if during this time
board will not respond cli_programmer exits with timeout error.

### bin2image options

    --prod-id <id>   DA14680-00, DA14681-00, DA14680-01, DA14681-01

Selects the chip product id. This option applies only when write_qspi_exec cmd is used.
It instructs cli_programmer to set the flash header which corresponds to the selected chip revision.

    --enable-uart y|n

Enables/disables booting from UART. 
It applies only for DA14681-00 product id and write_qspi_exec cmd.

    --ram-shuffling <n>    configure the DataRAM cell shuffling:
                           0:  8 - 24 - 32 (default)
                           1: 24 -  8 - 32
                           2: 32 -  8 - 24
                           3: 32 - 24 -  8

It applies only for DA14681-00 product id and write_qspi_exec cmd.

### Configuration file

When cli_programmer is executed it first tries to read cli_programmer.ini file which may contain various 
cli_programmer options. Instead of creating this file manually, user should use --save-ini command line option.
Format of cli_programmer.ini adheres to standard Windows ini file syntax.
The cli_programmer looks for ini file in the following locations:

- current directory
- home directory
- cli_programmer executable directory

## Usage examples

Upload binary data to FLASH.

    cli_programmer COM40 write_qspi 0x0 data_i
    cli_programmer /dev/ttyUSB0 write_qspi 0x0 data_i

Upload binary data to FLASH using maximum serial port baudrate.

    cli_programmer -s 1000000 -i 57600 COM40 write_qspi 0x0 data_i

Read data from FLASH to local file.

    cli_programmer COM40 read_qspi 0x0 data_o 0x100

Upload custom binary `test_api.bin` to RAM and execute it.

    cli_programmer -b test_api.bin COM40 boot

Modify FLASH at specified location with arguments passed in command line.

    cli_programmer COM40 write_qspi_bytes 0x80000 0x11 0x22 0x33

Run a few commands with uartboot, using UART Tx/Rx P1_3/P2_3 at baud rate 115200
(initial rate for uartboot uploading is 9600).

    cli_programmer -i 9600 -s 115200 --tx-port 1 --tx-pin 3 --rx-port 2 --rx-pin 3 COM40 write_qspi 0x0 data_i
    cli_programmer -i 9600 -s 115200 --tx-port 1 --tx-pin 3 --rx-port 2 --rx-pin 3 COM40 read_qspi 0x0 data_o 0x100

Read FLASH contents (10 bytes at address 0x0).

    cli_programmer gdbserver read_qspi 0 -- 10

Write register 0x50003000 with value 0x00FF and register 0x50003002 with value 0x00AA.

    cli_programmer gdbserver write_tcs 4 0x50003000 0x00FF 0x50003002 0x00AA

Write settings to the `cli_programmer.ini` file. Long bootloader path is passed with -b option and command line to start GDB server is passed with --gdb-cmd.
In this example GDB server command line contains arguments and path to executable has space so whole command line is put in quotes and quotes required by
Windows path are additionally escaped.

    cli_programmer -b c:\users\jon\sdk\bsp\system\loaders\uartboot\Release\uartboot.bin --save-ini --gdb-cmd "\"C:\Program Files\SEGGER\JLink_V510d\JLinkGDBServerCL.exe\" -if SWD -device Cortex-M0 -singlerun -silent -speed auto"

Program a DA14681-01 chip with an executable flash image.

    cli_programmer --prod-id DA14681-01 gdbserver write_qspi_exec ../../../../projects/dk_apps/features/tickless/DA14681-01-Debug_QSPI/tickless.bin

## Building cli_programmer

- 'cli_programmer' makes use of the 'libprogrammer' library which implements the underlying functionality 
on the host side. 'cli_programmer' can be linked either statically or dynamically with 'libprogrammer'.

- 'cli_programmer' uses 'uartboot' application which acts as a secondary bootloader which cli_programmer 
downloads to the target for performing the read/write operations. 

- The project is located in the `utilities/cli_programmer/cli` folder

- Build configurations:

  * Debug	 		- Debug version for Linux
  * Debug_static 		- Debug version linked with a static version of libprogrammer - recommended
                   		for Linux. It also builds uartboot project and includes it in cli_programmer executable
  * Debug_static_win32		- Debug version for Windows linked with a static version of libprogrammer 

  * Release	   		- Release version for Linux
  * Release_static 		- Release version linked with a static version of libprogrammer - recommended
                     		for Linux. It also builds uartboot project and includes it in cli_programmer executable
  * Release_static_win32	- Release version for Windows linked with a static version of libprogrammer

- Build instructions
  * Import `libprogrammer`, `cli_programmer` and `uartboot` into Eclipse
  * Build `libprogrammer` , `cli_programmer` and `uartboot` in `Release_static` configuration (recommended)
  * Run `cli_programmer` with proper parameters, described in `Usage` and `Commands and arguments` sections

> Note: A prebuilt version of cli_programmer can be found under SDK's `binaries` folder
