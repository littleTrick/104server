#include "iec104_parser.h"
#include <cassert>
#include <cerrno>
#include <sys/time.h>
#include <stdlib.h>
#include <cstdio>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include "logging.h"

using namespace std;

IEC104Parser::IEC104Parser(int clientfd)
{

    fd_ = clientfd;
    masterAddr_ = 1;
    seq_order_check = true;
    VS = 0;
    VR = 0;
}

IEC104Parser::~IEC104Parser()
{
    shutdown();
}

//来自主站读取的数据重组ＡＰＤＵ
void IEC104Parser::start()
{
    APDU apdu;
    unsigned char *buff;
    unsigned char len;
    int nbytes;

    buff = (unsigned char*)&apdu;

    while (true)
    {
        // 寻找起始位START
        do
        {
            nbytes =read((char*)buff, 1);
            if (nbytes <= 0)
                return;
        } while (buff[0] != START);
        nbytes = read( (char*)buff+1, 1); // length of APDU
        if (nbytes <= 0)
            return;

        len = buff[1];
        if (len < 4) // APDU length must be >= 4
        {
            LOG(WARN) << "--> ERROR: INVALID FRAME\n";
            continue;
        }

        nbytes = read((char*)buff+2, len); // read the remaining of the APDU
        if (nbytes <= 0)
        {
            LOG(WARN) << "--> Broken APDU\n";
            return;
        }

        if (apdu.asduh.comAddr!=masterAddr_ && len>4 )
        {
            LOG(WARN) << "--> READ FORM MASTER ASDU WITH UNEXPECTED ORIGIN! Ignoring...\n";
            continue;
        }

        parse(&apdu, len + 2);
    }
}

void IEC104Parser::disableSequenceOrderCheck()
{
    seq_order_check = false;
}

int IEC104Parser::read(char *data, int size)
{
    int nread = 0;
    while (nread < size) {
        ssize_t n = ::read(fd_, data + nread, size - nread);
        if(n < 0)
        {
            LOG(ERROR) << "read error client fd=" << fd_ << " reason:" << strerror(errno);
            return -1;
        }
        if(n == 0)
        {
            LOG(INFO) << "connection closed by client fd=" << fd_;
            return -1;
        }

        nread += n;
    }
    return nread;
}

void IEC104Parser::send(const char *data, int sz)
{
    ssize_t nwrite = write(fd_,data,sz);
    if(nwrite < 0)
    {
        LOG(ERROR) << "write error clint fd=" << fd_ << " reason:" << strerror(errno);
    }
}

void IEC104Parser::send(const APDU &apdu)
{
    send((char*)&apdu, apdu.apci.lenth + 2);
    VS += 2;
}

void IEC104Parser::shutdown()
{
    if (fd_ >= 0)
    {
        close(fd_);
        fd_ = -1;
    }
}

//总召唤确认
void IEC104Parser::totalCallConf()
{
    APDU wapdu;  //定义总召唤确认报文
    wapdu.apci.start = START;
    wapdu.apci.lenth = 0x0E;
    wapdu.apci.NS = VS;
    wapdu.apci.NR = VR;
    wapdu.asduh.ti = INTERROGATION;//报文类型１００
    wapdu.asduh.number = 1;
    wapdu.asduh.sq = 0;
    wapdu.asduh.cot = ACTCONFIRM;//传输原因：７
    wapdu.asduh.comAddr = masterAddr_;
    wapdu.asduinfo.uinfo.dados[0] = 0x00;
    wapdu.asduinfo.uinfo.dados[1] = 0x00;
    wapdu.asduinfo.uinfo.dados[2] = 0x00;
    wapdu.asduinfo.uinfo.dados[3] = 0x14; //召唤限定词
    send(wapdu);
}

//总召唤结束
void IEC104Parser::endTotalCall()
{
    APDU wapdu;  //定义总召唤确认报文
    wapdu.apci.start = START;
    wapdu.apci.lenth = 0x0E;
    wapdu.apci.NS = VS;
    wapdu.apci.NR = VR;
    wapdu.asduh.ti = INTERROGATION;//报文类型１００
    wapdu.asduh.number = 1;
    wapdu.asduh.sq = 0;
    wapdu.asduh.cot = ACTTERM;//传输原因：10　激活终止
    wapdu.asduh.comAddr = masterAddr_;
    wapdu.asduinfo.uinfo.dados[0] = 0x00;
    wapdu.asduinfo.uinfo.dados[1] = 0x00;
    wapdu.asduinfo.uinfo.dados[2] = 0x00;
    wapdu.asduinfo.uinfo.dados[3] = 0x14; //召唤限定词
    send(wapdu);
}

