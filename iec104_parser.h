#ifndef IEC104Parser_H
#define IEC104Parser_H

#include "iec104_types.h"

struct tm;

struct iec_obj {
    unsigned int address;  // 3 byte address

    float value; // value

    CP56Time2a timetag; // 7 byte time tag
    unsigned char reserved; // for future use

    unsigned char type; // iec type
    unsigned char cause; //
    unsigned short ca;  // common addres of asdu

    union {
        unsigned char ov :1; // overflow/no overflow
        unsigned char sp :1; // single point information
        unsigned char dp :2; // double point information
        unsigned char scs :1; // single command state
        unsigned char dcs :2; // double command state
        unsigned char rcs :2; // regulating step command
    }infoSort; // 2 bits

    unsigned char qu :5; // qualifier of command
    unsigned char se :1; // select=1 / execute=0
    // + 6 bits = 8 bits

    unsigned char bl :1; // blocked/not blocked
    unsigned char sb :1; // substituted/not substituted
    unsigned char nt :1; // not topical/topical
    unsigned char iv :1; // valid/invalid
    unsigned char t :1; // transient flag
    unsigned char pn :1; // 0=positive, 1=negative
};


class IEC104Parser
{
public:
    IEC104Parser();
    static const unsigned int START = 0x68;
    unsigned short masterAddr;
    unsigned char slaveAddr;

    //传送原因COT
    static const unsigned int CYCLIC = 1;
    static const unsigned int BGSCAN = 2;
    static const unsigned int SPONTANEOUS = 3;//突发（自发）
    static const unsigned int REQUEST = 5;//被请求
    static const unsigned int ACTIVATION = 6;//激活
    static const unsigned int ACTCONFIRM = 7;//激活确认
    static const unsigned int ACTTERM = 10;//激活终止
    static const unsigned int INTROGEN = 20;//响应站召唤

    //命令限定词
    static const unsigned int QRP = 1;//复位进程命令限定词

    //报文类型
    static const unsigned int INTERROGATION = 0x64;


    //U格式
    static const unsigned int STARTDTACT = 0x07;
    static const unsigned int STARTDTCON = 0x0B;
    static const unsigned int STOPDTACT = 0x13;
    static const unsigned int STOPDTCON = 0x23;
    static const unsigned int TESTFRACT = 0x43;
    static const unsigned int TESTFRCON = 0x83;

    //I格式的类型标识
    static const unsigned int M_SP_NA_1 = 1; // 单点信息
    static const unsigned int M_DP_NA_1 = 3; // 双点信息
    static const unsigned int M_ST_NA_1 = 5; // 步位置信息
    static const unsigned int M_BO_NA_1 = 7; // ３２位比特串
    static const unsigned int M_ME_NA_1 = 9; // 规一化值
    static const unsigned int M_ME_NB_1 = 11; // 标度化值
    static const unsigned int M_ME_NC_1 = 13; // 短浮点数
    static const unsigned int M_IT_NA_1 = 15; // 累计量
    static const unsigned int M_SP_TB_1 = 30; // 带时标的单点信息
    static const unsigned int M_DP_TB_1 = 31; // 带时标的双点信息
    static const unsigned int M_ST_TB_1 = 32; // 带时标的步位置信息
    static const unsigned int M_BO_TB_1 = 33; // 带时标的３２位比特串
    static const unsigned int M_ME_TD_1 = 34; // 带时标的规一化值
    static const unsigned int M_ME_TE_1 = 35; // 带时标的标度化值
    static const unsigned int M_ME_TF_1 = 36; // 带时标的短浮点数
    static const unsigned int M_IT_TB_1 = 37; // 带时标的累计量
    static const unsigned int C_SC_NA_1 = 45; // 单点命令
    static const unsigned int C_DC_NA_1 = 46; // 双点命令
    static const unsigned int C_RC_NA_1 = 47; // 步调节命令
    static const unsigned int C_SC_TA_1 = 58; // 带时标的单点命令
    static const unsigned int C_DC_TA_1 = 59; // 带时标的双点命令
    static const unsigned int C_RC_TA_1 = 60; // 带时标的步调节命令
    static const unsigned int M_EI_NA_1 = 70; // 初始化结束
    static const unsigned int C_IC_NA_1 = 100; // 站总召唤命令
    static const unsigned int C_CI_NA_1 = 101; // 电能量召唤命令
    static const unsigned int C_CS_NA_1 = 103; // 时钟同步命令
    static const unsigned int C_RP_NA_1 = 105; // 复位进程命令
    static const unsigned int C_SR_NA_1 = 200; //　切换定值区
    static const unsigned int C_RR_NA_1 = 201; //　读定值区号
    static const unsigned int C_RS_NA_1 = 202; //　读参数和定值
    static const unsigned int C_WS_NA_1 = 203; // 写参数和定值
    static const unsigned int F_FR_NA_1 = 210; //　文件传输
    static const unsigned int F_SR_NA_1 = 211; //　软件升级

    void disableSequenceOrderCheck();  // 允许序列乱序

protected:
    void parseAPDU(const apdu * papdu,int sz);
    void readFromMaster();
    void totalCallConf();//总召唤回应
    void endTotalCall();//总召唤结束
    void clockSyncConf(const struct timeval * , const struct tm * );//时钟同步
    void readClockConf();//时钟读取确认
    void resetConf();//复位进程确认
    void sendYX();//发送遥信报文,总召唤
    void sendYC();//发送遥测报文，总召唤
    void sendYX_T();//发送遥信报文，自发数据或者突发事件发生
    void sendYC_T();//发送遥测报文，主动周期性的发送给主站

    // ---- 纯虚函数，用户在派生类中定义（强制性）---
    
    virtual void sendTCP(char *data, int sz) = 0;
    virtual int readTCP(char *data, int sz) = 0;
    virtual void disconnectTCP() = 0;

    
    // ---- 虚函数，用户在派生类中定义（不是强制性的）---

    // 用户点进程，用户提供。 （一次通话只能是一种类型的对象）组装接受报文？
    virtual void dataIndication( iec_obj * /*obj*/, int /*numpoints*/){}

private:
    unsigned short VS; //发送包计数
    unsigned short VR; //接受包计数
    bool seq_order_check; //是否允许乱序

};

#endif // IEC104Parser_H
