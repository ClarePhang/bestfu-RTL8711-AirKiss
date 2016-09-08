/***************************Copyright BestFu 2014-05-14*************************
��	����	Fault.c
˵	����	�쳣��������ˮλ�쳣����ʪ���쳣�������쳣��
��	�룺	IAR
��	����	v1.0
��	д��	xjx
��	�ڣ�	
�ޡ��ģ�	����
*******************************************************************************/
#include "FreeRTOS.h"
#include "task.h"
#include "Fault.h"
#include "Water.h"

#include "SHT20.h"
#include "Kalman_Filter.h"



void upload_fault_state(u32 fault)
{    
    if(fault &(1<<WATER_LEVEL_LOW_FAULT))//��ˮλ
    {
        
    }
    if(fault &(1<<TEMP_TOO_LOW_FAULT))//�¶ȹ���
    {
        
    }
    if(fault &(1<<TEMP_TOO_HIGH_FAULT))//�¶ȹ���
    {
        
    }
    if(fault &(1<<HUMIDITY_TOO_LOW_FAULT))//ʪ�ȹ���
    {
        
    }
    if(fault &(1<<HUMIDITY_TOO_HIGH_FAULT))//ʪ�ȹ���
    {
        
    }   
}





void task_fault_detection(void *param)
{
    u32 fault = 0;

    
    int temp_tmp = 0;
    u8 humidity_tmp = 0;
    
    u8 temp_min;        //�¶�����
    u8 temp_max;        //�¶�����
    u8 humidity_min;    //ʪ������
    u8 humidity_max;    //ʪ������
    
    u8 cnt = 0;
    u8 water_level_tmp = 0;
    
    WaterGPIOPortInit();
    vTaskDelay(100);

    
    

    
    for(;;)
    {
      
       

        
        get_temp_and_humidity(&temp_tmp,&humidity_tmp);
        
        taskENTER_CRITICAL();
        {//����ȫ�ֱ���
            humidity_tmp = sht20.humidity;
            temp_tmp = sht20.temp;
            water_level_tmp = g_water_level;
            temp_min = p_temp_humidity_set->temp_min;
            temp_max = p_temp_humidity_set->temp_max;
            humidity_min = p_temp_humidity_set->humidity_min;
            humidity_max = p_temp_humidity_set->temp_max;           
        }
        taskEXIT_CRITICAL();
        
        {//��ӡ����
            //printf("temperature is %d, humidity is %d \n",temp_tmp,humidity_tmp);
            
            if(water_level_tmp == NO_WATER)
            {
                printf("no water \n");
            
            }
            else
            {
                printf("water full \n");
            }
        }
        
        
        
        if(NO_WATER == water_level_tmp)
        {
            //printf("no water \n");
            fault |= 1 << WATER_LEVEL_LOW_FAULT;
        }
        
        if(temp_tmp < temp_min)//�¶ȹ���
        {
            //printf("temperature is too low , %d \n",temp_tmp);
            fault |= 1 << TEMP_TOO_LOW_FAULT;
            
        }
        if(temp_tmp > temp_max)//�¶ȹ���
        {
            //printf("temperature is too high , %d \n",temp_tmp);
            fault |= 1 << TEMP_TOO_HIGH_FAULT;
            
        }
        if(humidity_tmp < humidity_min)//ʪ�ȹ���
        {
            //printf("humidity is too low , %d \n",humidity_tmp);
            fault |= 1 << HUMIDITY_TOO_LOW_FAULT;
            
        }
        if(humidity_tmp > humidity_max)//ʪ�ȹ���
        {
            //printf("humidity is too high , %d \n",humidity_tmp);
            fault |= 1 << HUMIDITY_TOO_HIGH_FAULT;            
        }
        
        
        if(fault)
        {
            upload_fault_state(fault);
            fault = 0;
        }
        
        vTaskDelay(950);
        
        
    }
}
















/**************************Copyright BestFu 2014-05-14*************************/