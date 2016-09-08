/***************************Copyright BestFu 2014-05-14*************************
��	����	I2C.c
˵	����	I2Cģʽͨ������
��	�룺	Keil uVision4 V4.54.0.0
��	����	v1.0
��	д��	Unarty
��	�ڣ�	2014.05.04  
�ޡ��ģ�	����
*******************************************************************************/
#include "EEPROM_I2C.h"
#include "gpio_api.h"   // mbed

//Private macro define
#define WaitTime	3//2		//�ȴ�ʱ�� 
#define NOP			5		//����ʱ��

//Private function declaration
static void Delay_nNOP(u16 n);
static void EEPROM_I2C_Start(void);
static void  EEPROM_I2C_Stop(void);
static void  EEPROM_I2C_ACK(u8 ack);
static u8 EEPROM_I2C_WaitAck(void);
static u8 EEPROM_I2C_SendB_Ack(u8 data);
static u8 EEPROM_I2C_RecvB_Ack(u8 ack);

gpio_t gpio_eeprom_scl;
gpio_t gpio_eeprom_sda;

/*******************************************************************************
�� �� ����	Delay_nNOP
����˵���� 	��ʱn���ղ���
��	  ���� 	n-�ղ����ĸ���
�� �� ֵ��	void
*******************************************************************************/
static void Delay_nNOP(u16 n)
{
	u16 i = 0;

	for ( ; n > 0; n--)
	{
		for(i = NOP; i > 0; i--)
		{}
	}
}


/*******************************************************************************
**��    ���� IIC_InitPort
**��    �ܣ� IIC�ӿڳ�ʼ��
**��    ���� void
**��    �أ� void
*******************************************************************************/
void EEPROM_IIC_InitPort(void)
{                        

    
      printf("\n IIC_InitPort");
    // Init IIC_1_SCL_PIN
      gpio_init(&gpio_eeprom_scl, EEPROM_SCL_PIN);
      gpio_dir(&gpio_eeprom_scl, PIN_OUTPUT);    // Direction: Output
      gpio_mode(&gpio_eeprom_scl, PullNone);     // No pull
      
      gpio_write(&gpio_eeprom_scl, 1);


      // Init IIC_1_SDA_PIN
      gpio_init(&gpio_eeprom_sda, EEPROM_SDA_PIN);
      gpio_dir(&gpio_eeprom_sda, PIN_OUTPUT);    // Direction: Output
      gpio_mode(&gpio_eeprom_sda, PullNone);     // No pull
    
      gpio_write(&gpio_eeprom_sda, 1);
}


/*******************************************************************************
�� �� ����	I2C_Init
����˵���� 	I2C�ĳ�ʼ��
��	  ���� 	void
�� �� ֵ��	void
*******************************************************************************/
void EEPROM_I2C_Init(void)
{
    EEPROM_IIC_InitPort();

    EEPROM_SDA_OUT;
    EEPROM_SCL_OUT;
    
    EEPROM_SCL1;
    EEPROM_SDA1;
}

/*******************************************************************************
�� �� ����	I2C_Start
����˵���� 	I2C�Ŀ�ʼ
��	  ���� 	void
�� �� ֵ��	void
*******************************************************************************/
static void EEPROM_I2C_Start(void)
{
    EEPROM_SDA1;         
    EEPROM_SCL1;         
    Delay_nNOP(WaitTime);
    EEPROM_SDA0;
    Delay_nNOP(WaitTime);
    EEPROM_SCL0;
}

/*******************************************************************************
�� �� ����	I2C_Stop
����˵���� 	I2C��ֹͣ
��	  ���� 	void
�� �� ֵ��	void
*******************************************************************************/
static void EEPROM_I2C_Stop(void)
{
    EEPROM_SDA0;
    Delay_nNOP(WaitTime);
    EEPROM_SCL1;
    Delay_nNOP(WaitTime);
    EEPROM_SDA1;
    Delay_nNOP(WaitTime);
}

