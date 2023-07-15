## Interfacing stm32f103 with sd card
This project aims to demonstrate the integration of a FAT (File Allocation Table) library into an STM32 microcontroller. The FAT library allows the microcontroller to interface with a file system commonly used in storage devices like SD cards .

### pins connection
(MOSI1 )PA7 --> Sdcard module pin DI
(MISO1)PA6  --> Sdcard module pin DO
(SCLK1)PA5  --> Sdcard module pin SCLK
(CS)PA4     --> Sdcard module pin CS

### Dependencies
- FAT library source and doccan be found here :http://elm-chan.org/fsw/ff/00index_p.html
- Standard peripherals library source and doc can be found here : https://github.com/wajatimur/stm32f10x-stdperiph-lib
