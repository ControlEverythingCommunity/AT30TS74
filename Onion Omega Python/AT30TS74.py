# Distributed with a free-will license.
# Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
# AT30TS74
# This code is designed to work with the AT30TS74_I2CS I2C Mini Module available from ControlEverything.com.
# https://www.controleverything.com/products

from OmegaExpansion import onionI2C
import time

# Get I2C bus
i2c = onionI2C.OnionI2C()

# AT30TS74 address, 0x48(72)
# Select configuration register, 0x01(01)
#		0x60(96)	Set resolution = 12-bits, Normal operations, Comparator mode
#		0x00(00)	Reserved, default to 0x00
config = [0x60, 0x00]
i2c.writeBytes(0x48, 0x01, config)

time.sleep(0.5)

# AT30TS74 address, 0x48(72)
# Read data back from 0x00(00), 2 bytes
# temp MSB, temp LSB
data = i2c.readBytes(0x48, 0x00, 2)

# Convert the data to 12-bits
temp = ((data[0] * 256) + (data[1] & 0xF0)) / 16
if temp > 2047 :
	temp -= 4096
cTemp = temp * 0.0625
fTemp = (cTemp * 1.8) + 32

# Output data to screen
print "Temperature in Celsius : %.2f C" %cTemp
print "Temperature in Fahrenheit : %.2f F" %fTemp
