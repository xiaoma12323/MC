#include "ov7670.h"
#include "delay.h"
#include "usart.h"

/***************************************************************************
功  能: 写OV7670寄存器
参  数: regID(寄存器地址)，regDat(写入寄存器数据)
返回值: 0(写入失败),1(写入成功)
***************************************************************************/
u8 WrCmos7670(u8 regID, u8 regDat)
{
	
	startSCCB();
	if(0==SCCBwriteByte(0x42))
	{
		stopSCCB();
		return(0);
	}
	Delay_us(100);
  	if(0==SCCBwriteByte(regID))
	{
		stopSCCB();
		return(0);
	}
	Delay_us(100);
  	if(0==SCCBwriteByte(regDat))
	{
		stopSCCB();
		return(0);
	}
  	stopSCCB();
	
  	return(1);
}
/***************************************************************************
功  能: 读OV7670寄存器
参  数: regID(寄存器地址)，regDat(读出数据存放地址)
返回值: 0(读取失败),1(读取成功)
***************************************************************************/
u8 RdCmos7670(u8 regID, u8 *regDat)
{
	//通过写操作设置寄存器地址
	startSCCB();
	if(0==SCCBwriteByte(0x42))
	{
		stopSCCB();
		return(0);
	}
	Delay_us(100);
  	if(0==SCCBwriteByte(regID))
	{
		stopSCCB();
		return(0);
	}
	stopSCCB();
	
	Delay_us(100);
	
	//设置寄存器地址后，才是读
	startSCCB();
	if(0==SCCBwriteByte(0x43))
	{ 
		stopSCCB();
		return(0);
	}
	Delay_us(100);
  *regDat=SCCBreadByte();
 	noAck();
 	stopSCCB(); 
 	return(1);
}

/***************************************************************************
功  能: 设置OV7670采集图像大小
参  数: startx(像素起始横坐标), starty(像素启示纵坐标), width(图像宽度), height(图像高度)
返回值: 无
***************************************************************************/
//(140,16,640,480) is good for VGA
//(272,16,320,240) is good for QVGA
void OV7670_Config_Window(u16 startx,u16 starty,u16 width, u16 height)
{
	u16 endx;
	u16 endy;
	u8 temp_reg1, temp_reg2;
	u8 temp=0;
	
	endx=(startx+width);                                          
	endy=(starty+height*2);
	temp_reg1 = 0x00;
	temp_reg2 = 0x80;
	
	// Horizontal
	temp = temp_reg2|((endx&0x7)<<3)|(startx&0x7);
	WrCmos7670(0x32, temp );
	temp = (startx&0x7FF)>>3;
	WrCmos7670(0x17, temp );
	temp = (endx&0x7FF)>>3;
	WrCmos7670(0x18, temp );
	
	// Vertical
	temp =temp_reg1|((endy&0x3)<<2)|(starty&0x3);
	WrCmos7670(0x03, temp );
	temp = starty>>2;
	WrCmos7670(0x19, temp );
	temp = endy>>2;
	WrCmos7670(0x1A, temp );
}

/***************************************************************************
功  能: 初始化OV7670模块
参  数: 无
返回值: 无
***************************************************************************/
void OV7670_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//PC5-PC12作为数据输入口
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	Cmos7670_On();

	SCCB_GPIO_Config();
	
	while(0==WrCmos7670(0x12, 0x80)); //Reset SCCB
	
	Delay_ms(100);
  set_Cmos7670reg();
	
	FIFO_Init();

}

