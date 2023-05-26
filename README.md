# Project Stauts

*Functional* - Most of the mower functionalies are implemented and have been tests.

## ToDo

- Implement perimeter wire sensing
- Extend status message to include perimeter wire data
- Test perimeter wire
- Implement panel UART and data transfer
- Extend status message to include top pannel data
- Test pannel integration


# Serial Protocol
To control the mower a text based USB Serial protocol is used.
Simply connect to the mower with a USB data cable and open a USB serial connection with a baud rate of 9600. 

## Status Output
Beside some log information mainly at boot time the B005CSerial sends a periodic status update. The status update is a JSON formatted message:
```javascript
{  
  'state':0,
  'emergency':0,
  'blade_state':0,
  'blade_temp': 21.20,
  'rain':0,
  'home':0,
  'play':0,
  'battery_v': 26.18,
  'charger_state:' 0,
  'charger_v': 30.98,
  'charger_a': 1.02,
 }
```

## Commands

| Command  | Parameter Type  | Description| Example   |
|---|---|---|---|
| arm  |    | Needs to be issued to enable any motor |  arm |
| speed:left_speed right_speed  | float (-0.5 to 0.5)   | Controls the speed of the left and right wheel  |  speed:0.3 0.3 |
| cut:on_off  |  boolean  | Controls the cutting blade  |  cut:1 |
| charger:on_off  |  boolean  | Controls the charger  |  charger:1 |
| halt  |    | Emergency Stop  |  halt |
| ack  |    | Needs to be sent every 500ms to avoid triggering the emergency timeout  |  ack |
# PIN MAPPING


| PIN  | Peripheral  | Type | Config   |
|---|---|---|---|
| A8 (TODO: not working)  |  Mechanical tilt  | GPIO  |  (HIGH when set) GPIO_MODE_INPUT GPIO_PULLDOWN |
| B8  (TODO: test)| Perimeter Sense Control  | GPIO  |  GPIO_MODE_OUTPUT_PP |
| B9  (TODO: test)| Perimeter Sense Control  | GPIO  | GPIO_MODE_OUTPUT_PP  |
| A10 (TODO: test) | Panle UART Port J16  |  UART1 RX (DMA) |   |
| A9  (TODO: test)| Panle UART Port J16  | UART1 TX (DMA)  |   |
| B1 | Buzzer   | PWM  | TIM4_CH3  |
| C9 | Play button  | GPIO  |  (LOW when pressed) GPIO_MODE_INPUT GPIO_PULLUP |
|  E8 | Charge Control Pins  |  TIM1_CH1N | HighSide/LowSide MosFET   |
|  E9 | Charge Control Pins  | TIM1_CH1   | HighSide/LowSide MosFET  |
| B13  | Home button  | GPIO  | (LOW when pressed) GPIO_MODE_INPUT GPIO_PULLUP  |
|  E2 |   Rain Sensor | GPIO  | (LOW when active)  GPIO_MODE_INPUT GPIO_PULLUP  |
|  C0 | Stop button  | GPIO  | yellow (HIGH when pressed) GPIO_MODE_INPUT GPIO_PULLDOWN  |
|  C8 | Stop button  | GPIO  | white (HIGH when pressed)  GPIO_MODE_INPUT GPIO_PULLDOWN  |
|  B2 | LED  | GPIO  | GPIO_MODE_OUTPUT_PP GPIO_PULLDOWN GPIO_SPEED_HIGH  |
|  C5 |  24V Enable | GPIO  | GPIO_MODE_OUTPUT_PP GPIO_NOPULL GPIO_SPEED_HIGH  |
|  E14 | Blade Motor nRESET  | GPIO  | (HIGH for no RESET) GPIO_MODE_OUTPUT_PP GPIO_NOPULL GPIO_SPEED_HIGH  |
|  E15 | Drive Motors  | GPIO  | HC366 OE Pins (LOW to enable) GPIO_MODE_OUTPUT_PP GPIO_NOPULL GPIO_SPEED_HIGH  |
| D0  | Wheel lift blue  | GPIO  | GPIO_MODE_INPUT GPIO_PULLDOWN  |
| D1  | Wheel lift red  | GPIO  | GPIO_MODE_INPUT GPIO_PULLDOWN  |
| D6  | Driver motors  |  UART2 RX (DMA)  |   |
| D5  | Driver motors  |  UART2 TX (DMA)  |   |
| C7  | Blade motors  | UART6 RX (DMA)  |   |
| C6   | Blade motor  |  UART6 TX (DMA) |   |
| A12  | USB Serial  | USB_DP  |   |
|  A11 | USB Serial  | USB_DM  |   |
| A1  | Charge Current  | ADC1  |   |
|  A2 | Charge Voltage  | ADC1  |   |
| A3  | Battery Voltage  | ADC1  |   |
|  A7 | Charger Input Voltage  | ADC1  |   |
|  C2 | Blade NTC  |  ADC1 |   |
|  A6 | Perimeter  | ADC1 (DMA)  |   |
| H0  | Crystal 8Mhz  | RCC HSE   |   |
| H1  | Crystal 8Mhz  | RCC HSE  |   |
| A10 [not used] | Master UART Port J16  | UART1 RX  |   |
| A9  [not used]|  Master UART Port J16 | UART1 TX  |   |
| B3 [not used]  |  SOFT I2C | GPIO  |   |
| B4 [not used]  |  SOFT I2C |  GPIO |   |
|  D2 [not used]|  STOP HALL Sensor  | GPIO  | (HIGH when set) TODO: find config  |
| D3  [not used]|  STOP HALL Sensor |  GPIO |  (HIGH when set) TODO: find config |
|  B3 [not used]| Flash CLK  | SPI3  |   |
|  B4 [not used]| Flash MISO  |  SPI3 |   |
| B5  [not used]|  SPI3 Flash MISI |  SPI3 |   |
| A15 [not used]|  Flash nCS |  GPIO | GPIO_MODE_OUTPUT_PP GPIO_PULLUP GPIO_SPEED_HIGH  |
| C11 [not used]| Master UART Red Port J18   | UART4 RX (DMA)  |   |
|  C10 [not used]| UMaster UART Red Port J18  | UART4 TX (DMA)  |   |
