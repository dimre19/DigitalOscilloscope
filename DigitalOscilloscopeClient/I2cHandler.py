# I2cHandler.py
# Handle I2C Communication

import smbus
import time

# I2C channel 1 is connected to the GPIO pins
channel = 1
#  MCP4725 defaults to address 0x60
address = 0x7F

# Register addresses (with "normal mode" power-down bits)
reg_write_dac = 0x40

# Initialize I2C (SMBus)
bus = smbus.SMBus(channel)

#msg
msg = [0xAB]

# Write out I2C command: address, reg_write_dac, msg[0], msg[1]
#bus.write_i2c_block_data(address, reg_write_dac, msg)
for i in range(15):
    tempValue = bus.read_i2c_block_data(address,0x40,1)
    time.sleep(1) 
    print(tempValue)

