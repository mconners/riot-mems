#ifndef _MAG3110_H
#define _MAG3110_H



typedef struct {
  int16_t x;
  int16_t y;
  int16_t z;
}mag3110Data;


int init3110(int i2cNum);
uint8_t mag3110Read(mag3110Data *data);
double getDirection(double x, double y);
void mag3110CleanUp();
#endif /* _MAG3110_H */
