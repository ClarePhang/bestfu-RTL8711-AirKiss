/***************************Copyright BestFu 2014-05-14*************************
��	����	Water.c
˵	����	ˮλ�뽽ˮ
��	�룺	IAR
��	����	v1.0
��	д��	xjx
��	�ڣ�	
�ޡ��ģ�	����
*******************************************************************************/
#include "Water.h"

#include "FreeRTOS.h"
#include "task.h"
#include "gpio_api.h" 
#include "pwmout_api.h"   // mbed

#include "semphr.h"

#include "SHT20.h"
#include "Fault.h"


xTaskHandle manual_watering_handle;




gpio_t gpio_water;

pwmout_t pwm_watering;//��ˮPWM���

volatile u8 g_water_level = NO_WATER;

void stop_watering(void);
void start_watering(float period,float pulse_width);



void WaterGPIOPortInit(void)
{
      printf("\n WaterGPIOPortInit");
    // Init gpio_water pin
      gpio_init(&gpio_water, WATER_DTC_PIN);
      gpio_dir(&gpio_water, PIN_INPUT);    // Direction: Output
      gpio_mode(&gpio_water, PullUp);     // pull up
      
     
}
u8 GetWaterLevel(void)
{
   // u8 tmp = gpio_read(&gpio_water);
    return (u8)gpio_read(&gpio_water);//tmp ? NO_WATER : WATER_FULL;
}



void task_get_water_level(void *param)
{
    u8 waterLevel = 0;
    WaterGPIOPortInit();
    vTaskDelay(100);
    
    while(1)
    {
        waterLevel = GetWaterLevel();
        
        
        taskENTER_CRITICAL();
        {
            g_water_level = waterLevel;
        }
        taskEXIT_CRITICAL();
        
        
        if(NO_WATER == waterLevel)
        {
            printf("No water \n");
        }
        else if(WATER_FULL == waterLevel)
        {
            printf("Water Full \n");
        }
        else
        {
            printf("Get water level error");
        }
        vTaskDelay(1000);
    }
}


/*******************************************************************************
**��    ���� watering_init
**��    �ܣ� ��ˮ
**��    ���� u8 cnt:���ý�ˮ�����ĳ�ʼֵ
**��    �أ� u8��
*******************************************************************************/
void watering_init(void)
{
    pwmout_init(&pwm_watering, WATERING_PIN);
//    pwmout_period(&pwm_watering, WATERING_PWM_PERIOD);            
//    pwmout_pulsewidth(&pwm_watering, 0);
        
}
/*******************************************************************************
**��    ���� StartWatering
**��    �ܣ� ��ˮ
**��    ���� float period:pwm����
            float pulse_width:pwm���
**��    �أ�void
*******************************************************************************/
void start_watering(float period,float pulse_width)
{   
    watering_init();
    pwmout_period(&pwm_watering, period);            
    pwmout_pulsewidth(&pwm_watering, pulse_width);   
}


/*******************************************************************************
**��    ���� StartWatering
**��    �ܣ� ��ˮ
**��    ���� void
**��    �أ� u8��
*******************************************************************************/
void stop_watering(void)
{
    pwmout_pulsewidth(&pwm_watering, 0);
    gpio_dir(&pwm_watering, PIN_OUTPUT);    // Direction: Output
    gpio_mode(&pwm_watering, PullNone);     // PullNone
    gpio_write(&pwm_watering, 0);
    
}


void example_watering_test(void *param)
{
    vTaskDelay(1000);
    printf("\n entering example_watering_test \n");
    while(1)
    {
        watering_init();
        vTaskDelay(100);
        start_watering(WATERING_PWM_PERIOD,WATERING_PWM_PULSE_WIDTH);
        vTaskDelay(200);
        stop_watering();
        vTaskDelay(1000);
    }
}



/*******************************************************************************
**��    ���� manual_watering_task
**��    �ܣ� �ֶ���ˮ
**��    ���� void *param����ˮ��Ϣ
**��    �أ� ��
*******************************************************************************/
void manual_watering_task(void *param)
{

    struct watering_t *watering_param = (struct watering_t*)param;
    u8 water_level = 0;
    u8 watering_cnt = 0;
    
    printf("entering manual watering task \n");
    watering_init();

    vTaskDelay(100);
  
    while(watering_cnt < watering_param->cnt)
    {



        //if(g_water_level == WATER_FULL)
        {

            printf("start watering \n");
            start_watering(watering_param->pwm_period,watering_param->pwm_pulsewidth);

            vTaskDelay(watering_param->watering_period);
            stop_watering();
            printf("stop watering \n");

            
            vTaskDelay(watering_param->watering_interval);
            watering_cnt++;
                
        }
        //else//ˮλ�쳣
        {   //stop_watering();      
            //break;
        }
    }
    
    vTaskDelete(NULL);
}





//extern xSemaphoreHandle sht20_handle;

void auto_watering_task(void *param)
{
    u8 humidity_tmp = 0;
    for(;;)
    {
        //xSemaphoreTake(sht20_handle,portMAX_DELAY);
        taskENTER_CRITICAL();
        {
            printf("temp is %d, humidity is %d \n", sht20.temp,sht20.humidity);
            humidity_tmp = sht20.humidity;
        }
        taskEXIT_CRITICAL();
        //xSemaphoreGive(sht20_handle);
        
        if(humidity_tmp < p_temp_humidity_set->humidity_min)
        {
            start_watering(WATERING_PWM_PERIOD,WATERING_PWM_PULSE_WIDTH);
            vTaskDelay(3000);           //��ˮ3��
        }       
     
        vTaskDelay(300000);//5���Ӽ��һ��
    }
    
}






/**************************Copyright BestFu 2014-05-14*************************/