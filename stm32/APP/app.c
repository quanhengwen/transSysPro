#include "includes.h"

/* ����� */
#define	SH		(0x01 << 0)					   //�����С����ı���
#define	SG		(0x01 << 1)					   //�����G�εı���
#define	SF		(0x01 << 2)					   //�����F�εı���
#define	SE		(0x01 << 3)					   //�����E�εı���
#define	SD		(0x01 << 4)					   //�����D�εı���
#define	SC		(0x01 << 5)					   //�����C�εı���
#define	SB		(0x01 << 6)					   //�����B�εı���
#define	SA		(0x01 << 7)					   //�����A�εı���
const u8 LEDData[] =
{
	SA |SB |SC | SD |SE |SF,		    // 0�ı���
	SB | SC,						    //01�ı���
	SA |SB |SG | SE | SD,			    //02�ı���
	SA |SB |SC | SD | SG,			    //03�ı���
	SB |SF | SG | SC,				    //04�ı���
	SA |SF |SG |SC  | SD,			    //05�ı���
	SA |SC | SD | SE | SG | SF,	        //06�ı���
	SA | SB| SC,					    //07�ı���
	SA |SB |SC | SD | SE | SF | SG,     //08�ı���
	SA |SB |SC|SD | SF | SG	,	        //09�ı���	
};

/* ADC */
u8 ADC_ConvertedValueLocal;        
extern __IO uint16_t ADC_ConvertedValue;
extern OS_EVENT* adc_MBOX;

/* ���� */
extern OS_EVENT* key_SEM;
extern OS_EVENT* keyDis_SEM;

/* ȫ�ֱ��� */
unsigned int sum=0;

static OS_STK led_task_stk[LED_TASK_STK_SIZE];	//����ջ
static OS_STK usart1_task_stk[USART1_TASK_STK_SIZE];	
static OS_STK ledDis_task_stk[LEDDIS_TASK_STK_SIZE];
static OS_STK adc1_task_stk[ADC1_TASK_STK_SIZE];
static OS_STK exti_task_stk[EXTI_TASK_STK_SIZE];
static OS_STK calculate_task_stk[CALCULATE_TASK_STK_SIZE];

void Task_START(void *p_arg)
{
	(void)p_arg;	//'p_arg'û���õ�����ֹ����������
	adc_MBOX = OSMboxCreate((void *)0);
	key_SEM = OSSemCreate(0);
	keyDis_SEM = OSSemCreate(0);

	OSTaskCreate(Task_LED,(void *)0,
		&led_task_stk[LED_TASK_STK_SIZE-1],LED_TASK_PRIO);

	OSTaskCreate(Task_USART1,(void *)0,
		&usart1_task_stk[USART1_TASK_STK_SIZE-1],USART1_TASK_PRIO);

	OSTaskCreate(Task_LEDDIS,(void *)0,
		&ledDis_task_stk[LEDDIS_TASK_STK_SIZE-1],LEDDIS_TASK_PRIO);

	OSTaskCreate(Task_ADC1,(void *)0,
		&adc1_task_stk[ADC1_TASK_STK_SIZE-1],ADC1_TASK_PRIO);

	OSTaskCreate(Task_EXTI,(void *)0,
		&exti_task_stk[EXTI_TASK_STK_SIZE-1],EXTI_TASK_PRIO);

	OSTaskCreate(Task_CALCULATE,(void *)0,
		&calculate_task_stk[CALCULATE_TASK_STK_SIZE-1],CALCULATE_TASK_PRIO);

	while(1)
	{
		OSTimeDlyHMSM(0,0,0,10);
	}
}

void Task_LED(void *p_arg)
{
	(void)p_arg;	//'p_arg'û���õ�����ֹ����������
	while(1)
	{
		LED1( ON );
		OSTimeDlyHMSM(0,0,0,500);
		LED1( OFF );

		LED2( ON );
		OSTimeDlyHMSM(0,0,0,500);
		LED2( OFF );

		LED3( ON );
		OSTimeDlyHMSM(0,0,0,500);
		LED3( OFF );
	}
}

void Task_USART1(void *p_arg)
{
	(void)p_arg;	//'p_arg'û���õ�����ֹ����������
	while(1)
	{	
		unsigned char errkey;
		unsigned char num,err;

		OSSemPend(key_SEM,0,&errkey);
		num = *(unsigned char*)OSMboxPend(adc_MBOX,0,&err);
		printf(" hello: %d",num);
		OSTimeDlyHMSM(0,0,0,500);
	}
}

void Task_LEDDIS(void *p_arg)
{
	(void)p_arg;	//'p_arg'û���õ�����ֹ����������

	while(1)
	{
		u8 a,b,c,d;						//����a,b,c,d�ĸ����������������ܸ���λ��ֵ

		a = sum /1000;						//ǧλֵ����a
		b = sum % 1000 /100;				//��λֵ����b
		c = sum % 1000 % 100 /10;			//ʮλֵ����c
		d = sum % 1000 % 100 % 10;		    //��λֵ����d

		GPIO_SetBits(GPIOB , GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);     
//		Refresh_LED(1234);
		GPIO_ResetBits(GPIOB , GPIO_Pin_12);			//�����0λλѡ�����������õͣ�ʹ��λ0
		data_shift(LEDData[d]);			//����λ��ֵ��Ӧ�ı���������������ݹܽ�
		OSTimeDlyHMSM(0,0,0,3);

		GPIO_SetBits(GPIOB , GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);     
//		Refresh_LED(1234);
		GPIO_ResetBits(GPIOB , GPIO_Pin_13);			//�����0λλѡ�����������õͣ�ʹ��λ0
		data_shift(LEDData[c]);			//����λ��ֵ��Ӧ�ı���������������ݹܽ�
		OSTimeDlyHMSM(0,0,0,3);

		GPIO_SetBits(GPIOB , GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);     
//		Refresh_LED(1234);
		GPIO_ResetBits(GPIOB , GPIO_Pin_14);			//�����0λλѡ�����������õͣ�ʹ��λ0
		data_shift(LEDData[b]);			//����λ��ֵ��Ӧ�ı���������������ݹܽ�
		OSTimeDlyHMSM(0,0,0,3);

		GPIO_SetBits(GPIOB , GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);     
//		Refresh_LED(1234);
		GPIO_ResetBits(GPIOB , GPIO_Pin_15);			//�����0λλѡ�����������õͣ�ʹ��λ0
		data_shift(LEDData[a]);			//����λ��ֵ��Ӧ�ı���������������ݹܽ�
		OSTimeDlyHMSM(0,0,0,3);
	}
}

void Task_ADC1(void *p_arg)
{
	(void)p_arg;	//'p_arg'û���õ�����ֹ����������
	while(1)
	{
		 ADC_ConvertedValueLocal =(float) ADC_ConvertedValue/4096*10;
//		 OSMboxPost(adc_MBOX,(void *)&ADC_ConvertedValueLocal);
		 OSTimeDlyHMSM(0,0,0,500);
	}
}

void Task_EXTI(void *p_arg)
{
	(void)p_arg;	//'p_arg'û���õ�����ֹ����������
	while(1)
	{
		OSTimeDlyHMSM(0,0,0,500);
	}
}

void Task_CALCULATE(void *p_arg)
{
	(void)p_arg;	//'p_arg'û���õ�����ֹ����������

	while(1)
	{
		unsigned char errkey;
		OSSemPend(keyDis_SEM,0,&errkey);

		sum=sum+ADC_ConvertedValueLocal;
		OSMboxPost(adc_MBOX,(void *)&sum);
		OSTimeDlyHMSM(0,0,0,500);
	}
}
