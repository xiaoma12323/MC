#include "sccb.h"
#include "delay.h"

/***************************************************************************
��  ��: ��ʼ��ģ��SCCB�˿�
��  ��: ��
����ֵ: ��
***************************************************************************/
void SCCB_GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	
  GPIO_InitStructure.GPIO_Pin =  SCCB_SIC_BIT|SCCB_SID_BIT;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		  //GPIO_Mode_AIN = 0x0,     //ģ������
  GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;		  //GPIO_Mode_IN_FLOATING = 0x04, //��������
  GPIO_Init(GPIOB, &GPIO_InitStructure);				  //GPIO_Mode_IPD = 0x28,    //��������
														  //GPIO_Mode_IPU = 0x48,    //��������
														  //GPIO_Mode_Out_OD = 0x14, //��©���
														  //GPIO_Mode_Out_PP = 0x10,  //�������
														  //GPIO_Mode_AF_OD = 0x1C,   //��©����
														  //GPIO_Mode_AF_PP = 0x18    //���츴�� 
}

/***************************************************************************
��  ��: ����SID�ӿ�Ϊ���ģʽ
��  ��: ��
����ֵ: ��
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
��  ��: ����SID�ӿ�Ϊ����ģʽ
��  ��: ��
����ֵ: ��
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
��  ��: Start���SCCB��ʼ�ź�
��  ��: ��
����ֵ: ��
***************************************************************************/
void startSCCB(void)
{
    SCCB_SID_H();     //�����߸ߵ�ƽ
    Delay_us(500);

    SCCB_SIC_H();	   //��ʱ���߸ߵ�ʱ���������ɸ�����
    Delay_us(500);
 
    SCCB_SID_L();
    Delay_us(500);

    SCCB_SIC_L();	 //�����߻ָ��͵�ƽ��������������Ҫ
    Delay_us(500);


}

/***************************************************************************
��  ��: Stop���SCCB��ֹ�ź�
��  ��: ��
����ֵ: ��
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
��  ��: noAck,����������ȡ�е����һ����������
��  ��: ��
����ֵ: ��
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
��  ��: д��һ���ֽڵ����ݵ�SCCB
��  ��: д������
����ֵ: 0(д��ʧ��), 1(д��ɹ�)
***************************************************************************/
u8 SCCBwriteByte(u8 m_data)
{
	u8 i,tem;

	for(i=0;i<8;i++) //ѭ��8�η�������
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
	SCCB_SID_IN;/*����SDAΪ����*/
	Delay_us(500);
	SCCB_SIC_H();	
	Delay_us(100);
	if(SCCB_SID_STATE){tem=0;}   //SDA=1����ʧ�ܣ�����0}
	else {tem=1;}   //SDA=0���ͳɹ�������1
	SCCB_SIC_L();	
	Delay_us(500);	
  SCCB_SID_OUT;/*����SDAΪ���*/
	return (tem);  
}


/***************************************************************************
��  ��: ��һ���ֽ����ݶ�ȡ���ҷ���
��  ��: ��
����ֵ: ��ȡ��������
***************************************************************************/
u8 SCCBreadByte(void)
{
	u8 read,j;
	read=0x00;
	
	SCCB_SID_IN;/*����SDAΪ����*/
	Delay_us(500);
	for(j=0;j<8;j++) //ѭ��8�ν�������
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
