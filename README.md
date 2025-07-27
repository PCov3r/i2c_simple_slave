# I²C Register-Based Slave Device

A simple script to turn any microcontroller (e.g. Arduino, ATtinyx17) into an I²C slave device with readable and writable 4-byte registers.

## Features

- Exposes a bank of 256 registers, each 4 bytes wide (supports `float` values).
- Master can read or write to any register by specifying its address.
- I²C slave address is configurable via 2 GPIO pins — just connect them to GND to change the address (up to 4 unique addresses total like for seesaw devices).

## Use Cases

- Sharing sensor data (e.g., floats from analog reads) over I²C
- Controlling actuators or settings via register writes
- Using small MCUs as smart I²C peripherals

