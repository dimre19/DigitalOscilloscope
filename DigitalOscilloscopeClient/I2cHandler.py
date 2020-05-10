# i2ctest.py
# A brief demonstration of the Raspberry Pi I2C interface, using the Sparkfun
# Pi Wedge breakout board and a SparkFun MCP4725 breakout board:
# https://www.sparkfun.com/products/8736
'''
import smbus

# I2C channel 1 is connected to the GPIO pins
channel = 1

#  STM32F4 address
address = 0x45
reg_write_dac = 0x40 #Why??

# Initialize I2C (SMBus)
bus = smbus.SMBus(channel)
msg = [0xAB]
# Write out I2C command: address, reg_write_dac, msg[0], msg[1]
bus.write_i2c_block_data(address, reg_write_dac, msg)

print("I2C done")
'''
# i2ctest.py
# A brief demonstration of the Raspberry Pi I2C interface, using the Sparkfun
# Pi Wedge breakout board and a SparkFun MCP4725 breakout board:
# https://www.sparkfun.com/products/8736

import smbus

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
bus.read_i2c_block_data(address,0x40,1)

