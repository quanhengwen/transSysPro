#include "includes.h"

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
