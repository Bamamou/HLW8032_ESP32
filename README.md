# HLW8032_ESP32
This is a simple library for the HLW8032 with ESP32 as the MCU using Platfprmio and the Arduino framework <br> Note: most of the test was done with 220VAC 50Hz
## Pin layout

If you have an ESP32, a HLW8032 module and an I2C LCD, the below connection should work wwll

| ESP32 | HLW8032 | LCD |
| ------------- | ------------- | -------------|
| 5V         |  VCC   | VCC |
| GND        |  GND   | GND |
| GPIO16      |   RX   | NC |
| SDA (GPIO21)|   NC   | SDA |
| SCL(GPIO22)|   NC   | SCL |

## Module drawing 
The below schematic denotes how the single module is designed with these three pins as low voltage connections
1.  VCC
2.  GND
3.  TX
And 4 other pins for input and output of the 220VAC side
![image](https://github.com/user-attachments/assets/edfc32a6-d68c-487d-b9fb-2e3638d38436)
