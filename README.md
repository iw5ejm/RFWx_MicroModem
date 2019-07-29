# RFWx_MicroModem
APRS Weather packet sender for Simple Serial microAPRS modem by unsigned.io using bosh BME280 sensor

All infos about microAPRS by Mark, OZ7TMD here: https://unsigned.io/projects/microaprs/

This project communicate with microAPRS module to send weather aprs data packet via RF on 144.800 MHz AX25.
It initializes the modem (callsign and SSID) and sends temperature, humidity and pressure data every 10 minutes.
It uses the Simple Serial communication protocol by Mark.

I built the system with two arduino nano: one for the controller and one for the modem.
Mark realeased the code for the microAPRS modem, so you can either buy it or build it your self with an arduino nano AT328p.
See schematic in repository.

The two arduino are simply connected with a cross cable between the two phisical serial port: RX0 with TX1 and viceversa.

The controller has no need for real time clock: it uses Countdown library by Rob Tillaart.
Thanks to Marco IU5HJU for the BME280 related function.