//时钟读取确认
void IEC104Parser::readClockConf()
{
    APDU wapdu;
    wapdu.apci.start = START;
    wapdu.apci.lenth = 0x14;
    wapdu.apci.NS = VS;
    wapdu.apci.NR = VR;
    wapdu.asduh.ti = 0x67;
    wapdu.asduh.number = 1;
    wapdu.asduh.sq = 0;
    wapdu.asduh.cot = REQUEST;
    wapdu.asduh.comAddr = masterAddr_;

    //携带的系统时间
    time_t tm1 = time( NULL );
    struct timeval time_tv;
    tm *agora = localtime( &tm1 );
    gettimeofday(&time_tv,0);//为获取微妙而生

    wapdu.asduinfo.uinfo.csc.ioa16 = 0;
    wapdu.asduinfo.uinfo.csc.ioa8 = 0;
    wapdu.asduinfo.uinfo.csc.obj[0].time.year = (unsigned char)agora->tm_year % 100;
    wapdu.asduinfo.uinfo.csc.obj[0].time.month = (unsigned char)agora->tm_mon + 1;
    wapdu.asduinfo.uinfo.csc.obj[0].time.mday = (unsigned char)agora->tm_mday;
    wapdu.asduinfo.uinfo.csc.obj[0].time.hour = (unsigned char)agora->tm_hour;
    wapdu.asduinfo.uinfo.csc.obj[0].time.min = (unsigned char)agora->tm_min;
    wapdu.asduinfo.uinfo.csc.obj[0].time.msec = (unsigned short)(agora->tm_sec * 1000 + time_tv.tv_usec /1000);
    send(wapdu);
}

//时钟同步帧
void IEC104Parser::clockSyncConf(const struct timeval *time_tv,const struct tm *time_tm)
{
    //读取系统时钟
    APDU wapdu;
    wapdu.apci.start = START;
    wapdu.apci.lenth = 0x14;
    wapdu.apci.NS = VS;
    wapdu.apci.NR = VR;
    wapdu.asduh.ti = 0x67;
    wapdu.asduh.number = 1;
    wapdu.asduh.sq = 0;
    wapdu.asduh.cot = ACTCONFIRM;
    wapdu.asduh.comAddr = masterAddr_;

    wapdu.asduinfo.uinfo.csc.ioa16 = 0;//信息对象地址为０
    wapdu.asduinfo.uinfo.csc.ioa8 = 0;
    wapdu.asduinfo.uinfo.csc.obj[0].time.year = (unsigned char)((time_tm->tm_year + 1900) % 100);
    wapdu.asduinfo.uinfo.csc.obj[0].time.month = (unsigned char)(time_tm->tm_mon + 1);
    wapdu.asduinfo.uinfo.csc.obj[0].time.mday = (unsigned char)time_tm->tm_mday;
    wapdu.asduinfo.uinfo.csc.obj[0].time.hour = (unsigned char)time_tm->tm_hour;
    wapdu.asduinfo.uinfo.csc.obj[0].time.min = (unsigned char)time_tm->tm_min;
    wapdu.asduinfo.uinfo.csc.obj[0].time.msec = (unsigned short)(time_tm->tm_sec * 1000 + time_tv->tv_usec / 1000);

    send(wapdu);
}

//复位进程确认
void IEC104Parser::resetConf()
{
    APDU wapdu;
    wapdu.apci.start = START;
    wapdu.apci.lenth = 0x0E;
    wapdu.apci.NS = VS;
    wapdu.apci.NR = VR;
    wapdu.asduh.ti = 0x69;
    wapdu.asduh.number = 1;
    wapdu.asduh.sq = 0;
    wapdu.asduh.cot = ACTCONFIRM;
    wapdu.asduh.comAddr = masterAddr_;
    wapdu.asduinfo.uinfo.grp.ioa16 = 0;
    wapdu.asduinfo.uinfo.grp.ioa8 = 0;
    wapdu.asduinfo.uinfo.grp.obj[0].VGR = QRP;

    send(wapdu);
}

