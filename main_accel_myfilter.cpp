#include "main_accel_myfilter.h"
// mpu6050	cpp


int16_t	ax,ay,az;
int16_t	gx,gy,gz;
		

float	gOffsX=0,gOffsY=0,gOffsZ=0,aOffsX=0,aOffsY=0,aOffsZ=0;
float	LPX=0,LPY=0,LPZ=0;
char	flagCalibrate=0;
float	Ax,Ay,Az,Gx,Gy,Gz,accNetX,accNetY,accNetZ;		

float linearAccY;
bool  acce_data_ready;

void calculateAccel()
{
// timer1_enable();

	MPU6050         getMotion;

	getMotion.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
	Ax = (float)ax / 8192;
	Ay = (float)ay / 8192;
	Az = (float)az / 8192;
	
	
	Gx = (float)(((float)gx / 131));
	Gy = (float)(((float)gy / 131 ));
	Gz = (float)(((float)gz / 131));
	

	if(flagCalibrate<nOfSample)
	{
							gOffsX 	 =	(float)((float)gOffsX + (float)Gx/nOfSample);												// in mohasebato deghghaaaaaaaaaaaaaaaaaaatt kon ke
							gOffsY 	 =	(float)((float)gOffsY + (float)Gy/nOfSample);												// faqat Gx bayad be nOfSample	taghsiiiiiiimm besheeeeeee		
							gOffsZ 	 =	(float)((float)gOffsZ + (float)Gz/nOfSample);
							
							aOffsX	=	(float)(aOffsX + (float)Ax/nOfSample);
							aOffsY	=	(float)(aOffsY + (float)Ay/nOfSample);
							aOffsZ	=	(float)(aOffsZ + ((float)Az-0.98)/nOfSample);
							flagCalibrate++;
	}
	else{
							
							Ax	=	Ax-aOffsX;
							Ay	=	Ay-aOffsY;
							Az	=	Az-aOffsZ;
							
							Gx = 	round(Gx-gOffsX);
							Gy =   	round(Gy-gOffsY);
							Gz =	round(Gz-gOffsZ);
							
						
							Ax *= 10;
							Ay *= 10;
							Az *= 10;
							

				
				
							LPX = alpha * LPX + (1-alpha) * Ax;
							LPY = alpha * LPY + (1-alpha) * Ay;
							LPZ = alpha * LPZ + (1-alpha) * Az;
							
							
							accNetX = Ax - LPX;
							accNetY = Ay - LPY;
							accNetZ = Az - LPZ; 

		
	linearAccY = pow(abs(accNetY),6) /((1 + pow(Gy,2)+pow(abs(Gx),3)+pow(Gz,2))*(1+sqrt(pow(accNetX,2)+pow(accNetZ,2)))) ;
							

	 }
        //LED3=1;
        
	//timer1_disbale();
	//unsigned int time = (T1CNTH << 8)|T1CNTL;
	
		//sprintf(USB_buff,"%.2f\n\r",linearAccY);
		//usb_send(USB_buff);
		
	acce_data_ready = false;
} 


#pragma vector = P0INT_VECTOR
__interrupt void P0INT_ISR(void){
	acce_data_ready = true;	
	P0IFG = ~(1<<4); 									// pak kardane flag interrupt peripheral
	IRCON &= ~(1<<5);								 	// pak kardne flasg az baxshe interrupt
}


int accelInit( void )
{
	//LED3 = 0;
    // GlobalVar Init  
    linearAccY=0;
	acce_data_ready = false;
    //-------------------
	// Interrupt Init
	PICTL = (1<<0); //  hasas be pain rawande kardan
	P0IFG = ~(1<<4); // pak kardene flag marboot be rwgister I/O ba tawjooo be nokteye gofte shhode ke bayad be in sooorat pak shawad chooon nweshtane 1 tasir nadard dar flag ama sefr nweshtan pak mikonad
	P0IEN = (1<<4); // fal kardane intterupt az periphelal 
	IP0 |= (1<<5);   //
	IP1 |= (1<<5);     // dadane awlawiate 3 yani highest be groh port 0 
	IEN1 |=  (1<<5);  // feal kardane intterrupte mawrede nazar az baxshe interrupt
	IEN0 |= (1<<7);  // feal kardane global interrupt	
	//------------------
	
	i2c_cc2530	i2c1;
	MPU6050         imu;
	//char            data = 0;

	P1DIR  |= (1<<1)|(1<<2);
       
	//usb_init();
        //timer1_init();
        
	delay_ms(1000);
     	
        i2c1.a = 0;
	i2c1.b = 0;
	i2c1.I2c_Init();
	
        //	imu initialize : 

	 
	while(!imu.testConnection());
	
	 //LED3 = 1;  
         //imu.setSleepEnabled(false); 																	// thanks to Jack Elston for pointing this one out!

         imu.initialize(); 
        
         //data Ready INT Enable :
      
	char data = imu.getIntDataReadyEnabled();
	while(!data){
        imu.setIntDataReadyEnabled(1);
        data = imu.getIntDataReadyEnabled();
        }
        
	//sprintf(USB_buff,"Rate : %i\n\r",data);
	//usb_send(USB_buff);	
  

	//data = imu.getIntStatus();	

         

	//sprintf(USB_buff,"initialize Done !\n",data);
	//usb_send(USB_buff);


          return 0;
          
	
}