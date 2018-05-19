#include "system.h"

ErrorStatus HSEStartUpStatus;

u8 VSYNC_Flag=0;		//OV7670场中断标记

/***************************************************************************
功  能: 系统时钟初始化，72Mhz
参  数: 无
返回值: 无
***************************************************************************/
void System_Init()
{
	  /* RCC system reset(for debug purpose) */
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if (HSEStartUpStatus == SUCCESS)
  {
    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_2);
 
    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1); 
  
    /* PCLK2 = HCLK */
    RCC_PCLK2Config(RCC_HCLK_Div1); 

    /* PCLK1 = HCLK/2 */
    RCC_PCLK1Config(RCC_HCLK_Div2);

    /* PLLCLK = 8MHz * 9 = 72 MHz */
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);

    /* Enable PLL */ 
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till PLL is used as system clock source */
    while(RCC_GetSYSCLKSource() != 0x08);
	}
		
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO ,ENABLE);
	
}


/***************************************************************************
功  能: 启动GSM模块
参  数: 无
返回值: 无
***************************************************************************/
void Gsm_Reset()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);
	Delay_s(2);
	GPIO_SetBits(GPIOB,GPIO_Pin_12);
}


/***************************************************************************
功  能: 初始化红外感应模块中断
参  数: 无
返回值: 无
***************************************************************************/
void EXTI7_Init()
{
  GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef  EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

 
  EXTI_InitStructure.EXTI_Line = EXTI_Line7;           
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;      
  EXTI_Init(&EXTI_InitStructure); 
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource7);


	NVIC_InitStructure.NVIC_IRQChannel=EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

/***************************************************************************
功  能: ov7670场中断处理函数
参  数: 无
返回值: 无
***************************************************************************/
void EXTI4_IRQHandler()
{
	

	if(EXTI_GetITStatus(EXTI_Line4) != RESET)
	{
		u8 t1,t2;
		u16 i,j;

		EXTI_InitTypeDef EXTI_InitStructure;
		
		VSYNC_Flag++;
		
		if(VSYNC_Flag==1)    //FIFOD
    {
      FIFO_WRST_H();                  //FIFO_WRST=1;
      __asm{nop;}
      FIFO_WRST_L();    //FIFO_WRST=0;           
      Delay_us(2);
      FIFO_WRST_H();                 //FIFO_WRST=1; 
      FIFO_Write_Enable();                 
    }
		
		if(VSYNC_Flag==2)
		{	
      FIFO_Write_Disable();    //FIFO_WR=0;     //禁止数据写入FIFO

      EXTI_InitStructure.EXTI_Line= EXTI_Line4;
      EXTI_InitStructure.EXTI_Mode= EXTI_Mode_Interrupt;
      EXTI_InitStructure.EXTI_Trigger= EXTI_Trigger_Falling;
      EXTI_InitStructure.EXTI_LineCmd= DISABLE;
      EXTI_Init(&EXTI_InitStructure);      
     
			FIFO_RCK_H();                  //FIFO_RCK=1;         
                  
      FIFO_RRST_L();    //FIFO_RRST=0;  //读指针复位
 
      FIFO_RCK_L();     //FIFO_RCK=0;  
      __asm{nop;}__asm{nop;}
      FIFO_RCK_H();                   //FIFO_RCK=1;         
      __asm{nop;}__asm{nop;}
                  
      FIFO_RCK_L();     //FIFO_RCK=0;
                  
      FIFO_RRST_H();             //FIFO_RRST=1;
      __asm{nop;}
 
      Delay_us(2);
      FIFO_OE_L();     //FIFO_OE=0;//
      __asm{nop;}       
      

			LED2_OFF;
			LED3_ON;
      for(i = 0; i< 9600; i++)                               
      {     
        for(j = 0; j < 8; j ++)
        {                                                                                                                                          
          FIFO_RCK_L();//FIFO_RCK=0;   
          __asm{nop;}
                                    
          FIFO_RCK_H();         //FIFO_RCK=1;         
          __asm{nop;}
                                    
          t1=(0x1fe0&GPIOC->IDR)>>5;       
                                    
          FIFO_RCK_L();//FIFO_RCK=0;
          __asm{nop;}
        
          FIFO_RCK_H();         //FIFO_RCK=1;         
          __asm{nop;}
                                    
          t2=(0x1fe0&GPIOC->IDR)>>5;
					USART2_SendChar(t2);
					USART2_SendChar(t1);
         }
        }
				USART2_SendChar(0x00);
				LED3_OFF;
                  
        FIFO_OE_L();                            //FIFO_OE=1;                     
                  
        VSYNC_Flag=0;
				Cmos7670_Down();
		}
		
		EXTI_ClearFlag(EXTI_Line4);      
    EXTI_ClearITPendingBit(EXTI_Line4);
	}
}

/***************************************************************************
功  能: 红外感应模块中断处理函数
参  数: 无
返回值: 无
***************************************************************************/
void EXTI9_5_IRQHandler ()
 {
   if(EXTI_GetITStatus(EXTI_Line7) != RESET) 
    { 
			if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7))
			{
				LED4_ON;
			  WR_ON;
				USART2_SendStr("ATD18603551718;\r\n");			}
      else 
			{
				LED4_OFF;
				WR_OFF;
			}
			EXTI_ClearFlag(EXTI_Line7);      
      EXTI_ClearITPendingBit(EXTI_Line7);
    }
		
 }
/***************************************************************************
功  能: 拍照并通过彩信发送
参  数: 无
返回值: 无
***************************************************************************/ 
void Send_Message()
{
	USART2_SendStr("AT\r\n");
		Delay_ms(500);
		USART2_SendStr("AT+CMMSINIT\r\n");
		Delay_ms(500);
		USART2_SendStr("AT+CMMSCURL=\"mmsc.monternet.com\"\r\n");
		Delay_ms(500);
		USART2_SendStr("AT+CMMSCID=1\r\n");
		Delay_ms(500);
		USART2_SendStr("AT+CMMSPROTO=\"10.0.0.172\",80\r\n");
		Delay_ms(500);
		USART2_SendStr("AT+CMMSSENDCFG=6,3,0,0,2,4\r\n");
		Delay_ms(500);
		USART2_SendStr("AT+SAPBR=3,1,\"Contype\",\"GPRS\"\r\n");
		Delay_ms(500);
		USART2_SendStr("AT+SAPBR=3,1,\"APN\",\"CMWAP\"\r\n");
		Delay_ms(500);
		USART2_SendStr("AT+SAPBR=1,1\r\n");
		Delay_s(2);
		USART2_SendStr("AT+SAPBR=2,1\r\n");
		Delay_ms(500);
		USART2_SendStr("AT+CMMSEDIT=1\r\n");
		Delay_ms(500);
		USART2_SendStr("AT+CMMSDOWN=\"PIC\",153672,40000\r\n");
		Delay_s(1);
		Take_Pic();
		USART2_SendStr("\r\n");
		Delay_s(3);
		USART2_SendStr("AT+CMMSRECP=\"18603551718\"\r\n");
		Delay_s(1);
		USART2_SendStr("AT+CMMSSEND\r\n");
} 
 