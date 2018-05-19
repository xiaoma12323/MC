#include "stm32f10x.h"
#include "sccb.h"#include "delay.h"
#include "io.h"

#define Cmos7670_Down()    GPIOA->BSRR = GPIO_Pin_7 ;
#define Cmos7670_On()    	 GPIOA->BRR = GPIO_Pin_7 ;
#define FIFO_WE    						GPIO_Pin_4
#define FIFO_OE    						GPIO_Pin_5
#define FIFO_RCK    					GPIO_Pin_6

#define FIFO_Write_Disable()  GPIOA->BRR = FIFO_WE
#define FIFO_Write_Enable()   GPIOA->BSRR = FIFO_WE

#define FIFO_OE_L()  					GPIOA->BRR = FIFO_OE
#define FIFO_OE_H()  	  			GPIOA->BSRR = FIFO_OE

#define FIFO_RCK_L() 				  GPIOA->BRR = FIFO_RCK
#define FIFO_RCK_H()  			  GPIOA->BSRR = FIFO_RCK

#define FIFO_RRST_L() 				GPIOA->BRR = GPIO_Pin_11;
#define FIFO_RRST_H()  			  GPIOA->BSRR = GPIO_Pin_11;

#define FIFO_WRST_L() 				GPIOA->BRR = GPIO_Pin_12;
#define FIFO_WRST_H()  			  GPIOA->BSRR = GPIO_Pin_12;


u8 WrCmos7670(u8 regID, u8 regDat);
u8 RdCmos7670(u8 regID, u8 *regDat);
void OV7670_Config_Window(u16 startx,u16 starty,u16 width, u16 height);
void OV7670_Init(void);
void EXTI4_Init(void);
void set_Cmos7670reg(void);
void Set_OV7670reg(void);
void FIFO_Init(void);
void Take_Pic(void);
