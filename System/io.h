#include "stm32f10x.h"

#define LED2_ON GPIO_ResetBits(GPIOC,GPIO_Pin_3)
#define LED2_OFF GPIO_SetBits(GPIOC,GPIO_Pin_3)

#define LED3_ON GPIO_ResetBits(GPIOC,GPIO_Pin_1)
#define LED3_OFF GPIO_SetBits(GPIOC,GPIO_Pin_1)

#define LED4_ON GPIO_ResetBits(GPIOC,GPIO_Pin_13)
#define LED4_OFF GPIO_SetBits(GPIOC,GPIO_Pin_13)
#define S1_ON GPIO_ResetBits(GPIOB,GPIO_Pin_13)
#define S1_OFF GPIO_SetBits(GPIOB,GPIO_Pin_13)

#define S2_ON GPIO_ResetBits(GPIOB,GPIO_Pin_14)
#define S2_OFF GPIO_SetBits(GPIOB,GPIO_Pin_14)

#define S3_ON GPIO_ResetBits(GPIOB,GPIO_Pin_15)
#define S3_OFF GPIO_SetBits(GPIOB,GPIO_Pin_15)

#define WR_ON GPIO_ResetBits(GPIOA,GPIO_Pin_1)
#define WR_OFF GPIO_SetBits(GPIOA,GPIO_Pin_1)

void LED_Init(void);void IO_Init(void);
