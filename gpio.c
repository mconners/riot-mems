#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

void gpioInit(int gpio)
{
    int fd;
    char buf[255];

    fd = open("/sys/class/gpio/export", O_WRONLY);
    sprintf(buf, "%d", gpio);
    write(fd, buf, 3);
    close(fd);

    sprintf(buf, "/sys/class/gpio/gpio%d/direction", gpio);
    fd = open(buf, O_WRONLY);
    // Set out direction
    write(fd, "out", 3);
    close(fd);
}

void gpioWrite(int gpio, char *value)
{
    char buf[255];
    int fd;
    sprintf(buf, "/sys/class/gpio/gpio%d/value", gpio);
    fd = open(buf, O_WRONLY);

    // Set GPIO value "0" = low "1" = high
    write(fd, value, 1);
    close(fd);
}

void gpioCleanup(int gpio)
{
    int fd;
    char buf[255];

    fd = open("/sys/class/gpio/unexport", O_WRONLY);

    sprintf(buf, "%d", gpio);

    write(fd, buf, 3);

    close(fd);
}

// Reserve (export) the GPIO:
// int fd;
// char buf[MAX_BUF];
// int gpio = XX;

// fd = open("/sys/class/gpio/export", O_WRONLY);

// sprintf(buf, "%d", gpio);

// write(fd, buf, strlen(buf));

// close(fd);

// Set the direction in the GPIO folder just created:
// sprintf(buf, "/sys/class/gpio/gpio%d/direction", gpio);

// fd = open(buf, O_WRONLY);

// // Set out direction
// write(fd, "out", 3);
// // Set in direction
// write(fd, "in", 2);

//close(fd);

// In case of out direction set the value of GPIO:
// sprintf(buf, "/sys/class/gpio/gpio%d/value", gpio);

// fd = open(buf, O_WRONLY);

// // Set GPIO high status
// write(fd, "1", 1);
// // Set GPIO low status
// write(fd, "0", 1);

// close(fd);

// In case of in direction get the current value of GPIO:
// char value;

// sprintf(buf, "/sys/class/gpio/gpio%d/value", gpio);

// fd = open(buf, O_RDONLY);

// read(fd, &value, 1);

// if(value == '0')
// {
//      // Current GPIO status low
// }
// else
// {
//      // Current GPIO status high
// }

// close(fd);

// Once finished free (unexport) the GPIO:
// fd = open("/sys/class/gpio/unexport", O_WRONLY);

// sprintf(buf, "%d", gpio);

// write(fd, buf, strlen(buf));

// close(fd);
