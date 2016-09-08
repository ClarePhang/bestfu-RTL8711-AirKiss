/***************************Copyright BestFu 2014-05-14*************************
��   ����  Fifo.h
˵   ����  ͨ�ö��д���ͷ�ļ�
��   �룺  Keil uVision4 V4.54.0.0
��   ����  v1.0
��   д��  Unarty
��   �ڣ�  2013-09-03
��  ��:     ����
*******************************************************************************/
#ifndef __FIFO_H
#define __FIFO_H
#include "freertos_service.h"


/***********************************�궨��************************************/
/*�ݴ��*/
#define ERRR(conf, ret)      do              \
                            {                \
                                if (conf)    \
                                {            \
                                    ret;     \
                                }            \
                            } while(0) 
                            
#define MIN(a, b)           ((a) > (b) ? (b) : (a))

/*******************************�������ݽṹ**********************************/
struct fifo_data
{
    u8   *data;
    u32   size;
    u32   front;
    u32   rear;
};

/*********************************��������*************************************/
u8 fifo_Init(struct fifo_data *head, u8 *buf, u32 len); //���г�ʼ��
void fifo_Rst(struct fifo_data *head);                        //��ն���
u32 fifo_validSize(struct fifo_data *head);     //���ݿ��ÿռ��С
u8 fifo_empty(struct fifo_data *head);        //�ӿ��ж�
u8 fifo_puts(struct fifo_data *head, u8 *data, u32 len);
u8 fifo_gets(struct fifo_data *head, u8 *data, u32 len);
u8 fifo_putc(struct fifo_data *head, u8 data);
u8 fifo_getc(struct fifo_data *head, u8 *data);
u32 fifo_find(struct fifo_data *head, const u8 *data, u32 len);
u8 fifo_cmp(const struct fifo_data *fifo, u32 seat, const u8 *cmp, u32 cmpsize);
                            
#endif //queue.h end
/**************************Copyright BestFu 2014-05-14*************************/