/*******************************************************************************
�� �� ����	I2C_ACK
����˵���� 	I2C������Ӧ
��	  ���� 	ack-��Ӧ��־��0��ʾ������ȡ��1��ʾ��ȡ����
�� �� ֵ��	void
*******************************************************************************/
static void EEPROM_I2C_ACK(u8 ack)
{
    EEPROM_SCL0;
    Delay_nNOP(WaitTime);
    if (ack) 
    {
        EEPROM_SDA1;
    }
    else 
    {
        EEPROM_SDA0;
    }
    Delay_nNOP(WaitTime);
    EEPROM_SCL1;
    Delay_nNOP(WaitTime);
    EEPROM_SCL0;
}

/*******************************************************************************
�� �� ����	I2C_WaitAck
����˵���� 	I2C�ȴ���������Ӧ
��	  ���� 	void
�� �� ֵ��	u8-��Ӧ���ݣ�1��ʾ�л�Ӧ��0��ʾ��ʱ���޻�Ӧ
*******************************************************************************/
static u8 EEPROM_I2C_WaitAck(void)
{
	u16 OutTime=0;
	
	EEPROM_SDA_IN;       //SDA����Ϊ����  
	
        Delay_nNOP(WaitTime);	   
	EEPROM_SCL1;
        Delay_nNOP(WaitTime); 
	while (EEPROM_Get_SDA)
	{
		if ((++OutTime) > 500)
		{
			EEPROM_SDA_OUT;
			return FALSE;
		}
	}
	EEPROM_SCL0;
        Delay_nNOP(WaitTime); 
	EEPROM_SDA_OUT;
	Delay_nNOP(WaitTime);
	
	return TRUE;
}

/*******************************************************************************
�� �� ����	I2C_SendB_Ack
����˵���� 	I2C����һ���ֽ����ݲ��ҵȴ���Ӧ
��	  ���� 	data-��Ҫ���͵��ֽ�
�� �� ֵ��	u8-��Ӧ���ݣ�1��ʾ�л�Ӧ��0��ʾ��ʱ���޻�Ӧ
*******************************************************************************/
static u8 EEPROM_I2C_SendB_Ack(u8 data)
{
	u8 i;
	
	EEPROM_SCL0;
	Delay_nNOP(WaitTime);
	for (i = 0; i < 8; i++)
	{
		if(data&0x80)
		  EEPROM_SDA1;
		else
		  EEPROM_SDA0;
		data <<= 1;
		Delay_nNOP(WaitTime);
		EEPROM_SCL1;
		Delay_nNOP(WaitTime);
		EEPROM_SCL0;
		Delay_nNOP(WaitTime);
	}

	return EEPROM_I2C_WaitAck();
}

/*******************************************************************************
�� �� ����	I2C_RecvB_Ack
����˵���� 	I2C����һ���ֽ����ݲ��һ�Ӧack
��	  ���� 	ack-��Ӧ��Ϣ��0��ʾ������ȡ��1-��ʾֹͣ��ȡ
�� �� ֵ��	u8-�յ�������
*******************************************************************************/
static u8 EEPROM_I2C_RecvB_Ack(u8 ack)
{
	u8 i,rcvdata=0;
	
	EEPROM_SDA_IN; 	  //SDA����Ϊ����  
	for (i = 0; i < 8; i++)
	{
		EEPROM_SCL0;
		Delay_nNOP(WaitTime);
		EEPROM_SCL1;
		Delay_nNOP(WaitTime);
		rcvdata <<= 1;

		if(EEPROM_Get_SDA)
		{
			rcvdata++;
		}
		Delay_nNOP(WaitTime);
	}
	EEPROM_SCL0;
	Delay_nNOP(WaitTime);
	EEPROM_SDA_OUT;
	Delay_nNOP(WaitTime);
	EEPROM_I2C_ACK(ack);
	
	return (rcvdata);
}

