# Hardware Setup For EEPROM Programmer

## Components List
- Arduino Mega Board (Recomended, otherwise you need to modify the input/output pin definitions of the code) 
- 28C16 EEPROM
- Jumper Wires
- ZIF Connector

## Wirering
- Use 28C16 EEPROM datasheet to find the pin configuration
- Connect data pins (I/O 0-7) in EEPROM with pin number 22, 24, 26, 28, ..., 36 pins in Arduino Mega
- Connect address pins (A0-A10) in EEPROM with pin number 2, 3, 4, ..., 12 pins in Arduino Mega
- Connect WE pin in EEPROM with the pin number 13 pin in Arduino Mega
- To writing purposes connect CO pin in EEPROM to a 5V pin in Arduino Mega
- To reading purposes connect CO pin in EEPROM to a GND pin in Arduino Mega
- Connect VCC and VSS pins in EERPOM with 5V and GND pins in Arduino Mega
- Connect CE pin in EEPROM to a GND pin in Arduino Mega

## Note that
<b>All Arduino codes are working with the same hardware setup</b>
