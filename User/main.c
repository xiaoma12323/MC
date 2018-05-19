
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include <stdio.h>
#include "system.h"

u16 i=0;

int main(void)
{
	System_Init();      //ϵͳʱ�ӳ�ʼ��
	IO_Init();					//I/O�˿ڳ�ʼ��		
	Delay_Init(72);			//��72Mhzϵͳʱ�ӳ�ʼ����ʱ
	USART1_Init();			//����1��ʼ��
	USART2_Init();			//����2��ʼ��
	EXTI7_Init();
	Gsm_Reset();				//����GSMģ��
  Delay_s(3);

	USART2_SendStr("AT\r\n");
	
	LED2_OFF;						//�ر�LED��
	LED3_OFF;
	LED4_OFF;

	
	while(1)
	{
		if(GsmRev[0]!=0)
		{ 
			
					if(GsmFlag==1&&strstr(GsmRev,"18603551718"))
					{
						if(strstr(GsmRev,"#1_ON"))
						{
							S1_ON;
						}
						else if(strstr(GsmRev,"#1_OFF"))
						{
							S1_OFF;
						}
						else if(strstr(GsmRev,"#2_ON"))
						{
							S2_ON;
						}
						else if(strstr(GsmRev,"#2_OFF"))
						{
							S2_OFF;
						}
						else if(strstr(GsmRev,"#3_ON"))
						{
							S3_ON;
						}
						else if(strstr(GsmRev,"#3_OFF"))
						{
							S3_OFF;
						}
						else if(strstr(GsmRev,"PIC"))
						{

							Send_Message();
						}
					}
			else if(strstr(GsmRev,"+CLCC"))
			{
					LED3_ON;
				if(strstr(GsmRev,"18603551718")) 
				{
					Delay_s(2);
				  USART2_SendStr("ATA\r\n");
				}
			}
			else if(strstr(GsmRev,"NO CARRIER"))
			{
					LED3_OFF;
			}		
			GsmRevClean();
			GsmFlag=0;
		}
	}
}