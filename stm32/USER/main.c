/**
  ******************************************************************************
  * ����    ����λ��_
  *         
  * ʵ��ƽ̨��stm32f103c8t6
  * Ӳ�����ӣ�------------------------
  *          | PA9  - USART1(Tx)      |
  *          | PA10 - USART1(Rx)      |
  *           ------------------------
  * ��汾  ��ST3.5.0
  * ����    ��zhangwj   
  ******************************************************************************
  *  
  * �����ǵ� main.c �ļ���Ҫ�� stdio.h ���ͷ�ļ�������������Ҫ��
  * ������������һ��ѡ�� Use MicroLIB (ʹ��΢��)���� Options for target��
  * ���΢���� keil MDK ΪǶ��ʽӦ���������� C �⣬����Ҫ�Ⱦ��п⣬�����ض���
  * ��ѡʹ��֮�����ǾͿ���ʹ�� printf() ��������ˡ�
  *
  **/	
/* Includes ------------------------------------------------------------------*/
#include "includes.h"

static OS_STK startup_task_stk[STARTUP_TASK_STK_SIZE];	//����ջ

int main(void)
{
	BSP_Init();
	OSInit();
	OSTaskCreate(Task_START,(void *)0,
		&startup_task_stk[STARTUP_TASK_STK_SIZE-1],STARTUP_TASK_PRIO);

	OSStart();
	return 0;
}


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
