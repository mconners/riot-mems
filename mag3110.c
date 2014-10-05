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

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <unistd.h>

#include "mag3110.h"
#include "i2c.h"

#define MAG3110_I2C_ID 0x0E
#define WHO_AM_I 0xC4
#define WHO_AM_I_REG 0x07
#define CTRL_REG_1 0x10
#define CTRL_REG_2 0x11
#define TEMPERATURE_MSB 0x04
#define TEMPERATURE_LSB 0x05
#define X_OUT_MSB 0x01
#define X_OUT_LSB 0x02
#define STATUS_REG 0x00
#define X_DATA_RDY (0x01 << 0)
#define Y_OUT_MSB 0x03
#define Y_OUT_LSB 0x04
#define Y_DATA_RDY  (0x01 << 1)
#define Z_OUT_MSB 0x05
#define Z_OUT_LSB 0x06
#define Z_DATA_RDY  (0x01 << 2)
#define XYZ_DATA_RDY    (0x01 << 3)
#define ALL_DATA_RDY    (0x07)

static int i2cDev = -1;
int16_t MAG3110_ReadRawData(unsigned char msbReg, unsigned char lsbReg, unsigned char statusFlag);

int init3110(int i2cNum)
{
    unsigned char data;

    i2cDev = i2cOpen(i2cNum);

    if (i2cDev < 0)
        return 0;

    write_register(MAG3110_I2C_ID, CTRL_REG_1, 0);

    read_register(MAG3110_I2C_ID, WHO_AM_I_REG, &data);

    if (data == WHO_AM_I)
    {
        write_register(MAG3110_I2C_ID, CTRL_REG_2, 0x80);
        return 1;
    }
    return 0;
}

int16_t MAG3110_ReadRawData_x()
{
    return MAG3110_ReadRawData(X_OUT_MSB, X_OUT_LSB, X_DATA_RDY);
}

int16_t MAG3110_ReadRawData_y()
{
    return MAG3110_ReadRawData(Y_OUT_MSB, Y_OUT_LSB, Y_DATA_RDY);
}

int16_t MAG3110_ReadRawData_z()
{
    return MAG3110_ReadRawData(Z_OUT_MSB, Z_OUT_LSB, Z_DATA_RDY);
}

int16_t MAG3110_ReadRawData(unsigned char msbReg, unsigned char lsbReg, unsigned char statusFlag)
{
    unsigned char data = 0;
    int16_t val = 0;

    read_register(MAG3110_I2C_ID, msbReg, &data);
    write_register(MAG3110_I2C_ID, CTRL_REG_2, 0x80);
    write_register(MAG3110_I2C_ID, CTRL_REG_1, 0x02);

    do
    {
        read_register(MAG3110_I2C_ID, STATUS_REG, &data);
    }
    while ((data & statusFlag) != statusFlag);

    read_register(MAG3110_I2C_ID, msbReg, &data);
    val = data;
    val <<= 8;
    read_register(MAG3110_I2C_ID, lsbReg, &data);
    val += data;

    return val;
}

uint8_t mag3110ReadRaw(uint8_t *data)
{
    unsigned char status = 0;
    // Wait til data is ready

    write_register(MAG3110_I2C_ID, CTRL_REG_2, 0x80);
    write_register(MAG3110_I2C_ID, CTRL_REG_1, 0x02);
    do
    {
        //printf("Calling 3100 status register\n");
        read_register(MAG3110_I2C_ID, STATUS_REG, &status);
        //sleep(1);
    }
    while (((status & ALL_DATA_RDY) != ALL_DATA_RDY));

    // do bulk read
    int ii;
    for (ii = 0; ii < 6; ii++)
    {
        read_register(MAG3110_I2C_ID, X_OUT_MSB + ii, &data[ii]);
        //printf("Reading register 0x%x + %d\n",X_OUT_MSB,ii);
    }

    return 1;
}

uint8_t MAG3110Read(mag3110Data *data)
{
    data->x = MAG3110_ReadRawData_x();
    data->y = MAG3110_ReadRawData_y();
    data->z = MAG3110_ReadRawData_z();
    return 1;
}

uint8_t mag3110Read(mag3110Data *data)
{
    uint8_t rawData[8] = {0};
    if (mag3110ReadRaw(rawData))
    {
        data->x = rawData[0];
        data->x <<= 8;
        data->x += rawData[1];

        data->y = rawData[2];
        data->y <<= 8;
        data->y += rawData[3];

        data->z = rawData[4];
        data->z <<= 8;
        data->z += rawData[5];
    }
    return 1;
}

double getDirection(double x, double y)
{
    //if (y > 0)
        //return 90 - atan(x / y) * 180 / M_PI;
    //if (y < 0)
        //return 270 - atan(x / y) * 180 / M_PI;
    //if (x > 0)
        //return 180.0;
    //return 0.0;

     float heading = atan2(-y,x);
     if(heading < 0)
         heading += 2 * M_PI;

     return heading * (180/M_PI);
}

void mag3110CleanUp()
{
    i2cCleanUp(i2cDev);
}
