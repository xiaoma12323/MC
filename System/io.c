#include "io.h"

/***************************************************************************
功  能: 初始化LED灯端口
参  数: 无
返回值: 无
***************************************************************************/
void LED_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1 |GPIO_Pin_3|GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
}

/***************************************************************************
功  能: 初始化I/O端口
参  数: 无
返回值: 无
***************************************************************************/
void IO_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_OD;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	S1_OFF;
	S2_OFF;
	S3_OFF;
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_OD;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	WR_OFF;
	
	LED_Init(); 
	
}
