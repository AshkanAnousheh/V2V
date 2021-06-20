#ifndef  ACCEL_H
#define  ACCEL_H


#define		DRG2RAD	0.01745329252
//#define		LED3		P1_2


#include        "types.h"
#include        <stdio.h>
#include        "delay_8051.h"

#include        "i2c_new.h"
#include	<string.h>


#include	<math.h>

#include 	"MPU6050.h"


#define dt              0.2 // sampling period in seconds (shown as 1 ms)
#define	nOfSample	50
#define	beta	        0.68
#define	alpha	        0.4
#define	gravity	        9.8

extern  float linearAccY;
extern  bool  acce_data_ready;
extern  int accelInit(void);
extern  void calculateAccel();

#endif
