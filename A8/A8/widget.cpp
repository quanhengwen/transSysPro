#include "widget.h"
#include "ui_widget.h"
#include <QDateTime>
#include <QTimer>
#include <QDebug>

Widget *pMainWnd = NULL;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    mageSecretInput = new secretInput;
    workW = new workwidget(0);
    magWorkW = new workwidget(1);

    serialManage();

    QTimer *WorkTimer = new QTimer;
    WorkTimer->start(1000);
    connect(WorkTimer,SIGNAL(timeout()),this,SLOT(updatetime()));
    connect(this,SIGNAL(switchToWorkWSignal()),this,SLOT(workWShow()));
    connect(this,SIGNAL(switchToSecretSignal()),this,SLOT(toSecret()));
    connect(mageSecretInput,SIGNAL(switchToWorkwidgetSignal()),this,SLOT(manShow()));
    connect(mageSecretInput,SIGNAL(switchToMagWorkwidgetSignal()),this,SLOT(magWorkWShow()));
    connect(workW,SIGNAL(switchToManSignal()),this,SLOT(manShow()));
    connect(magWorkW,SIGNAL(switchToManSignal()),this,SLOT(manShow()));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::updatetime()
{
    QDateTime time = QDateTime::currentDateTime();
    QString timestr =time.toString(" yyyy��MM��dd��  hh:mm:ss  ");
    ui->Timelab->setText(timestr);
}

void Widget::workWShow()
{
    workW->show();
    this->hide();
}

void Widget::magWorkWShow()
{
    mageSecretInput->hide();
    magWorkW->show();
    this->hide();
}

void Widget::manShow()
{
    mageSecretInput->hide();
    workW->hide();
    magWorkW->hide();
    this->show();
}

void Widget::toSecret()
{
    mageSecretInput->show();
    this->hide();
}

void Widget::on_visiBtn_clicked()
{
    emit switchToWorkWSignal();
}

void Widget::on_manaBtn_clicked()
{
    emit switchToSecretSignal();
}

/* ���� */
void Widget::serialManage()
{
    if(pSer_Com == NULL)
    {
        pSer_Com = new serial;
        if(pSer_Com == NULL)
            return;
        if(!pSer_Com->Initcom(ComDataArriveOK,"COM2",
                              BAUD115200,DATA_8,PAR_NONE,STOP_1,100))
            qDebug()<<"COM open fail!";
        else
        {
            qDebug()<<"serial suceess";
            QTimer *TimerSerData = new QTimer;
            connect(TimerSerData,SIGNAL(timeout()),this,SLOT(SendModDataSlot()));
            TimerSerData->start(200);
        }
    }
}

void Widget::SendModDataSlot()
{
    static uint i=0;
    i++;
    SendModData(SendControlData,SendIsSaveState,i);
    if(i>255)
        i=0;
}

void Widget::ComDataArriveOK(char *DataBuff,ulong m_ulen)
{
    if(pMainWnd->DataCrcCheck(DataBuff,m_ulen)&&pMainWnd!=NULL)
    {
        Comdata mData;
        memset(&mData,0,sizeof(Comdata));
        memcpy(mData.DataBuff,DataBuff,m_ulen);
        mData.Len=m_ulen;
        PutDataIntoQueuen(&mData);
    }
}

ulong Widget::CRCCheck(char *crcBuff, ulong size)
{
    ulong m_uCrcValue=0;
    if(NULL==crcBuff || size<0)
        return m_uCrcValue;
    //����Ϊcrc���������ֽڵ��ڴ�ռ�
    unsigned short crc = 0xffff;
    unsigned short a,j,k;
    unsigned char crcValue=0;
    for(a=0;a<size;a++)
    {
        //crc�͵�a���ֽ����ֵ�����ֵ����crc��ע��gemfield[a]��ת��Ϊint��ʱ���з��ŵģ�
        //��ˣ���������ֵ����7Fʱ����Ҫ����Ӧ��ת������
        crc =crc ^ ( (int)crcBuff[a]>=0 ? crcBuff[a] : (crcBuff[a]+256) );
        for(j=0;j<8;j++)
        {
            k=crc & 01;//��crc�����λ�ǲ���0���������0������0xA001���
            crc = crc >> 1;
            if (k==0) continue;
            crc =crc ^ 0xA001;
        }
    }
    crcValue =crc%256;
    m_uCrcValue=(crcValue<<8)&0xff00;
    crcValue =crc/256;
    m_uCrcValue=m_uCrcValue|((crcValue)&0x00ff);
    return m_uCrcValue;    //����������,�����Ǹ�ʮ������
}

bool Widget::DataCrcCheck(char *DataBuff, ulong m_ulen)
{
    if(DataBuff==NULL||m_ulen<=2)
        return false;
    uint m_CrcLow,m_CrcHig;
    ulong m_uCrcValue=0;
    m_uCrcValue = CRCCheck(DataBuff,m_ulen - 2);
    m_CrcHig=(m_uCrcValue)>>8&0xff;
    m_CrcLow=m_uCrcValue&0xff;
    //qDebug()<<m_CrcHig<<m_CrcLow<<"CRChL";
    //�ж�CRC�Ƿ���ȷ
    if( m_CrcLow==(uchar)DataBuff[m_ulen - 1]&&m_CrcHig == (uchar)DataBuff[m_ulen - 2] )
    {
        qDebug()<< "CRC ok!";
        return true;
    }
    else
    {
        qDebug()<<"CRC error";
        return false;
    }
    return false;
}

void Widget::SendModData(qint64 Data,qint64 IsSaveState,int Counter)
{
    uint m_CrcLow = 0,m_CrcHig = 0;//���CRC�ߵ�λ
    char SendData[10]={0};//��ŷ��ͻ�������
    SendData[0]=0x01;//modbus ��ַλ
    SendData[1]=0x01;//����λ
    SendData[2]=Data;//����λ
    SendData[3] = IsSaveState;//�Ƿ�洢״̬
    SendData[4]=Counter;

    ulong m_uCrcValue=0;//crcУ�鷵��ֵ
    m_uCrcValue = CRCCheck(SendData,5);//CRCУ��
    //ȡ��CRC�ߵ�λ
    m_CrcHig=m_uCrcValue>>8&0xff;
    m_CrcLow=m_uCrcValue&0xff;
    //��CRC�ߵ�λ���뻺��
    SendData[5]=m_CrcHig;
    SendData[6]=m_CrcLow;

    //�ж�mainwindow��pser_com����������Ѵ���
    if( pMainWnd!=NULL)
    if( pMainWnd->pSer_Com!=NULL)
    //Ȼ�󼴿��õ���mainwindow��pserCOM����ĺ�����������
    //                 for(int i =0;i<7;i++)
    //                     qDebug("%d",(uchar)SendData[i]);
    pMainWnd->pSer_Com->SendData(SendData,10);
    qDebug("%c",SendData[5]);
}
