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
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <signal.h>

#include "i2c.h"
#include "mag3110.h"
#include "mma8491.h"
#include "mpl3115.h"

// Talks to a Freescale Xtrinsic Sensor board
// Connected in the following manner
// Xtrinsic Sensor  | RIoT Board
// CN1 - 1 (EN)     | Pin 21 GPIO5_07
// CN1 - 9 (SDA)    | Pin 37
// CN1 - 10 (SCL)   | Pin 35
// CN2 - 1 (Vdd)    | Pin 1
// CN2 - 3 (GND)    | Pin 3


void sigHandler(int nothing);
static volatile int cont = 1;

int main()
{
    mma8491Data mmaData;
    mag3110Data magData;

    signal(SIGINT,sigHandler);

    if (!init3110(2) || !init8491(2, 135) || !init3115(2))
    {
        printf("%s\n", "Device initialization failed.");
        exit(1);
    }

    printf("%s\n", "Devices sucessfully initialized");

    while (cont)
    {
        mag3110Read(&magData);
        printf("3110 X Val Read = %d\n", magData.x);
        printf("3110 Y Val Read = %d\n", magData.y);
        printf("3110 Z Val Read = %d\n", magData.z);
        printf("Direction is %g\n", getDirection((double)magData.x, (double)magData.y));
        
        MAG3110Read(&magData);
        printf("3110 - X Val Read = %d\n", magData.x);
        printf("3110 - Y Val Read = %d\n", magData.y);
        printf("3110 - Z Val Read = %d\n", magData.z);
        printf("Direction is %g\n", getDirection((double)magData.x, (double)magData.y));
        
        mma8491Read(&mmaData);
        printf("8491 X Val Read = %d\n", mmaData.x);
        printf("8491 Y Val Read = %d\n", mmaData.y);
        printf("8491 Z Val Read = %d\n", mmaData.z);

        printf("3115 Temp Read = %d\n", mpl3115ReadTemp());
        printf("3115 Temp Read F = %d\n", mpl3115ReadTempF());

        sleep(1);
    }

    mma8491CleanUp();
    mag3110CleanUp();

    return 0;
}

void sigHandler(int nothing)
{
    cont = 0;
}
