#include "includes.h"
/*
 * ��������SysTick_Init
 * ����  ������SysTick��ʱ��
 */
void SysTick_Init(void)
{
	SysTick_Config(SystemCoreClock/OS_TICKS_PER_SEC);	//��ʼ����ʹ��SysTick��ʱ��
}

/*
 * ��������BSP
 * ����  ��ʱ�ӳ�ʼ����Ӳ����ʼ��
 */
 void BSP_Init(void)
{
 	SystemInit();					/* ����ϵͳʱ��Ϊ72MHz */
	SysTick_Init();					/* ��ʼ����ʹ��SysTick��ʱ�� */
	LED_GPIO_config();				/* LED�˿ڳ�ʼ�� */
	USART1_Config();				/* ���� */
	LEDDIS_GPIO_Configuration();	/* ����� */
	ADC1_Init();					/* ADC1 */
}