/*******************************************************************************
�� �� ����  IIC_GetState
����˵����  IICͨ���豸״̬
��	  ����  add:	�豸��ַ
			overtime:�ȴ���ʱʱ��
  ����ֵ:  TRUE(��æ)/FALSE(æ)
*******************************************************************************/
u8 EEPROM_IIC_GetState(u8 sla, u32 overTime)
{
	do 
	{	
		EEPROM_I2C_Start();
	}while((!EEPROM_I2C_SendB_Ack(sla)) && --overTime);
	
	EEPROM_I2C_Stop();
	
	return (overTime ?  TRUE : FALSE);
}

/*******************************************************************************
�� �� ����	IIC_Read
����˵���� 	IIC���ݶ�ȡ
��	  ���� 	sla-��������ַ
            suba-�����׵�ַ
            data-��ȡ�������ݴ���ڸ�ָ��
            len-���ȣ���಻�ܳ���һҳ��С
�� �� ֵ��	1��ʾ�ɹ���0��ʾʧ��
*******************************************************************************/
u8 EEPROM_IIC_Read(u8 sla, u16 suba, u8 *data, u8 len)
{
	EEPROM_I2C_Start();
	if (EEPROM_I2C_SendB_Ack(sla))
	{	
		if (EEPROM_I2C_SendB_Ack((u8)(suba>>8)))
		{
			if (EEPROM_I2C_SendB_Ack((u8)(suba)))
			{
				EEPROM_I2C_Start();
				if (EEPROM_I2C_SendB_Ack(sla | 0x01))
				{
					for (; len > 1; len--)
					{
						*data++ = EEPROM_I2C_RecvB_Ack(0);
					}
					len--;
					*data = EEPROM_I2C_RecvB_Ack(1);
				}
			}
		}
	}
	EEPROM_I2C_Stop();
	
	return (len ? FALSE : TRUE);
}

/*******************************************************************************
�� �� ����	I2C_Write
����˵���� 	��һ������д�뵽EEPROM�е�ָ��λ��
��	  ���� 	sla-��������ַ
            suba-�����׵�ַ
            data-Ҫд������ݴ���ڸ�ָ��
            len-���ȣ���಻�ܳ���һҳ��С
�� �� ֵ��	1��ʾ�ɹ���0��ʾʧ��
*******************************************************************************/
u8 EEPROM_IIC_Write(u8 sla, u16 suba, u8 *data, u8 len)
{
//	WP_UN;
	EEPROM_I2C_Start();
	if(EEPROM_I2C_SendB_Ack(sla))
	{
		if (EEPROM_I2C_SendB_Ack((u8)(suba>>8)))
		{
			if (EEPROM_I2C_SendB_Ack((u8)(suba)))
			{
				for (; len > 0; len--)
				{
					if (!EEPROM_I2C_SendB_Ack(*data++))
					{
						break;
					}
				}
			}
		}
    }
	EEPROM_I2C_Stop();
//	WP_EN;
	
	return (len ? FALSE : TRUE);
}

/*******************************************************************************
�� �� ����	I2C_WriteSame
����˵���� 	��һ����ͬ�����ݣ�д�뵽EEPROM�е�ָ��λ��
��	  ���� 	sla-��������ַ
            suba-�����׵�ַ
            data-Ҫд�������
            len-���ȣ���಻�ܳ���һҳ��С
�� �� ֵ��	1��ʾ�ɹ���0��ʾʧ��
*******************************************************************************/
u8 EEPROM_IIC_WriteSame(u8 sla, u16 suba, u8 data, u8 len)
{
//	WP_UN;
	EEPROM_I2C_Start();
	if (EEPROM_I2C_SendB_Ack(sla))
	{
		if (EEPROM_I2C_SendB_Ack((u8)(suba>>8)))
		{
			if (EEPROM_I2C_SendB_Ack((u8)(suba)))
			{
				for	(; len > 0; len--)
				{
					if (!EEPROM_I2C_SendB_Ack(data))
					{
						break;
					}
				}
			}
		}
    }
	EEPROM_I2C_Stop();
//	WP_EN;
	
	return (len ? FALSE : TRUE);
}
/**************************Copyright BestFu 2014-05-14*************************/
