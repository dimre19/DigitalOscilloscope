# I2cHandler.py
# Handle I2C Communication

import smbus
import time
import random

class I2cClass:
    def __init__(self):
        # I2C channel 1 is connected to the GPIO pins
        channel = 1
        #  MCP4725 defaults to address 0x60
        self.address = 0x7F

        # Register addresses (with "normal mode" power-down bits)
        self.eg_write_dac = 0x40

        # Initialize I2C (SMBus)
        self.bus = smbus.SMBus(channel)

        #msg
        msg = [0xAB]

        # Write out I2C command: address, reg_write_dac, msg[0], msg[1]
        #bus.write_i2c_block_data(address, reg_write_dac, msg)
    def I2cRead(self):   
        tempValue = self.bus.read_i2c_block_data(self.address,0x40,1) #TODO: read word (2 byte is fraction)
           
            
        #time.sleep(0.5) 
        print(tempValue)
        return tempValue