//发送遥信报文 不带时标，地址连续的遥信单点信息。用于总召唤
void IEC104Parser::sendYX()
{
    APDU wapdu;
    wapdu.apci.start = START;
    wapdu.apci.lenth = 0x11;
    wapdu.apci.NS = VS;
    wapdu.apci.NR = VR;
    wapdu.asduh.ti = 1;//单点信息M_SP_NA_1
    wapdu.asduh.number = 4;
    wapdu.asduh.sq = 1;
    wapdu.asduh.cot = INTROGEN;//响应站召唤
    wapdu.asduh.comAddr = masterAddr_;
    wapdu.asduinfo.uinfo.siq.ioa16 = 1000;
    wapdu.asduinfo.uinfo.siq.ioa8 = 0;
    wapdu.asduinfo.uinfo.siq.obj[0].SPI = 0;
    wapdu.asduinfo.uinfo.siq.obj[0].RES = 0;
    wapdu.asduinfo.uinfo.siq.obj[0].BL = 1;
    wapdu.asduinfo.uinfo.siq.obj[0].SB = 1;
    wapdu.asduinfo.uinfo.siq.obj[0].NT = 1;
    wapdu.asduinfo.uinfo.siq.obj[0].IV = 1;

    wapdu.asduinfo.uinfo.siq.obj[1].SPI = 0;
    wapdu.asduinfo.uinfo.siq.obj[1].RES = 0;
    wapdu.asduinfo.uinfo.siq.obj[1].BL = 0;
    wapdu.asduinfo.uinfo.siq.obj[1].SB = 0;
    wapdu.asduinfo.uinfo.siq.obj[1].NT = 0;
    wapdu.asduinfo.uinfo.siq.obj[1].IV = 0;

    wapdu.asduinfo.uinfo.siq.obj[2].SPI = 0;
    wapdu.asduinfo.uinfo.siq.obj[2].RES = 0;
    wapdu.asduinfo.uinfo.siq.obj[2].BL = 1;
    wapdu.asduinfo.uinfo.siq.obj[2].SB = 1;
    wapdu.asduinfo.uinfo.siq.obj[2].NT = 1;
    wapdu.asduinfo.uinfo.siq.obj[2].IV = 1;

    wapdu.asduinfo.uinfo.siq.obj[3].SPI = 0;
    wapdu.asduinfo.uinfo.siq.obj[3].RES = 0;
    wapdu.asduinfo.uinfo.siq.obj[3].BL = 0;
    wapdu.asduinfo.uinfo.siq.obj[3].SB = 0;
    wapdu.asduinfo.uinfo.siq.obj[3].NT = 0;
    wapdu.asduinfo.uinfo.siq.obj[3].IV = 0;

    send(wapdu);
}

