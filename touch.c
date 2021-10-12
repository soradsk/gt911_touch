#include "touch.h"
//#include "ra8873_api.h"
unsigned char sht_BitCnt;
unsigned char sht_Bit;
unsigned char sht_retc = 0;
unsigned int sht_t = 100;
unsigned char buff[14];

#define TOUCH_LAST_X_LIST 12
int touch_last_x_list[TOUCH_LAST_X_LIST];
int touch_last_x_list_size = 0;
coordinate_touch coordinateTouch;


void touch_init(){
	coordinateTouch.x = 0;
	coordinateTouch.y = 0;
	coordinateTouch.start_x = 0;
	coordinateTouch.count = 0;
	coordinateTouch.used = 0;
	
	LCD_TUCH_INT = 0;
	LCD_TUCH_RESET = 0;
	Delay_us(400);
	LCD_TUCH_RESET = 1;
	//for(i = 0 ; i < 10 ; i++){
	touch_init_clean();
	//}
}

void IIC_ACK(){ //主机发送ACK
	//CLOSE_SYS_ISR();
  LCD_TUCH_SCL = 0; 
	Delay_us(1); 
  LCD_TUCH_SDA = 0;
  Delay_us(1);     
  LCD_TUCH_SCL = 1;
  Delay_us(1);                  
  LCD_TUCH_SCL = 0; 
	LCD_TUCH_SDA = 1;
  Delay_us(1);	
	//OPEN_SYS_ISR(); 
}

void IIC_NACK(){  //主机不发送ACK
	//CLOSE_SYS_ISR();
  LCD_TUCH_SCL = 0;
  Delay_us(1); 
  LCD_TUCH_SDA = 1;
  Delay_us(1);     
  LCD_TUCH_SCL = 1;
  Delay_us(1);                  
  LCD_TUCH_SCL = 0;  
	LCD_TUCH_SDA = 1;	
  Delay_us(1);
	//OPEN_SYS_ISR(); 
}


unsigned int IIC_wait_ACK(){  //等待从机的ACK

	int sht_t = 2;
	//CLOSE_SYS_ISR();
	LCD_TUCH_SDA=1;
	Delay_us(1);
	LCD_TUCH_SCL=0;
	Delay_us(1); 

	while(LCD_TUCH_SDA){
		sht_t--;
		Delay_us(1); 
		if(sht_t<=0){
			LCD_TUCH_SCL=0;
			return 1;
		}
		Delay_us(1); 
	}
	Delay_us(1);      
	LCD_TUCH_SCL=1;
	Delay_us(1);
	LCD_TUCH_SCL=0;             
	Delay_us(1);   
  //OPEN_SYS_ISR(); 	
	return 0;
 
}

void IIC_Start(){  //开始命令
	//CLOSE_SYS_ISR();
  LCD_TUCH_SDA = 1;
  LCD_TUCH_SCL = 1;
  Delay_us(1);	
  LCD_TUCH_SDA = 0;
  Delay_us(1); 
  LCD_TUCH_SCL = 0;
	Delay_us(1); 
	//OPEN_SYS_ISR(); 
}

void IIC_Stop(){  //结束命令
	//CLOSE_SYS_ISR();
	LCD_TUCH_SCL = 0;
	LCD_TUCH_SDA = 0;  
	Delay_us(1);	
	LCD_TUCH_SCL = 1;
	Delay_us(1);
	LCD_TUCH_SDA = 1;
	Delay_us(1);
	//OPEN_SYS_ISR(); 
}

void  IIC_SendByte(unsigned char byte){//字节发送
   //CLOSE_SYS_ISR();
	for(sht_BitCnt = 0x80;sht_BitCnt != 0; sht_BitCnt >>= 1){
		if((byte & sht_BitCnt) == 0){ 
      LCD_TUCH_SDA = 0;
    }else{ 
			LCD_TUCH_SDA = 1;
    }				
		Delay_us(5); 
		LCD_TUCH_SCL = 1;
		Delay_us(5);
		LCD_TUCH_SCL = 0;
		Delay_us(5);
	}
	//OPEN_SYS_ISR(); 
}

unsigned char IIC_RcvByte(){//字节接收
  //CLOSE_SYS_ISR();
  for(sht_Bit=0;sht_Bit<8;sht_Bit++){  
	  LCD_TUCH_SCL = 0;
	  Delay_us(5);               
	  LCD_TUCH_SCL = 1;               
	  sht_retc=sht_retc<<1;
	  if(LCD_TUCH_SDA){ 
			sht_retc |= 1;
    }			
	  Delay_us(5);
  }
  LCD_TUCH_SCL = 0;   
	//OPEN_SYS_ISR(); 
  return sht_retc;
}

void clean_buff()
{
	int i;
	for(i=0;i<14;i++)
	{
		buff[i] = 0;
	}
}

