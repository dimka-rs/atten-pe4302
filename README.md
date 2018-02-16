# atten-pe4302

Attenuator consists of:
* STM8S103F3P6 MCU
* 3 digits 7 segs common cathode LED indicator SMA420363
* 74HC595D decoder used to decode segments data
* 2 normal open buttons
* PE4302 attenuator

All ICs are placed on breakout boards, all components are mounted on a piece of protoboard. I have not done schematics since it is simple and should be quite clear from STM8CubeMX project. The only thing worth mentioning is external pullup required on pin B4, because it is true open drain and cannot drive up.


## Indicator SMA420363 pinout:

c1 sa sf c2 c3 sb  
Dig1, Dig2, Dig3  
se sd sh sc sg XX  

## Segments positions:

.a.  
fgb  
edch  