/***************************************************************************
功  能: 初始化OV7670场中断
参  数: 无
返回值: 无
***************************************************************************/
void EXTI4_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	//PC4 帧同步信号
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line4;           
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;      
  EXTI_Init(&EXTI_InitStructure); 
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource4);

	NVIC_InitStructure.NVIC_IRQChannel=EXTI4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/***************************************************************************
功  能: 设置OV7670寄存器
参  数: 无
返回值: 无
***************************************************************************/
void set_Cmos7670reg(void)
{        
	

         WrCmos7670(0x67,0x11);     
         WrCmos7670(0x68,0xff);     

				 WrCmos7670(0x3a,0x04);     
         WrCmos7670(0x40,0x10);   //输出方式为RGB565  
         WrCmos7670(0x12,0x14);  	//输出格式为QVGA

	
         WrCmos7670(0x32,0x80);
         WrCmos7670(0x17,0x16);     
         WrCmos7670(0x18,0x04);     
         WrCmos7670(0x19,0x02);
         WrCmos7670(0x1a,0x7b);
         WrCmos7670(0x03,0x06);     
         WrCmos7670(0x0c,0x00);
         WrCmos7670(0x3e,0x00);
         WrCmos7670(0x70,0x3a);
         WrCmos7670(0x71, 0x35);
         WrCmos7670(0x72,0x11);
         WrCmos7670(0x73,0x00);
         WrCmos7670(0xa2,0x02);
         WrCmos7670(0x11,0x81);
        
         WrCmos7670(0x7a,0x20);
         WrCmos7670(0x7b,0x1c);
         WrCmos7670(0x7c,0x28);
         WrCmos7670(0x7d,0x3c);
         WrCmos7670(0x7e,0x55);
         WrCmos7670(0x7f,0x68);
         WrCmos7670(0x80,0x76);
         WrCmos7670(0x81,0x80);
         WrCmos7670(0x82,0x88);
         WrCmos7670(0x83,0x8f);
         WrCmos7670(0x84,0x96);
         WrCmos7670(0x85,0xa3);
         WrCmos7670(0x86,0xaf);
         WrCmos7670(0x87,0xc4);
         WrCmos7670(0x88,0xd7);
         WrCmos7670(0x89,0xe8);
        
         WrCmos7670(0x13,0xe0);
         WrCmos7670(0x00,0x00);
        
         WrCmos7670(0x10,0x00);
         WrCmos7670(0x0d,0x00);
         WrCmos7670(0x14,0x28);
         WrCmos7670(0xa5,0x05);
         WrCmos7670(0xab,0x07);
         WrCmos7670(0x24,0x75);
         WrCmos7670(0x25,0x63);
         WrCmos7670(0x26,0xA5);
         WrCmos7670(0x9f,0x78);
         WrCmos7670(0xa0,0x68);
         WrCmos7670(0xa1,0x03);
         WrCmos7670(0xa6,0xdf);
         WrCmos7670(0xa7,0xdf);
         WrCmos7670(0xa8,0xf0);
         WrCmos7670(0xa9,0x90);
         WrCmos7670(0xaa,0x94);
         WrCmos7670(0x13,0xe5);
 
         WrCmos7670(0x0e,0x61);
         WrCmos7670(0x0f,0x4b);
         WrCmos7670(0x16,0x02);
         WrCmos7670(0x1e,0x37);
         WrCmos7670(0x21,0x02);
         WrCmos7670(0x22,0x91);
         WrCmos7670(0x29,0x07);
         WrCmos7670(0x33,0x0b);
         WrCmos7670(0x35,0x0b);
         WrCmos7670(0x37,0x1d);
         WrCmos7670(0x38,0x71);
         WrCmos7670(0x39,0x2a);
         WrCmos7670(0x3c,0x78);
         WrCmos7670(0x4d,0x40);
         WrCmos7670(0x4e,0x20);
         WrCmos7670(0x69,0x00);
         WrCmos7670(0x6b,0x60);
         WrCmos7670(0x74,0x19);
         WrCmos7670(0x8d,0x4f);
         WrCmos7670(0x8e,0x00);
         WrCmos7670(0x8f,0x00);
         WrCmos7670(0x90,0x00);
         WrCmos7670(0x91,0x00);
         WrCmos7670(0x92,0x00);
         WrCmos7670(0x96,0x00);
         WrCmos7670(0x9a,0x80);
         WrCmos7670(0xb0,0x84);
         WrCmos7670(0xb1,0x0c);
         WrCmos7670(0xb2,0x0e);
         WrCmos7670(0xb3,0x82);
         WrCmos7670(0xb8,0x0a);
 
         WrCmos7670(0x43,0x14);
         WrCmos7670(0x44,0xf0);
         WrCmos7670(0x45,0x34);
         WrCmos7670(0x46,0x58);
         WrCmos7670(0x47,0x28);
         WrCmos7670(0x48,0x3a);
         WrCmos7670(0x59,0x88);
         WrCmos7670(0x5a,0x88);
         WrCmos7670(0x5b,0x44);
         WrCmos7670(0x5c,0x67);
         WrCmos7670(0x5d,0x49);
         WrCmos7670(0x5e,0x0e);
         WrCmos7670(0x64,0x04);
         WrCmos7670(0x65,0x20);
         WrCmos7670(0x66,0x05);
         WrCmos7670(0x94,0x04);
         WrCmos7670(0x95,0x08);
         WrCmos7670(0x6c,0x0a);
         WrCmos7670(0x6d,0x55);
         WrCmos7670(0x6e,0x11);
         WrCmos7670(0x6f,0x9f);
         WrCmos7670(0x6a,0x40);
         WrCmos7670(0x01,0x40);
         WrCmos7670(0x02,0x40);
         WrCmos7670(0x13,0xe7);
         WrCmos7670(0x15,0x00); 
        
        
         WrCmos7670(0x4f,0x80);
         WrCmos7670(0x50,0x80);
         WrCmos7670(0x51,0x00);
         WrCmos7670(0x52,0x22);
         WrCmos7670(0x53,0x5e);
         WrCmos7670(0x54,0x80);
         WrCmos7670(0x58,0x9e);
        
         WrCmos7670(0x41,0x08);
         WrCmos7670(0x3f,0x00);
         WrCmos7670(0x75,0x05);
         WrCmos7670(0x76,0xe1);
         WrCmos7670(0x4c,0x00);
         WrCmos7670(0x77,0x01);
         WrCmos7670(0x3d,0x80);
         WrCmos7670(0x4b,0x09);
         WrCmos7670(0xc9,0x60);
         WrCmos7670(0x41,0x38);
         WrCmos7670(0x56,0x40);
        
         WrCmos7670(0x34,0x11);
         WrCmos7670(0x3b,0x02);
                                                                          
         WrCmos7670(0xa4,0x89);
         WrCmos7670(0x96,0x00);
         WrCmos7670(0x97,0x30);
         WrCmos7670(0x98,0x20);
         WrCmos7670(0x99,0x30);
         WrCmos7670(0x9a,0x84);
         WrCmos7670(0x9b,0x29);
         WrCmos7670(0x9c,0x03);
         WrCmos7670(0x9d,0x4c);
         WrCmos7670(0x9e,0x3f);
         WrCmos7670(0x78,0x04);
        
         WrCmos7670(0x79,0x01);
         WrCmos7670(0xc8,0xf0);
         WrCmos7670(0x79,0x0f);
         WrCmos7670(0xc8,0x00);
         WrCmos7670(0x79,0x10);
         WrCmos7670(0xc8,0x7e);
         WrCmos7670(0x79,0x0a);
         WrCmos7670(0xc8,0x80);
         WrCmos7670(0x79,0x0b);
         WrCmos7670(0xc8,0x01);
         WrCmos7670(0x79,0x0c);
         WrCmos7670(0xc8,0x0f);
         WrCmos7670(0x79,0x0d);
         WrCmos7670(0xc8,0x20);
         WrCmos7670(0x79,0x09);
         WrCmos7670(0xc8,0x80);
         WrCmos7670(0x79,0x02);
         WrCmos7670(0xc8,0xc0);
         WrCmos7670(0x79,0x03);
         WrCmos7670(0xc8,0x40);
         WrCmos7670(0x79,0x05);
         WrCmos7670(0xc8,0x30);
         WrCmos7670(0x79,0x26);
         WrCmos7670(0x09,0x00);     
}

/***************************************************************************
功  能: 初始化FIFO各引脚
参  数: 无
返回值: 无
***************************************************************************/
void FIFO_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	FIFO_OE_L();
}
/***************************************************************************
功  能: 生成一张照片并将像素数据通过串口发送
参  数: 无
返回值: 无
***************************************************************************/
void Take_Pic(void)
{
	u8 i;
	u8 bmp_head[70]={ 0x42,0x4D,0x48,0x58,0x02,0x00,0x00,0x00,0x00,0x00,0x46,0x00,0x00,0x00,0x38,0x00,
									  0x00,0x00,0x40,0x01,0x00,0x00,0x10,0xFF,0xFF,0xFF,0x01,0x00,0x10,0x00,0x03,0x00,
								 	  0x00,0x00,0x02,0x58,0x02,0x00,0x12,0x0B,0x00,0x00,0x12,0x0B,0x00,0x00,0x00,0x00,
									  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0x00,0x00,0xE0,0x07,0x00,0x00,0x1F,0x00,
									  0x00,0x00,0x00,0x00,0x00,0x00};
	LED2_ON;
										
	OV7670_Init();
	for(i=0;i<70;i++)
	{
		USART2_SendChar(bmp_head[i]);
	}
	EXTI4_Init();
}

