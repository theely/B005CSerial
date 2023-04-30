/********************************************************************************
* PIN MAPPING



B2  GPIO LED - GPIO_MODE_OUTPUT_PP GPIO_PULLDOWN GPIO_SPEED_HIGH 
C5  GPIO 24V Enable- GPIO_MODE_OUTPUT_PP GPIO_NOPULL GPIO_SPEED_HIGH
E14 GPIO Blade Motor nRESET - (HIGH for no RESET) GPIO_MODE_OUTPUT_PP GPIO_NOPULL GPIO_SPEED_HIGH
E15 GPIO Drive Motors - HC366 OE Pins (LOW to enable) GPIO_MODE_OUTPUT_PP GPIO_NOPULL GPIO_SPEED_HIGH
E8  TIM1_CH1N Charge Control Pins - HighSide/LowSide MosFET  
E9  TIM1_CH1 Charge Control Pins - HighSide/LowSide MosFET 
C0  GPIO Stop button - yellow (HIGH when pressed) GPIO_MODE_INPUT GPIO_PULLDOWN
TODO: CONFLICT UART6 - C8  GPIO Stop button - white (HIGH when pressed)  GPIO_MODE_INPUT GPIO_PULLDOWN
A8  GPIO Mechanical tilt - (HIGH when set) GPIO_MODE_INPUT GPIO_PULLDOWN
B3  SPI3 Flash CLK
B4  SPI3 Flash MISO
B5  SPI3 Flash MISI
A15 GPIO  Flash nCS  GPIO_MODE_OUTPUT_PP GPIO_PULLUP GPIO_SPEED_HIGH 
D0  GPIO Wheel lift blue GPIO_MODE_INPUT GPIO_PULLDOWN
D1  GPIO Wheel lift red  GPIO_MODE_INPUT GPIO_PULLDOWN
TODO: CONFLICT UART6 - C7  GPIO Play button - (LOW when pressed) GPIO_MODE_INPUT GPIO_PULLUP
B13 GPIO Home button - (LOW when pressed) GPIO_MODE_INPUT GPIO_PULLUP
E2  GPIO Rain Sensor - (LOW when active)  GPIO_MODE_INPUT GPIO_PULLUP
D2  GPIO STOP HALL Sensor - (HIGH when set) TODO: find config
D3  GPIO STOP HALL Sensor - (HIGH when set) TODO: find config
[not used] A10 UART1 RX - Master UART Port J16
[not used] A9  UART1 TX - Master UART Port J16
TODO: NOT FOUND C11 UART4 RX (DMA) - Master UART Red Port J18 (DMA)
TODO: NOT FOUND C10 UART4 TX (DMA) - Master UART Red Port J18
D6  UART2 RX (DMA) - Driver motros 
D5  UART2 TX (DMA) - Driver motros 
TODO: Check new port C7 or A12  - B11 UART6 RX (DMA) - Blade motoror 
TODO: Check new port C6 or A11 - B10 UART6 TX (DMA) - Blade motor
A12 USB_DP
A11 USB_DM
A10 UART1 RX (DMA) - Panle UART Port J16
A9  UART1 TX (DMA) - Panle UART Port J16
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