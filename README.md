# Digital_clock
Digital clock is made using 
    1) ATmega32
    2) BCD to 7 segment decoder(7448)
    3) SevenSegment display
    4) RTC (DS1307)
    
RTC is interfaced with Atmega32 using I2C(Inter Integrated Circuit) protocol.To convert the BCD value to 7 segment, decoder (7448) is used. Six single 7 segment is multiplexed to easily connect it with decoder IC (7488).Atmel studio is used for writing and compiling the code.          
