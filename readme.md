

interfacing stm32f10x with Sdcard (spi1)

this project is tested with stm32f103c8 

complier :GCC ARM

pins connection

(MOSI1 )PA7-->Sdcard module pin DI
(MISO1)PA6-->Sdcard module pin DO
(SCLK1)PA5-->Sdcard module pin SCLK
PA4-->Sdcard module pin CS

                       
fat library source and documentation :
http://elm-chan.org/fsw/ff/00index_p.html