//发送遥测报文，不带时标，地址连续的短浮点测量值。用于总召唤
void IEC104Parser::sendYC()
{
    APDU wapdu;
    wapdu.apci.start = START;
    wapdu.apci.lenth = 0x21;
    wapdu.apci.NS = VS;
    wapdu.apci.NR = VR;
    wapdu.asduh.ti = 13;//短浮点数类型标识
    wapdu.asduh.number = 4;
    wapdu.asduh.sq = 1;
    wapdu.asduh.cot = INTROGEN;//响应站召唤
    wapdu.asduh.comAddr = masterAddr_;
    wapdu.asduinfo.uinfo.std.ioa16 = 4001;
    wapdu.asduinfo.uinfo.std.ioa8 = 0;
    wapdu.asduinfo.uinfo.std.obj[0].mv = 1.05;
    wapdu.asduinfo.uinfo.std.obj[0].OV = 0;
    wapdu.asduinfo.uinfo.std.obj[0].RES = 0;
    wapdu.asduinfo.uinfo.std.obj[0].BL = 0;
    wapdu.asduinfo.uinfo.std.obj[0].SB = 0;
    wapdu.asduinfo.uinfo.std.obj[0].NT = 0;
    wapdu.asduinfo.uinfo.std.obj[0].IV = 0;

    wapdu.asduinfo.uinfo.std.obj[1].mv = 1.15;
    wapdu.asduinfo.uinfo.std.obj[1].OV = 0;
    wapdu.asduinfo.uinfo.std.obj[1].RES = 0;
    wapdu.asduinfo.uinfo.std.obj[1].BL = 0;
    wapdu.asduinfo.uinfo.std.obj[1].SB = 0;
    wapdu.asduinfo.uinfo.std.obj[1].NT = 0;
    wapdu.asduinfo.uinfo.std.obj[1].IV = 0;

    wapdu.asduinfo.uinfo.std.obj[2].mv = 1.25;
    wapdu.asduinfo.uinfo.std.obj[2].OV = 0;
    wapdu.asduinfo.uinfo.std.obj[2].RES = 0;
    wapdu.asduinfo.uinfo.std.obj[2].BL = 0;
    wapdu.asduinfo.uinfo.std.obj[2].SB = 0;
    wapdu.asduinfo.uinfo.std.obj[2].NT = 0;
    wapdu.asduinfo.uinfo.std.obj[2].IV = 0;

    wapdu.asduinfo.uinfo.std.obj[3].mv = 1.35;
    wapdu.asduinfo.uinfo.std.obj[3].OV = 0;
    wapdu.asduinfo.uinfo.std.obj[3].RES = 0;
    wapdu.asduinfo.uinfo.std.obj[3].BL = 0;
    wapdu.asduinfo.uinfo.std.obj[3].SB = 0;
    wapdu.asduinfo.uinfo.std.obj[3].NT = 0;
    wapdu.asduinfo.uinfo.std.obj[3].IV = 0;

    send(wapdu);
}

