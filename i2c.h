#ifndef _I2C_H
#define _I2C_H

int i2cOpen(int i2cNum);
void i2cCleanUp(int i2cDev);
int i2cWrite(unsigned char address, unsigned char reg, unsigned char data);
int i2cRead(unsigned char address, unsigned char reg, unsigned char *data);

#endif /* _I2C_H */

