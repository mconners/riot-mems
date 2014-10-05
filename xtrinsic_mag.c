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
    // unsigned char data;
    // int temperatureData;
    // int temperatureFinal;
    // int DBG_ENABLE = 0;
    mma8491Data mmaData;
    mag3110Data magData;

    signal(SIGINT,sigHandler);


    if (!init3110(2) || !init8491(2, 135))
    {
        printf("%s\n", "Device initialization failed.");
        exit(1);
    }

    printf("%s\n", "Devices sucessfully initialized");

    while (cont)
    {
        // int16_t x = MAG3110_ReadRawData_x();
        // int16_t y = MAG3110_ReadRawData_y();
        mag3110Read(&magData);
        printf("3110 X Val Read = %d\n", magData.x);
        printf("3110 Y Val Read = %d\n", magData.y);
        printf("3110 Z Val Read = %d\n", magData.z);
        printf("Direction is %g\n", getDirection((double)magData.x, (double)magData.y));
        mma8491Read(&mmaData);
        printf("8491 X Val Read = %d\n", mmaData.x);
        printf("8491 Y Val Read = %d\n", mmaData.y);
        printf("8491 Z Val Read = %d\n", mmaData.z);

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