//解析报文函数
void IEC104Parser::parse(const APDU *apdu, int sz)
{
    APDU wapdu;  //缓冲区组装发送apdu
    unsigned short vr_new;

    if(apdu->apci.start != START)
    {
        LOG(ERROR) << "ERROR: NO START IN FRAME\n";
        return;
    }

    if(apdu->asduh.comAddr != masterAddr_ && apdu->apci.lenth > 6)
    {
        LOG(ERROR) << "PAESE ASDU WITH UNEXPECTED ORIGIN! Ignoring...\n";
        return;
    }

    if(sz == 6)
    {
        switch (apdu->apci.NS) {
        case STARTDTACT:    //启动链路确认
            LOG(INFO) << "STARTDTCON...";
            wapdu.apci.start = START;
            wapdu.apci.lenth = 4;
            wapdu.apci.NS = STARTDTCON;
            wapdu.apci.NR = 0;
            send(wapdu);
            LOG(INFO) << "STARTDTCON\n";
            break;
        case TESTFRACT:      //心跳测试确认
            LOG(INFO) << "TESTFRACT...";
            wapdu.apci.start=START;
            wapdu.apci.lenth=4;
            wapdu.apci.NS=TESTFRCON;
            wapdu.apci.NR=0;
            send(wapdu);
            LOG(INFO) << "TESTFRCON...";
            break;
        case STOPDTACT:    // 停止链路链接确认
            LOG(INFO) << "STOPDTACT...";
            wapdu.apci.start=START;
            wapdu.apci.lenth=4;
            wapdu.apci.NS=STOPDTCON;
            wapdu.apci.NR=0;
            send(wapdu);
            LOG(INFO) << "STOPDTCON...";
            break;
        default: // error
            LOG(ERROR) << "ERROR: UNKNOWN CONTROL MESSAGE\n";
            break;
        }
    }
    else
    {
        //I格式
        vr_new = (apdu->apci.NS & 0xFFFE);  //为什么?
        if ( vr_new != VR )
        {
            // 序列错误，必须关闭ＴＣＰ链接并重新打开链接
            LOG(INFO) << "*** SEQUENCE ERROR! **************************\n";
            if ( seq_order_check )
            {
                shutdown();
                return;
            }
        }
        VR = vr_new + 2; //为什么加２？

        unsigned int addr24 = 0; //信息元素地址
        const SIQ *pobj;
        iec_obj *piecarr;
        switch (apdu->asduh.ti)
        {
        case M_SP_NA_1:
            piecarr = new iec_obj[apdu->asduh.number];
            for(int i = 0; i < apdu->asduh.number; i++)
            {
                if(apdu->asduh.sq)
                {
                    pobj = &apdu->asduinfo.uinfo.siq.obj[i];//不带时标的信息元素１
                    if(i == 0)
                    {
                        addr24 = apdu->asduinfo.uinfo.siq.ioa16+
                                ((unsigned)apdu->asduinfo.uinfo.siq.ioa8 << 16);
                    }
                    else
                    {
                        addr24++;
                    }
                }
                else
                {
                    pobj = &apdu->asduinfo.uinfo.nsiq[i].obj;
                    addr24 = apdu->asduinfo.uinfo.nsiq[i].ioa16+
                            ((unsigned)apdu->asduinfo.uinfo.nsiq[i].ioa8 << 16);
                }
                piecarr[i].address = addr24;
                piecarr[i].ca = apdu->asduh.comAddr;
                piecarr[i].cause = apdu->asduh.cot;
                piecarr[i].type =apdu->asduh.ti;
                piecarr[i].value = pobj->SPI;
                piecarr[i].infoSort.sp = pobj->SPI;
                piecarr[i].bl = pobj->BL;
                piecarr[i].nt =pobj->NT;
                piecarr[i].sb = pobj->SB;
                piecarr[i].iv = pobj->IV;
            }
            send((char *)piecarr, apdu->asduh.number * sizeof(iec_obj));
            delete[] piecarr;
            break;
        case M_ME_NC_1://变化遥测
            if(apdu->asduh.cot == SPONTANEOUS)
            {
                send(*apdu);
            }
            else
            {
                LOG(ERROR) << "invalid " << apdu->asduh.cot;
            }
            break;
        case C_IC_NA_1://总召唤
            totalCallConf();//总召唤确认
            //发送遥测与遥信数据
            sendYX();
            sendYC();
            endTotalCall();//总召唤结束
            break;
        case C_CS_NA_1://时钟同步103
            if(apdu->asduh.cot == 6)
            {
                //立即更新系统时钟
                tm time_tm;
                time_t timep;
                timeval time_tv;

                memset(&time_tm, 0, sizeof(time_tm));
                time_tm.tm_year =(int)apdu->asduinfo.uinfo.csc.obj[0].time.year + 2000 - 1900;
                time_tm.tm_mon = (int)apdu->asduinfo.uinfo.csc.obj[0].time.month - 1;
                time_tm.tm_mday = (int)apdu->asduinfo.uinfo.csc.obj[0].time.mday;
                time_tm.tm_hour = (int)apdu->asduinfo.uinfo.csc.obj[0].time.hour;
                time_tm.tm_min = (int)apdu->asduinfo.uinfo.csc.obj[0].time.min;
                time_tm.tm_sec = (int)apdu->asduinfo.uinfo.csc.obj[0].time.msec / 1000;

                time_tv.tv_usec = (int)(apdu->asduinfo.uinfo.csc.obj[0].time.msec%1000) * 1000;

                if ((timep = mktime(&time_tm))< 0) {
                    LOG(ERROR) << strerror(errno);
                }
                time_tv.tv_sec = timep;

                int ret = settimeofday(&time_tv, NULL);
                if(ret == 0)
                {
                    LOG(TRACE) << "settimeofday succed clientfd=" << fd_;
                }
                else
                {
                    LOG(ERROR) << "settimeofday error: " << strerror(errno);
                }
                system("hwclock -w");//写入时间到CMOS中
                clockSyncConf(&time_tv,&time_tm); //携带当前系统时间回复响应帧
                break;
            }
            else if(apdu->asduh.cot == 5)
            {
                readClockConf();
                break;
            }
            else
            {
                LOG(ERROR) << "invalid cot " << apdu->asduh.cot << " client fd=" << fd_;
            }
        case C_RP_NA_1://复位进程命令105
            if(apdu->asduh.cot == ACTIVATION)
            {
                resetConf();
            }
            else
            {
                LOG(ERROR) << "invalid cot " << apdu->asduh.cot << " client fd=" << fd_;
            }
            //待写设备重启函数！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！
            break;
        default:
            LOG(FATAL) << "unknown TI " << apdu->asduh.ti;
            break;
        }
    }

}
