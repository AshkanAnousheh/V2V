/**********************************************************

Software I2C Library for AVR Devices.

Copyright 2008-2012
eXtreme Electronics, India
www.eXtremeElectronics.co.in
**********************************************************/


#ifndef _I2CSOFT_H
#define _I2CSOFT_H
class i2c_cc2530
{
public :

 int a,b;
// bejaye delay az increment a estefade kardim ke sorate bishtari dashte bashim
// dar in soorat hududan be soraate 145Kbps residiimmm... va ba optimize compiler ruye HIGH speed be soorate 305Kbps residim...

#define Q_DEL 		a=a+1//for(a=0;a<100;a++) //delay_us()
#define H_DEL 		a=a+1//for(a=0;a<100;a++)//a++ //delay_us()
#define ACK     1
#define NACK     0
/* 
I/O Configuration 
*/
#define 	 LED_Eror_I2C	P0_7
#define SCLPORT	P0	//TAKE PORTD as SCL OUTPUT WRITE
#define SCLDDR	P0DIR	//TAKE DDRB as SCL INPUT/OUTPUT configure

#define SDAPORT	P0	//TAKE PORTD as SDA OUTPUT WRITE
#define SDADDR	P0DIR	//TAKE PORTD as SDA INPUT configure

#define SDAPIN	P0	//TAKE PORTD TO READ DATA
#define SCLPIN	P0	//TAKE PORTD TO READ DATA

#define SCL	6		//PORTD.0 PIN AS SCL PIN
#define SDA	5		//PORTD.1 PIN AS SDA PIN


#define SOFT_I2C_SDA_LOW	SDADDR|=((1<<SDA)) ; P0_5=0           // xroji kardan
#define SOFT_I2C_SDA_HIGH	SDADDR&=(~(1<<SDA))           // vorodi kardan

#define SOFT_I2C_SCL_LOW	SCLDDR|=((1<<SCL)) ;P0_6=0
#define SOFT_I2C_SCL_HIGH	SCLDDR&=(~(1<<SCL))


/**********************************************************
SoftI2CInit()

Description:
	Initializes the Soft I2C Engine.
	Must be called before using any other lib functions.
	
Arguments:
	NONE
	
Returns:
	Nothing

**********************************************************/
void I2c_Init ()
{
        P0INP |=(1<<5)|(1<<6);
	SDAPORT&=~(1<<SDA);  // fek konam pull up kardane paye haye SDA , SCL
	SCLPORT&=~(1<<SCL);
	
	SOFT_I2C_SDA_HIGH;	
	SOFT_I2C_SCL_HIGH;	
		
}
void I2c_StartCondition ()
{       
        SOFT_I2C_SDA_HIGH;	
      	SOFT_I2C_SCL_HIGH;
	H_DEL;
	
	SOFT_I2C_SDA_LOW;	
	H_DEL;
          
}

void I2c_StopCondition ()
{
	 SOFT_I2C_SDA_LOW;
	 H_DEL;
	 SOFT_I2C_SCL_HIGH;
	 Q_DEL;
	 SOFT_I2C_SDA_HIGH;
	 H_DEL;
}

