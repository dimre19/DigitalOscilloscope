from SpiHandler import *

Spi_Init(0,1,50000) #init with spi0 (this is available on Rpi), chip select, 500kpbs
Spi_Send([12]) #test send
#msg = Spi_Read(1) #sends 0, but stores the receive

print("Finished DO_Client")