#ifndef __MYCONFIG_H__
#define __MYCONFIG_H__

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

/* eeprom */
#define  EEP_Firstpage      0x00
#define  EEP_Secondpage     0x01
//u8 I2c_Buf_Write[256];
u8 I2c_Buf_Read[256];
u8 I2c_Buf_Write[256];
//u8 *I2c_Buf_Read;

/* ȫ�ֱ��� */
unsigned int sum=0;
struct carData car1={ "����" , "0123456789" , "��E12345" , 10 };

#endif
