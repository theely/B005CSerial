/********************************************************************************
* PIN MAPPING


| PIN  | Peripheral  | Type | Config   |
|---|---|---|---|
|  B2 | LED  | GPIO  | GPIO_MODE_OUTPUT_PP GPIO_PULLDOWN GPIO_SPEED_HIGH  |
| D14  | Buzzer   | PWM  |   |
|  C5 |  24V Enable | GPIO  | GPIO_MODE_OUTPUT_PP GPIO_NOPULL GPIO_SPEED_HIGH  |
|  E14 | Blade Motor nRESET  | GPIO  | (HIGH for no RESET) GPIO_MODE_OUTPUT_PP GPIO_NOPULL GPIO_SPEED_HIGH  |
|  E15 | Drive Motors  | GPIO  | HC366 OE Pins (LOW to enable) GPIO_MODE_OUTPUT_PP GPIO_NOPULL GPIO_SPEED_HIGH  |
|  E8 | Charge Control Pins  |  TIM1_CH1N | HighSide/LowSide MosFET   |
|  E9 | Charge Control Pins  | TIM1_CH1   | HighSide/LowSide MosFET  |
|  C0 | Stop button  | GPIO  | yellow (HIGH when pressed) GPIO_MODE_INPUT GPIO_PULLDOWN  |
|  C8 (TODO: CONFLICT UART6) | Stop button  | GPIO  | white (HIGH when pressed)  GPIO_MODE_INPUT GPIO_PULLDOWN  |
| A8  |  Mechanical tilt  | GPIO  |  (HIGH when set) GPIO_MODE_INPUT GPIO_PULLDOWN |
|  B3 | Flash CLK  | SPI3  |   |
|  B4 | Flash MISO  |  SPI3 |   |
| B5  |  SPI3 Flash MISI |  SPI3 |   |
| A15  |  Flash nCS |  GPIO | GPIO_MODE_OUTPUT_PP GPIO_PULLUP GPIO_SPEED_HIGH  |
| D0  | Wheel lift blue  | GPIO  | GPIO_MODE_INPUT GPIO_PULLDOWN  |
| D1  | Wheel lift red  | GPIO  | GPIO_MODE_INPUT GPIO_PULLDOWN  |
| C7 (TODO: CONFLICT UART6)   | Play button  | GPIO  |  (LOW when pressed) GPIO_MODE_INPUT GPIO_PULLUP |
| B13  | Home button  | GPIO  | (LOW when pressed) GPIO_MODE_INPUT GPIO_PULLUP  |
|  E2 |   Rain Sensor | GPIO  | (LOW when active)  GPIO_MODE_INPUT GPIO_PULLUP  |
|  D2 |  STOP HALL Sensor  | GPIO  | (HIGH when set) TODO: find config  |
| D3  |  STOP HALL Sensor |  GPIO |  (HIGH when set) TODO: find config |
| A10 [not used] | Master UART Port J16  | UART1 RX  |   |
| A9  [not used]|  Master UART Port J16 | UART1 TX  |   |
| C11 (TODO: NOT FOUND)  | Master UART Red Port J18   | UART4 RX (DMA)  |   |
|  C10 (TODO: NOT FOUND) | UMaster UART Red Port J18  | UART4 TX (DMA)  |   |
| D6  | Driver motros  |  UART2 RX (DMA)  |   |
| D5  | Driver motros  |  UART2 TX (DMA)  |   |
| C7 or A12 (TODO: Check new port) | Blade motoror  | UART6 RX (DMA)  |   |
| C6 or A11 (TODO: Check new port)  | Blade motor  |  UART6 TX (DMA) |   |
| A12  | USB Serial  | USB_DP  |   |
|  A11 | USB Serial  | USB_DM  |   |
| A10  | Panle UART Port J16  |  UART1 RX (DMA) |   |
| A9  | Panle UART Port J16  | UART1 TX (DMA)  |   |
|   |   |   |   |
|   |   |   |   |
|   |   |   |   |
|   |   |   |   |
|   |   |   |   |
|   |   |   |   |
|   |   |   |   |
 
 
  - 
   - 
[not used] B3  GPIO SOFT I2C
[not used] B4  GPIO SOFT I2C
A1  ADC1 - Charge Current
A2  ADC1 - Charge Voltage
A3  ADC1 - Battery Voltage
A7  ADC1 - Charger Voltage
C2  ADC1 - Blade NTC
A6  ADC1 (DMA) - Perimeter
H0  RCC HSE - External crystal 8Mhz
H1  RCC HSE - External crystal 8Mhz
/********************************************************************************
