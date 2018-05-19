#include "usart.h"
#include "io.h"
#include "system.h"

u8 GsmFlag=0;
u16 GsmRevCnt=0;
char GsmRev[GsmRevSize]={0};
char temp[GsmRevSize]={0};

/***************************************************************************
功  能: 串口1初始化
参  数: 无
返回值: 无
***************************************************************************/
void USART1_Init()
{
	NVIC_InitTypeDef NVIC_InitStructure;     
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 , ENABLE);
		
	NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=4;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	
	USART_InitStructure.USART_BaudRate=115200;
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	USART_InitStructure.USART_StopBits=USART_StopBits_1;
	USART_InitStructure.USART_Parity=USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode=USART_Mode_Tx | USART_Mode_Rx ;
	USART_Init(USART1,&USART_InitStructure);
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	
	USART_Cmd(USART1,ENABLE);
	
	USART_ClearFlag(USART1,USART_FLAG_TC);
	
}

/***************************************************************************
功  能: 串口1发送一个字节数据
参  数: 发送数据
返回值: 无
***************************************************************************/
void USART1_SendChar(u8 ch)
{
	USART_SendData(USART1,(u8)ch);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
}

/***************************************************************************
功  能: 串口1发送字符串
参  数: 发送字符串
返回值: 无
***************************************************************************/
void USART1_SendStr(char* str)
{
	u16 Count=0;
	while(Count<=strlen(str))
	{
		USART1_SendChar(str[Count++]);
	}
}

/***************************************************************************
功  能: 串口1接受数据中断处理函数
参  数: 无
返回值: 无
***************************************************************************/
void USART1_IRQHandler(void)   
{
	u8 dat;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		dat =USART_ReceiveData(USART1);
		//USART2_SendChar(dat);
	}
}

/***************************************************************************
功  能: 串口2初始化
参  数: 无
返回值: 无
***************************************************************************/
void USART2_Init()
{
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 , ENABLE);
		
	NVIC_InitStructure.NVIC_IRQChannel=USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	
	USART_InitStructure.USART_BaudRate=115200;
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	USART_InitStructure.USART_StopBits=USART_StopBits_1;
	USART_InitStructure.USART_Parity=USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode=USART_Mode_Tx | USART_Mode_Rx ;
	USART_Init(USART2,&USART_InitStructure);
	
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	
	USART_Cmd(USART2,ENABLE);
	
	USART_ClearFlag(USART2,USART_FLAG_TC);
	
}

/***************************************************************************
功  能: 串口2发送一个字节数据
参  数: 发送数据
返回值: 无
***************************************************************************/
void USART2_SendChar(u8 ch)
{
	USART_SendData(USART2,(u8)ch);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
}

/***************************************************************************
功  能: 串口1发送一个字符串
参  数: 发送字符串
返回值: 无
***************************************************************************/
void USART2_SendStr(char* str)
{
	u16 Count=0;
	while(Count<strlen(str))
	{
		USART2_SendChar(str[Count++]);
	}
}

/***************************************************************************
功  能: 串口2接受数据中断处理函数
参  数: 无
返回值: 无
***************************************************************************/
void USART2_IRQHandler(void)   
{
	u8 dat,i;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		dat =USART_ReceiveData(USART2);
		temp[GsmRevCnt++]=dat;
		USART1_SendChar(dat);
		if(GsmRevCnt>GsmRevSize)
		{
			GsmRevCnt=0;
		}
		if(temp[GsmRevCnt-2]==0x0D&&temp[GsmRevCnt-1]==0x0A&&GsmRevCnt>2)
		{
			if(GsmFlag==0)
			{
				if(strstr(temp,"+CMTI"))
				{
					GsmFlag=2;
					USART2_SendStr("AT+CMGL\r\n");
				}
				else
				{
					i=0;
					while(temp[i]!=0)
					{
						GsmRev[i]=temp[i];
						temp[i]=0;
						i++;
					}
				}
			}
			else if(GsmFlag==2) 
			{
				if(strstr(temp,"+CMGL"))
				{
					GsmFlag=1;
				}
			}
			else
			{
				  i=0;
					while(temp[i]!=0)
					{
						GsmRev[i]=temp[i];
						temp[i]=0;
						i++;
					}
					GsmFlag=1;
			}
		}
	}
}

/***************************************************************************
功  能: 清除结束数据缓冲区
参  数: 无
返回值: 无
***************************************************************************/
void GsmRevClean(void)
{
	u16 Count=0;
	while(Count<GsmRevSize)
	{
		GsmRev[Count++]=0;
	}
	GsmRevCnt=0;
}
