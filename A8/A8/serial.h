#ifndef SERIAL_H
#define SERIAL_H

#include <QThread>
#include <QString>
#include <qextserialbase.h>
//#include "posix_qextserialport.h"
#include "win_qextserialport.h"

typedef void (*ComdataArrive)(char *DataBuff,ulong m_ulen);//���ݴ��ݳ�����
class serialThread;
class serial
{
public:
    serial();

private:
//    Posix_QextSerialPort *m_Com;  //��������
    Win_QextSerialPort *m_Com;
    bool m_bIsOpenFlag;         //���ڴ򿪱�־
    serialThread *pThread;

public:
    bool Initcom(ComdataArrive m_pFun,QString m_sCom,BaudRateType m_eBaund,DataBitsType m_eDataBit,ParityType m_eParity,
                  StopBitsType m_eStopbit,long m_lTimeOut,FlowType m_eFlowtype=FLOW_OFF);
    ComdataArrive m_curFun;     //�������ݺ���
    void startSerThread();
    void ReadData();
    void SendData(char * DataBuff,uint m_uSize);//��������
};

/* �����߳�*/
class serialThread:public QThread
{
public:
    serialThread(serial *m_pSerCom);
    serial *pSerCom;
    bool m_bIsRunFlag;
    void run();
    void stopThread();
};

#endif // SERIAL_H
