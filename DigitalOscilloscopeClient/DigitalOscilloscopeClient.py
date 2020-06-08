from SpiHandler import *
from I2cHandler import *
from Window import *
#Spi_Init(0,1,50000) #init with spi0 (this is available on Rpi), chip select, 500kpbs
#Spi_Send([12]) #test send
#msg = Spi_Read(1) #sends 0, but stores the receive

w = WindowApp() #TODO: Handle high CPU consumption if stop button is not clicked.
i2c = I2cClass()
#I2cHandlerInit()
for i in range(100):
    w.add_y_val(i2c.I2cRead())
    time.sleep(0.5)

print("Finished DO_Client")