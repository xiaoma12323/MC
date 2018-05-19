#include "sccb.h"
#include "delay.h"

/***************************************************************************
功  能: 初始化模拟SCCB端口
参  数: 无
返回值: 无
***************************************************************************/
void SCCB_GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	
  GPIO_InitStructure.GPIO_Pin =  SCCB_SIC_BIT|SCCB_SID_BIT;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		  //GPIO_Mode_AIN = 0x0,     //模拟输入
  GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;		  //GPIO_Mode_IN_FLOATING = 0x04, //悬空输入
  GPIO_Init(GPIOB, &GPIO_InitStructure);				  //GPIO_Mode_IPD = 0x28,    //下拉输入
														  //GPIO_Mode_IPU = 0x48,    //上拉输入
														  //GPIO_Mode_Out_OD = 0x14, //开漏输出
														  //GPIO_Mode_Out_PP = 0x10,  //推挽输出
														  //GPIO_Mode_AF_OD = 0x1C,   //开漏复用
														  //GPIO_Mode_AF_PP = 0x18    //推挽复用 
}

/***************************************************************************
功  能: 设置SID接口为输出模式
参  数: 无
返回值: 无
***************************************************************************/
void SCCB_SID_GPIO_OUTPUT(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  GPIO_InitStructure.GPIO_Pin =  SCCB_SID_BIT;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/***************************************************************************
功  能: 设置SID接口为输入模式
参  数: 无
返回值: 无
***************************************************************************/
void SCCB_SID_GPIO_INPUT(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  GPIO_InitStructure.GPIO_Pin =  SCCB_SID_BIT;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
 // GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/***************************************************************************
功  能: Start命令，SCCB起始信号
参  数: 无
返回值: 无
***************************************************************************/
void startSCCB(void)
{
    SCCB_SID_H();     //数据线高电平
    Delay_us(500);

    SCCB_SIC_H();	   //在时钟线高的时候数据线由高至低
    Delay_us(500);
 
    SCCB_SID_L();
    Delay_us(500);

    SCCB_SIC_L();	 //数据线恢复低电平，单操作函数必要
    Delay_us(500);


}

/***************************************************************************
功  能: Stop命令，SCCB终止信号
参  数: 无
返回值: 无
***************************************************************************/
void stopSCCB(void)
{
    SCCB_SID_L();
    Delay_us(500);
 
    SCCB_SIC_H();	
    Delay_us(500);
  

    SCCB_SID_H();	
    Delay_us(500);
   
}


/***************************************************************************
功  能: noAck,用于连续读取中的最后一个结束周期
参  数: 无
返回值: 无
***************************************************************************/
void noAck(void)
{
	
	SCCB_SID_H();	
	Delay_us(500);
	
	SCCB_SIC_H();	
	Delay_us(500);
	
	SCCB_SIC_L();	
	Delay_us(500);
	
	SCCB_SID_L();	
	Delay_us(500);

}

/***************************************************************************
功  能: 写入一个字节的数据到SCCB
参  数: 写入数据
返回值: 0(写入失败), 1(写入成功)
***************************************************************************/
u8 SCCBwriteByte(u8 m_data)
{
	u8 i,tem;

	for(i=0;i<8;i++) //循环8次发送数据
	{
		if((m_data<<i)&0x80)
		{
			SCCB_SID_H();	
		}
		else
		{
			SCCB_SID_L();	
		}
		Delay_us(500);
		SCCB_SIC_H();	
		Delay_us(500);
		SCCB_SIC_L();	
		Delay_us(500);

	}
	Delay_us(100);
	SCCB_SID_IN;/*设置SDA为输入*/
	Delay_us(500);
	SCCB_SIC_H();	
	Delay_us(100);
	if(SCCB_SID_STATE){tem=0;}   //SDA=1发送失败，返回0}
	else {tem=1;}   //SDA=0发送成功，返回1
	SCCB_SIC_L();	
	Delay_us(500);	
  SCCB_SID_OUT;/*设置SDA为输出*/
	return (tem);  
}


/***************************************************************************
功  能: 读一个字节数据读取并且返回
参  数: 无
返回值: 读取到的数据
***************************************************************************/
u8 SCCBreadByte(void)
{
	u8 read,j;
	read=0x00;
	
	SCCB_SID_IN;/*设置SDA为输入*/
	Delay_us(500);
	for(j=0;j<8;j++) //循环8次接收数据
	{		     
		Delay_us(500);
		SCCB_SIC_H();
		Delay_us(500);
		read=read<<1;
		if(SCCB_SID_STATE) 
		{
			read=read+1;
		}
		SCCB_SIC_L();
		Delay_us(500);
	}
	return(read);
}
