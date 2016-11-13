# Mapped Pins

NRF24 Pins

![NRF24 Pins](http://mco-s1-p.mlstatic.com/modulo-transceptor-rf-nrf24l01-arduino-pic-avr-24-ghz-1371-MCO3746363892_012013-O.jpg "NRF24 Pins")


Raspberry Pi GPio

![Raspberry Pi GPio](https://cloud.githubusercontent.com/assets/4025771/19619460/7d4da916-9868-11e6-93cf-139f01b733ad.png "Raspberry GPio")

Raspberry Pi Pyshical 

![Raspberry Pi Pyshical](https://cloud.githubusercontent.com/assets/4025771/19619194/53eb3874-9861-11e6-9474-4e80a61b653c.png "Rasperry")

Rasperry Pi NRF24 Connections

NRF24-1
```
PIN     NRF24L01    RPI             RPi -P1 Connector
1       GND         rpi-gnd         (25)
2       VCC         rpi-3v3         (17)
3       CE          rpi-gpio22      (15)****
4       CSN         rpi-gpio8       (24)
5       SCK         rpi-sckl        (23)
6       MOSI        rpi-mosi        (19)
7       MISO        rpi-miso        (21)
8       IRQ         Not Connected
```
NRF24-2
```
PIN     NRF24L01    RPI             RPi -P1 Connector
1       GND         rpi-gnd         (39)
2       VCC         rpi-3v3         (1)
3       CE          rpi-gpio26      (37)****
4       CSN         rpi-gpio19      (35)
5       SCK         rpi-sckl        (33)
6       MOSI        rpi-mosi        (31)
7       MISO        rpi-miso        (29)
8       IRQ         Not Connected
```

Arduino NRF24 Connections
```
PIN     NRF24L01                 Arduino -P1 Connector
1       GND                       (GND)
2       VCC                       (3v3)
3       CE                         (D7)****
4       CSN                        (D8)
5       SCK                       (D13)
6       MOSI                      (D11)
7       MISO                      (D12)
8       IRQ                      Not Connected
```
