# SpiHandler.py
# Spi handling module for Digital Oscilloscope (from STM32F4Discovery board)

import time
import spidev

#TODO class
spi = 0

def Spi_Init(bus,dev,speed):
    global spi
    _bus = bus
    _device = dev
    spi = spidev.SpiDev()
    spi.open(_bus,_device)
    #TODO: check speed credibility
    spi.max_speed_hz = speed #500kbps. Actually it will be 488kHz, see general documentation of RPi SPI
    spi.mode = 0
    
def Spi_Send(msg):
    global spi    
    spi.xfer2(msg)

def Spi_Read(n):
    msg = spi.readbytes(n)
    return msg
    
def Spi_Close():
    spi.close()