// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// AT30TS74
// This code is designed to work with the AT30TS74_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/products

import com.pi4j.io.i2c.I2CBus;
import com.pi4j.io.i2c.I2CDevice;
import com.pi4j.io.i2c.I2CFactory;
import java.io.IOException;

public class AT30TS74
{
	public static void main(String args[]) throws Exception
	{
		// Create I2CBus
		I2CBus bus = I2CFactory.getInstance(I2CBus.BUS_1);
		// Get I2C device, AT30TS74 I2C address is 0x48(72)
		I2CDevice device = bus.getDevice(0x48);

		byte[] config = new byte[2];
		// Set resolution = 12-bits, normal operations, comparator mode
		config[0] = 0x60;
		// Reserved, default to 0x00
		config[1] = 0x00;

		// Select configuration register 0x01(01)
		// config msb, config lsb
		device.write(0x01, config, 0, 2);
		Thread.sleep(500);

		// Read 2 bytes of data
		// temp msb, temp lsb
		byte[] data = new byte[2];
		device.read(0x00, data, 0, 2);

		// Convert the data to 12 bits
		int temp = (((data[0] & 0xFF) * 256) + (data[1] & 0xF0)) / 16;
		if(temp > 2047)
		{
			temp -= 4096;
		}
		double cTemp = temp * 0.0625;
		double fTemp = cTemp * 1.8 + 32;

		// Output data to screen
		System.out.printf("Temperature in Celsius : %.2f C %n", cTemp);
		System.out.printf("Temperature in Farhenheit : %.2f F %n", fTemp);
	}
}