 unsigned char I2c_WriteByte(unsigned char data)
{
	 
	 unsigned char i;
         unsigned char ack;
	 	
	 for(i=0;i<8;i++)
	 {
		SOFT_I2C_SCL_LOW;
		Q_DEL;
		
		if(data & 0x80)
			SOFT_I2C_SDA_HIGH;
		else
			SOFT_I2C_SDA_LOW;	
		
		H_DEL;
		
		SOFT_I2C_SCL_HIGH;
		H_DEL;
		
		//while((SCLPIN & (1<<SCL))==0);
		// while((SCLPIN & (1<<SCL))==0)
		// {
			// b++;
			// if(b>100)
				// break;
		// }	
			
		data=data<<1;
	}
	 
	//The 9th clock (ACK Phase)
	SOFT_I2C_SCL_LOW;
	Q_DEL;
		
	SOFT_I2C_SDA_HIGH;		
	H_DEL;
		
	SOFT_I2C_SCL_HIGH;
	H_DEL;	
	
	ack=!(SDAPIN & (1<<SDA));
	
	SOFT_I2C_SCL_LOW;
	H_DEL;
	
	return ack;
	 
}
 
 
unsigned char I2c_ReadByte(unsigned char ack)
{
	unsigned char data=0x00;
	unsigned char i;
			
	for(i=0;i<8;i++)
	{
			
		SOFT_I2C_SCL_LOW;
		H_DEL;
		SOFT_I2C_SCL_HIGH;
		H_DEL;
			
		// while((SCLPIN & (1<<SCL))==0)
		// {
			// b++;
			// if(b>100)
				// break;
		// }
		
		if(SDAPIN &(1<<SDA))
			data|=(0x80>>i);
			
	}
		
	SOFT_I2C_SCL_LOW;
	Q_DEL;						//Soft_I2C_Put_Ack
	
	if(ack)
	{
          SOFT_I2C_SDA_LOW;
	}
	else
	{
         SOFT_I2C_SDA_HIGH;	
		
	}	
	H_DEL;
	
	SOFT_I2C_SCL_HIGH;
	// H_DEL;
	Q_DEL;		
			
	
	SOFT_I2C_SCL_LOW;
	H_DEL;
	SOFT_I2C_SDA_HIGH;	
	
	return data;
	
}




void write_reg_i2c(unsigned char MAC_7bit , unsigned char address , unsigned char value)
{
  I2c_StartCondition ();
  LED_Eror_I2C=I2c_WriteByte((MAC_7bit<<1) | 0);
  LED_Eror_I2C=I2c_WriteByte(address);
  LED_Eror_I2C=I2c_WriteByte(value);
  I2c_StopCondition ();  
}

void write_multi_i2c(unsigned char MAC_7bit , unsigned char address ,uint8_t length, unsigned char *value)
{
	int i ;
  I2c_StartCondition ();
  LED_Eror_I2C=I2c_WriteByte((MAC_7bit<<1) | 0);
  LED_Eror_I2C=I2c_WriteByte(address);
  for(i=0;i<length;i++)
  LED_Eror_I2C=I2c_WriteByte(value[i]);
  I2c_StopCondition ();  
}


unsigned char read_onereg_i2c(unsigned char MAC_7bit,unsigned char address)
{
  unsigned char data=0;
  
  I2c_StartCondition ();
  LED_Eror_I2C=I2c_WriteByte((MAC_7bit<<1) | 0);
  LED_Eror_I2C=I2c_WriteByte(address);
  I2c_StartCondition (); 
  LED_Eror_I2C=I2c_WriteByte((MAC_7bit<<1) | 1);
  data=I2c_ReadByte(NACK);
  I2c_StopCondition ();
  
  return data;
}

int8_t read_multi_i2c(unsigned char MAC_7bit,unsigned char address,char n,uint8_t * data)
{
  I2c_StartCondition ();
  LED_Eror_I2C=I2c_WriteByte((MAC_7bit<<1) | 0);
  LED_Eror_I2C=I2c_WriteByte(address);
  I2c_StartCondition (); 
  LED_Eror_I2C=I2c_WriteByte((MAC_7bit<<1) | 1);
  int i;
  for (i = 0 ; i < n-1 ; i++)
		data[i] = I2c_ReadByte(ACK);
		data[i] = I2c_ReadByte(NACK);


		// data[i] = I2c_ReadByte(NACK);	
  I2c_StopCondition ();
  return	i;
 }

 
 int8_t read_multi_i2c_w(unsigned char MAC_7bit,unsigned char address,char n,uint16_t * data)
{
  I2c_StartCondition ();
  LED_Eror_I2C=I2c_WriteByte((MAC_7bit<<1) | 0);
  LED_Eror_I2C=I2c_WriteByte(address);
  I2c_StartCondition (); 
  LED_Eror_I2C=I2c_WriteByte((MAC_7bit<<1) | 1);
  int i;
  for (i = 0 ; i < n-1 ; i++)
		data[i] = I2c_ReadByte(ACK);
		data[i] = I2c_ReadByte(NACK);


		// data[i] = I2c_ReadByte(NACK);	
  I2c_StopCondition ();
  return	i;
 }
 
};


#endif