# STM32-CDC-Bootloader

A bootloader based on USB CDC protocol for STM32 F1, F3 and F4 family MCUs.



For each family the repository contains a respective Eclipse project.

### Features:
- no special drive installation: the device with this bootloader will enumerate as a serial COM port.
- the source files are partially based on libmaple core files, also included in the repository.
- additonally, some CMSIS files (slightly modified) are also used.
- in order to upload a program with the bootloader, a special utility program is needed, see [CDC flasher](https://github.com/stevstrong/CDC-flasher).
