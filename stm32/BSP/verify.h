#ifndef __Verify_H__
#define __Verify_H__


extern unsigned char  crc16L,crc16H;

void CRC16(unsigned char *MSG,unsigned short usDataLen);     //CRC����
unsigned char Verify_DATA(unsigned char *str,unsigned short strlen);	//�������У��
unsigned char SUM_verify(unsigned char *buf,unsigned char leng); //��У���������
#endif  
