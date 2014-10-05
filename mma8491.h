#ifndef _MMA8491_H
#define _MMA8491_H



typedef struct
{
  int16_t x;
  int16_t y;
  int16_t z;
} mma8491Data;


uint8_t init8491(int i2cNum,int gpioNum);
uint8_t mma8491Read(mma8491Data *pdata);
void mma8491Disable();
void mma8491Enable();
void mma8491CleanUp();


#endif /* _MMA8491_H */
