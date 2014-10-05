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
