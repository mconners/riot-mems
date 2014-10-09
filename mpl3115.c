// Copyright (c) 2014 Michael Conners
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:

// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include <stdint.h>

#include "i2c.h"
#include "mpl3115.h"


#define MPL3115_I2C_ID 0x60
#define MPL_3115_WHO_AM_I_REG 0x0C
#define MPL_3115_WHO_AM_I 0xC4

#define MPL3115A2_CTRL_REG1 0x26
#define MPL3115A2_CTRL_REG2 0x27
#define MPL3115_TEMP_MSB_REG 0x04
#define MPL3115_TEMP_LSB_REG 0x05
#define ACTIVE_MASK 0x01


static int i2cDev = -1;

int init3115(int i2cNum)
{
    unsigned char data;

    i2cDev = i2cOpen(i2cNum);

    if (i2cDev < 0)
        return 0;

    i2cWrite(MPL3115_I2C_ID, MPL3115A2_CTRL_REG1, 0);

    i2cRead(MPL3115_I2C_ID, MPL_3115_WHO_AM_I_REG, &data);

    if (data == MPL_3115_WHO_AM_I)
    {
    	i2cRead(MPL3115_I2C_ID, MPL3115A2_CTRL_REG1, &data);
        i2cWrite(MPL3115_I2C_ID, MPL3115A2_CTRL_REG1, ACTIVE_MASK);
        return 1;
    }
    return 0;
}

uint16_t mpl3115ReadTemp()
{
	unsigned char data = 0;
    uint16_t val = 0;

	i2cRead(MPL3115_I2C_ID,MPL3115_TEMP_MSB_REG,&data);
	val = data;
	val <<= 8;
	i2cRead(MPL3115_I2C_ID,MPL3115_TEMP_LSB_REG,&data);
	val += data;

	return val >> 8 & 0xff;
}

uint16_t mpl3115ReadTempF()
{
	return mpl3115ReadTemp() * 9 / 5 + 32;
}

void mpl3115CleanUp()
{
    i2cCleanUp(i2cDev);
}
