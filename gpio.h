#ifndef _GPIO_H
#define _GPIO_H

#define HIGH "1"
#define LOW	"0"

void gpioInit(int gpio);
void gpioWrite(int gpio,char *value);
void gpioCleanup(int gpio);

#endif /* _GPIO_H */