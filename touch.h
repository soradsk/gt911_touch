#ifndef __touch_H
#define __touch_H

#define TOUCH_START 2
#define TOUCH_MOVE 4
#define TOUCH_END 8

#define TOUCH_TIME_CLICK 0
#define TOUCH_TIME_LONG_CLICK 10

#define TOUCH_TYPE_CLICK 4
#define TOUCH_TYPE_LONG_CLICK 5


#include "config.h"
typedef struct Coordinate_Touch
{
	int x;
	int y;
	unsigned short count;
	short signal;
	char type;
	short used;
	int v; //velocity
	int start_x;
	int start_y;
}coordinate_touch;

unsigned char IIC_RcvByte();
void  IIC_SendByte(unsigned char byte);
void IIC_Stop();
void IIC_Start();
unsigned int IIC_wait_ACK();
void IIC_NACK();
void IIC_ACK();
void touch_run();
char* get_touch_data();


void touch_init();
void touch_clean();
void touch_init_clean();
coordinate_touch * get_coordinate();
void coordinate_clean();
#endif