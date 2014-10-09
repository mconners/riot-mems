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

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include "mma8491.h"
#include "i2c.h"
#include "gpio.h"


//sudo ./devmem2 0x20e01a4 w 0x05 to set gpio5_io07 to pin mux 0x05
// (IOMUXC_SW_MUX_CTL_PAD_DISP0_DATA13) pg2063 sec 36.4.101

#define MMA8491_I2C_ID          0x55
#define MMA8491Q_STATUS         0x00
#define ZYXDR_BIT               (1<<3)
#define MMA8491Q_OUT_X_MSB      0x01

static int i2cDev = -1;
static int gpio = -1;


uint8_t init8491(int i2cNum, int gpioNum)
{
 

    gpio = gpioNum;
    //int gpio = 112;

    i2cDev = i2cOpen(i2cNum);

    if (i2cDev < 0)
        return 0;

    gpioInit(gpio);

    mma8491Disable();
    return 1;
}

uint8_t mma8491ReadRaw(uint8_t *data)
{
    unsigned char status = 0;
    mma8491Enable();
    // Wait til data is ready
    do
    {
        //printf("Calling 8491 status register\n");
        i2cRead(MMA8491_I2C_ID, MMA8491Q_STATUS, &status);
        //sleep(1);
    }
    while (((status & ZYXDR_BIT) != ZYXDR_BIT) || (status > 0x0f));

    // do bulk read
    int ii;
    for (ii = 0; ii < 6; ii++)
    {
        i2cRead(MMA8491_I2C_ID, MMA8491Q_OUT_X_MSB + ii, &data[ii]);
        //printf("Reading register 0x%x + %d\n",MMA8491Q_OUT_X_MSB,ii);
    }
    mma8491Disable();

    return 1;
}

void mma8491CleanUp()
{
    gpioCleanup(gpio);
    i2cCleanUp(i2cDev);
}


uint8_t mma8491Read(mma8491Data *data)
{
    uint8_t rawData[8] = {0};
    if (mma8491ReadRaw(rawData))
    {
        data->x = rawData[0] << 8;
        data->x <<= 8;
        data->x += rawData[1];
        data->x >>= 2;

        data->y = rawData[2];
        data->y <<= 8;
        data->y += rawData[3];
        data->y >>= 2;

        data->z = rawData[4];
        data->z <<= 8;
        data->z += rawData[5];
        data->z >>= 2;
    }
    return 1;
}

void mma8491Disable()
{
     gpioWrite(gpio,LOW);
}

void mma8491Enable()
{
    gpioWrite(gpio,HIGH);
}

