#include "stm32f10x.h"

#define SCCB_SIC_BIT     GPIO_Pin_0	
#define SCCB_SID_BIT     GPIO_Pin_1	 

#define SCCB_SIC_H()     GPIOB->BSRR = SCCB_SIC_BIT;  //对端口A特定位的设置	,这里设置高
#define SCCB_SIC_L()     GPIOB->BRR =  SCCB_SIC_BIT;  //这里设置低

#define SCCB_SID_H()     GPIOB->BSRR = SCCB_SID_BIT;  //同上
#define SCCB_SID_L()     GPIOB->BRR =  SCCB_SID_BIT;

#define SCCB_SID_IN      SCCB_SID_GPIO_INPUT();
#define SCCB_SID_OUT     SCCB_SID_GPIO_OUTPUT();


#define SCCB_SID_STATE	 GPIOB->IDR&0x02

///////////////////////////////////////////	 
void SCCB_GPIO_Config(void);
void SCCB_SID_GPIO_OUTPUT(void);
void SCCB_SID_GPIO_INPUT(void);
void startSCCB(void);
void stopSCCB(void);
void noAck(void);
u8 SCCBwriteByte(u8 m_data);
u8 SCCBreadByte(void);