void touch_run(){

	unsigned char temp = 0x00;

	IIC_Start();
	IIC_SendByte(0xBA);
	IIC_wait_ACK();
	IIC_SendByte(0x81);
	IIC_wait_ACK();
	IIC_SendByte(0x4E);
	IIC_wait_ACK();
	IIC_Stop();
	
	IIC_Start();
	IIC_SendByte(0xBB);
  IIC_wait_ACK();
	temp = IIC_RcvByte();
	IIC_NACK();
	IIC_Stop();

	if((temp&0x80)==0x80){//0x80
		
		if((temp & 0x01)==0x01)
		{
			coordinateTouch.count ++;
			if(coordinateTouch.count > 100)
				coordinateTouch.count = 100;
			coordinateTouch.signal = 1;
			IIC_Start();
			IIC_SendByte(0xBA);
			IIC_wait_ACK();
			IIC_SendByte(0x81);
			IIC_wait_ACK();
			IIC_SendByte(0x50);
			IIC_wait_ACK();
			IIC_Stop();
			
			IIC_Start();
			IIC_SendByte(0xBB);
			IIC_wait_ACK();
			buff[0]=IIC_RcvByte();
			IIC_ACK();
			buff[1]=IIC_RcvByte();
			IIC_ACK();
			buff[2]=IIC_RcvByte();
			IIC_ACK();
			buff[3]=IIC_RcvByte();
			/*
			IIC_ACK();
			buff[4]=IIC_RcvByte();
			IIC_ACK();
			buff[5]=IIC_RcvByte();
			IIC_ACK();
			buff[6]=IIC_RcvByte();
			IIC_ACK();
			buff[7]=IIC_RcvByte();
			IIC_ACK();
			buff[8]=IIC_RcvByte();
			IIC_ACK();
			buff[9]=IIC_RcvByte();
			IIC_ACK();
			buff[10]=IIC_RcvByte();
			IIC_ACK();
			buff[11]=IIC_RcvByte();
			IIC_ACK();
			buff[12]=IIC_RcvByte();
			IIC_ACK();
			buff[13]=IIC_RcvByte();
			IIC_ACK();
			buff[14]=IIC_RcvByte();
			*/
			IIC_NACK();
			IIC_Stop();
		}
		else
		{
			coordinateTouch.signal = -1;
		}
		
		
		touch_clean();
  }
	
}

void touch_start()
{
	if(coordinateTouch.signal == 1 && coordinateTouch.count == 1)
	{
		int x,y;
		
		x = (((buff[1])<<8) | buff[0]);  
		y = (((buff[3])<<8) | buff[2]);
		if(x > 0 && x < 480)
		{
			coordinateTouch.start_x = x;  
			coordinateTouch.start_y = y;
			coordinateTouch.x = x;
			coordinateTouch.y = y;
			coordinateTouch.signal = TOUCH_START;
			coordinateTouch.type = 0;
		}
	}
}

void touch_move()
{
	if(coordinateTouch.signal == 1 && coordinateTouch.count > 1)
	{
		int x,y;
		
		x = (((buff[1])<<8) | buff[0]);  
		y = (((buff[3])<<8) | buff[2]);
		if(x > 0 && x < 480)
		{
			touch_last_x_list[touch_last_x_list_size] = x;
			touch_last_x_list_size++;
			if(touch_last_x_list_size > TOUCH_LAST_X_LIST - 1)
			{
				int i;
				touch_last_x_list_size = TOUCH_LAST_X_LIST - 1;
				for(i = 0; i < TOUCH_LAST_X_LIST - 1; i++)
				{
					touch_last_x_list[i] = touch_last_x_list[i+1];
				}
			}
			coordinateTouch.x = x;  
			coordinateTouch.y = y;
			coordinateTouch.signal = TOUCH_MOVE;
			if(coordinateTouch.count > TOUCH_TIME_LONG_CLICK)
				coordinateTouch.type = TOUCH_TYPE_LONG_CLICK;
		}
	}
}

void touch_end()
{
	if(coordinateTouch.signal == -1 && coordinateTouch.count > 0)
	{
		int x ,y;
		
		x = (((buff[1])<<8) | buff[0]);  
		y = (((buff[3])<<8) | buff[2]);
		if(x > 0 && x < 480)
		{
			//coordinateTouch.v = (touch_last_x_list[touch_last_x_list_size] - touch_last_x_list[0]);// /TOUCH_LAST_X_LIST;
			coordinateTouch.x = x;
			coordinateTouch.y = y;
			coordinateTouch.signal = TOUCH_END;
			coordinateTouch.v = (x - coordinateTouch.start_x) ;//*100 /coordinateTouch.count;
			//coordinateTouch.used = 0;
			if((coordinateTouch.count > TOUCH_TIME_CLICK) && (coordinateTouch.count <= TOUCH_TIME_LONG_CLICK))
				coordinateTouch.type = TOUCH_TYPE_CLICK;
		}
		touch_last_x_list_size = 0;
		coordinateTouch.count = 0;
		//coordinateTouch.type = 0;
		clean_buff();
	}
}
void coordinate_clean()
{
	coordinateTouch.x = -1;
	coordinateTouch.y = -1;
	coordinateTouch.start_x = -1;  
	coordinateTouch.start_y = -1;
	coordinateTouch.v = 0;
	coordinateTouch.used = 0;
}
coordinate_touch * get_coordinate()
{
	touch_start();
	touch_move();
	touch_end();
	//memcpy(&coordinateTouch,&coordinateTouchGet,sizeof(coordinate_touch));
	
	return &coordinateTouch;
}

void touch_clean()
{
	IIC_Start();
	IIC_SendByte(0xBA);
	IIC_wait_ACK();
	IIC_SendByte(0x81);
	IIC_wait_ACK();
	IIC_SendByte(0x4E);
	IIC_wait_ACK();
	IIC_SendByte(0x00);
	IIC_wait_ACK();
	IIC_SendByte(0x00);
	IIC_wait_ACK();
	IIC_Stop();
}

void touch_init_clean()
{

	unsigned char temp = 0x00;
	IIC_Start();
	IIC_SendByte(0xBA);
	IIC_wait_ACK();
	IIC_SendByte(0x81);
	IIC_wait_ACK();
	IIC_SendByte(0x4E);
	IIC_wait_ACK();
	IIC_Stop();
	
	IIC_Start();
	IIC_SendByte(0xBB);
  IIC_wait_ACK();
	temp=IIC_RcvByte();
	IIC_NACK();
	IIC_Stop();

	if(temp & 0x80)
	{
	 touch_clean();
	}
	touch_clean();
}

