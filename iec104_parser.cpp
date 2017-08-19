#include "iec104_parser.h"
#include <cassert>
#include <sys/time.h>
#include <stdlib.h>
#include <cstdio>
#include <unistd.h>
#include <cstring>
#include <iostream>

using namespace std;

#define LOG(msg) cerr << __FILE__ << ":" << __LINE__ << " " << (msg) << endl

IEC104Parser::IEC104Parser()
{

    masterAddr = 1;
    slaveAddr = 0;
    seq_order_check = true;
    VS = 0;
    VR = 0;
}

void IEC104Parser::disableSequenceOrderCheck()
{
    seq_order_check = false;
}

//总召唤确认
void IEC104Parser::totalCallConf()
{
    apdu wapdu;  //定义总召唤确认报文
    wapdu.apci.start = START;
    wapdu.apci.lenth = 0x0E;
    wapdu.apci.NS = VS;
    wapdu.apci.NR = VR;
    wapdu.asduh.typeIdentification = INTERROGATION;//报文类型１００
    wapdu.asduh.number = 1;
    wapdu.asduh.sq = 0;
    wapdu.asduh.transCause = ACTCONFIRM;//传输原因：７
    wapdu.asduh.comAddr = masterAddr;
    wapdu.asduinfo.uinfo.dados[0] = 0x00;
    wapdu.asduinfo.uinfo.dados[1] = 0x00;
    wapdu.asduinfo.uinfo.dados[2] = 0x00;
    wapdu.asduinfo.uinfo.dados[3] = 0x14; //召唤限定词
    sendTCP((char *)&wapdu,16);
    VS += 2;
}

//总召唤结束
void IEC104Parser::endTotalCall()
{
    apdu wapdu;  //定义总召唤确认报文
    wapdu.apci.start = START;
    wapdu.apci.lenth = 0x0E;
    wapdu.apci.NS = VS;
    wapdu.apci.NR = VR;
    wapdu.asduh.typeIdentification = INTERROGATION;//报文类型１００
    wapdu.asduh.number = 1;
    wapdu.asduh.sq = 0;
    wapdu.asduh.transCause = ACTTERM;//传输原因：10　激活终止
    wapdu.asduh.comAddr = masterAddr;
    wapdu.asduinfo.uinfo.dados[0] = 0x00;
    wapdu.asduinfo.uinfo.dados[1] = 0x00;
    wapdu.asduinfo.uinfo.dados[2] = 0x00;
    wapdu.asduinfo.uinfo.dados[3] = 0x14; //召唤限定词
    sendTCP((char *)&wapdu,16);
    VS += 2;
}

//时钟读取确认
void IEC104Parser::readClockConf()
{
    apdu wapdu;
    wapdu.apci.start = START;
    wapdu.apci.lenth = 0x14;
    wapdu.apci.NS = VS;
    wapdu.apci.NR = VR;
    wapdu.asduh.typeIdentification = 0x67;
    wapdu.asduh.number = 1;
    wapdu.asduh.sq = 0;
    wapdu.asduh.transCause = REQUEST;
    wapdu.asduh.comAddr = masterAddr;

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
    sendTCP( (char *)&wapdu,22);
    VS += 2;
}

//时钟同步帧
void IEC104Parser::clockSyncConf(const struct timeval *time_tv,const struct tm *time_tm)
{
    //读取系统时钟
    apdu wapdu;
    wapdu.apci.start = START;
    wapdu.apci.lenth = 0x14;
    wapdu.apci.NS = VS;
    wapdu.apci.NR = VR;
    wapdu.asduh.typeIdentification = 0x67;
    wapdu.asduh.number = 1;
    wapdu.asduh.sq = 0;
    wapdu.asduh.transCause = ACTCONFIRM;
    wapdu.asduh.comAddr = masterAddr;

    wapdu.asduinfo.uinfo.csc.ioa16 = 0;//信息对象地址为０
    wapdu.asduinfo.uinfo.csc.ioa8 = 0;
    wapdu.asduinfo.uinfo.csc.obj[0].time.year = (unsigned char)((time_tm->tm_year + 1900) % 100);
    wapdu.asduinfo.uinfo.csc.obj[0].time.month = (unsigned char)(time_tm->tm_mon + 1);
    wapdu.asduinfo.uinfo.csc.obj[0].time.mday = (unsigned char)time_tm->tm_mday;
    wapdu.asduinfo.uinfo.csc.obj[0].time.hour = (unsigned char)time_tm->tm_hour;
    wapdu.asduinfo.uinfo.csc.obj[0].time.min = (unsigned char)time_tm->tm_min;
    wapdu.asduinfo.uinfo.csc.obj[0].time.msec = (unsigned short)(time_tm->tm_sec * 1000 + time_tv->tv_usec / 1000);

    sendTCP( (char *)&wapdu, 20+2 );
    VS += 2;
}

//复位进程确认
void IEC104Parser::resetConf()
{
    apdu wapdu;
    wapdu.apci.start = START;
    wapdu.apci.lenth = 0x0E;
    wapdu.apci.NS = VS;
    wapdu.apci.NR = VR;
    wapdu.asduh.typeIdentification = 0x69;
    wapdu.asduh.number = 1;
    wapdu.asduh.sq = 0;
    wapdu.asduh.transCause = ACTCONFIRM;
    wapdu.asduh.comAddr = masterAddr;
    wapdu.asduinfo.uinfo.grp.ioa16 = 0;
    wapdu.asduinfo.uinfo.grp.ioa8 = 0;
    wapdu.asduinfo.uinfo.grp.obj[0].VGR = QRP;

    sendTCP( (char *)&wapdu, 14+2 );
    VS += 2;
}

//发送遥信报文 不带时标，地址连续的遥信单点信息。用于总召唤
void IEC104Parser::sendYX()
{
    apdu wapdu;
    wapdu.apci.start = START;
    wapdu.apci.lenth = 0x11;
    wapdu.apci.NS = VS;
    wapdu.apci.NR = VR;
    wapdu.asduh.typeIdentification = 1;//单点信息M_SP_NA_1
    wapdu.asduh.number = 4;
    wapdu.asduh.sq = 1;
    wapdu.asduh.transCause = INTROGEN;//响应站召唤
    wapdu.asduh.comAddr = masterAddr;
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

    sendTCP( (char *)&wapdu, 17+2 );
    VS += 2;
}

//发送遥测报文，不带时标，地址连续的短浮点测量值。用于总召唤
void IEC104Parser::sendYC()
{
    apdu wapdu;
    wapdu.apci.start = START;
    wapdu.apci.lenth = 0x21;
    wapdu.apci.NS = VS;
    wapdu.apci.NR = VR;
    wapdu.asduh.typeIdentification = 13;//短浮点数类型标识
    wapdu.asduh.number = 4;
    wapdu.asduh.sq = 1;
    wapdu.asduh.transCause = INTROGEN;//响应站召唤
    wapdu.asduh.comAddr = masterAddr;
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

    sendTCP( (char *)&wapdu, 33+2 );
    VS += 2;
}

//来自主站读取的数据重组ＡＰＤＵ
void IEC104Parser::readFromMaster()
{
    static apdu rapdu;
    unsigned char * br;
    br = (unsigned char*)&rapdu;
    int bytesrec;
    unsigned char byt;
    unsigned char len;

    while (true) {
        // 寻找起始位START
        do {
            bytesrec =readTCP((char*)br, 1);
            if (bytesrec <= 0)
                return;
            byt = br[0];
        } while (byt != START);
        bytesrec=readTCP( (char*)br+1, 1); // length of apdu
        if (bytesrec <= 0)
            return;

        len = br[1];
        if (len < 4) // apdu length must be >= 4
        {
            cout << "--> ERROR: INVALID FRAME\n";
            continue;
        }

        bytesrec=readTCP((char*)br+2, len); // read the remaining of the apdu
        if (bytesrec <= 0)
        {
            cout << "--> Broken apdu\n";
            return;
        }


        if (rapdu.asduh.comAddr != masterAddr && len>4 )
        {
            cout << "--> READ FORM MASTER ASDU WITH UNEXPECTED ORIGIN! Ignoring...\n";
            continue;
        }

        parseAPDU(&rapdu, len + 2);
    }
}

//解析报文函数
void IEC104Parser::parseAPDU(const apdu *papdu, int sz)
{
    apdu wapdu;  //缓冲区组装发送apdu
    unsigned short VR_NEW;

    if(papdu->apci.start != START)
    {
        cout << "ERROR: NO START IN FRAME\n";
        return;
    }
    if(papdu->asduh.comAddr != masterAddr && papdu->apci.lenth > 6)
    {
        cout << "PAESE ASDU WITH UNEXPECTED ORIGIN! Ignoring...\n";
        return;
    }
    if(sz == 6)
    {
        switch (papdu->apci.NS) {
        case STARTDTACT:    //启动链路确认
            LOG("STARTDTCON...");
            wapdu.apci.start = START;
            wapdu.apci.lenth = 4;
            wapdu.apci.NS = STARTDTCON;
            wapdu.apci.NR = 0;
            sendTCP((char *)&wapdu, 6);
            cout << "STARTDTCON\n";
            break;

        case TESTFRACT:      //心跳测试确认
            LOG("TESTFRACT...");
            wapdu.apci.start=START;
            wapdu.apci.lenth=4;
            wapdu.apci.NS=TESTFRCON;
            wapdu.apci.NR=0;
            sendTCP((char *)&wapdu, 6);
            LOG( "TESTFRCON...\n");
            break;

        case STOPDTACT:    // 停止链路链接确认
            LOG("STOPDTACT...\n");
            wapdu.apci.start=START;
            wapdu.apci.lenth=4;
            wapdu.apci.NS=STOPDTCON;
            wapdu.apci.NR=0;
            sendTCP((char *)&wapdu, 6);
            LOG("STOPDTCON...\n");
            break;

        default: // error
            cout << "ERROR: UNKNOWN CONTROL MESSAGE\n";
            break;
        }
    }
    else
    {
        //I格式
        VR_NEW = (papdu->apci.NS & 0xFFFE);  //为什么?
        if ( VR_NEW != VR )
        {
            // 序列错误，必须关闭ＴＣＰ链接并重新打开链接
            cout << "*** SEQUENCE ERROR! **************************\n";
            if ( seq_order_check )
            {
                disconnectTCP();
                return;
            }
        }
        VR = VR_NEW + 2; //为什么加２？

        unsigned int addr24 = 0; //信息元素地址
        const SIQ *pobj;
        iec_obj *piecarr;
        switch (papdu->asduh.typeIdentification)
        {
        case M_SP_NA_1:
            piecarr = new iec_obj[papdu->asduh.number];
            for(int i = 0; i < papdu->asduh.number; i++)
            {
                if(papdu->asduh.sq)
                {
                    pobj = &papdu->asduinfo.uinfo.siq.obj[i];//不带时标的信息元素１
                    if(i == 0)
                    {
                        addr24 = papdu->asduinfo.uinfo.siq.ioa16+
                                ((unsigned)papdu->asduinfo.uinfo.siq.ioa8 << 16);
                    }
                    else
                    {
                        addr24++;
                    }
                }
                else
                {
                    pobj = &papdu->asduinfo.uinfo.nsiq[i].obj;
                    addr24 = papdu->asduinfo.uinfo.nsiq[i].ioa16+
                            ((unsigned)papdu->asduinfo.uinfo.nsiq[i].ioa8 << 16);
                }
                piecarr[i].address = addr24;
                piecarr[i].ca = papdu->asduh.comAddr;
                piecarr[i].cause = papdu->asduh.transCause;
                piecarr[i].type =papdu->asduh.typeIdentification;
                piecarr[i].value = pobj->SPI;
                piecarr[i].infoSort.sp = pobj->SPI;
                piecarr[i].bl = pobj->BL;
                piecarr[i].nt =pobj->NT;
                piecarr[i].sb = pobj->SB;
                piecarr[i].iv = pobj->IV;
            }
            sendTCP((char *)piecarr, papdu->asduh.number * sizeof(iec_obj));
            delete[] piecarr;
            break;
        case M_ME_NC_1://变化遥测
            if(papdu->asduh.transCause == SPONTANEOUS)
            {
                sendTCP((char *)&papdu,sizeof(papdu));
                VS += 2;
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
            if(papdu->asduh.transCause == 6)
            {
                //立即更新系统时钟
                tm time_tm;
                time_t timep;
                timeval time_tv;


                unsigned char *p = (unsigned char*)papdu;
                for (int i = 0; i < sz; i++) {
                    printf("%02X ", p[i]);
                }
                printf("\n");

                memset(&time_tm, 0, sizeof(time_tm));
                time_tm.tm_year =(int)papdu->asduinfo.uinfo.csc.obj[0].time.year + 2000 - 1900;
                time_tm.tm_mon = (int)papdu->asduinfo.uinfo.csc.obj[0].time.month - 1;
                time_tm.tm_mday = (int)papdu->asduinfo.uinfo.csc.obj[0].time.mday;
                time_tm.tm_hour = (int)papdu->asduinfo.uinfo.csc.obj[0].time.hour;
                time_tm.tm_min = (int)papdu->asduinfo.uinfo.csc.obj[0].time.min;
                time_tm.tm_sec = (int)papdu->asduinfo.uinfo.csc.obj[0].time.msec / 1000;

                time_tv.tv_usec = (int)(papdu->asduinfo.uinfo.csc.obj[0].time.msec%1000) * 1000;

                if ((timep = mktime(&time_tm))< 0) {
                    perror("mktime");
                }
                time_tv.tv_sec = timep;

                int ret = settimeofday(&time_tv, NULL);
                if(ret != 0)
                {
                    perror("settimeofday failed\n");
                }
                else
                {
                    cout << "settimeofday succed\n";
                }
                system("hwclock -w");//写入时间到CMOS中
                clockSyncConf( &time_tv,&time_tm); //携带当前系统时间回复响应帧
                break;
            }
            else if(papdu->asduh.transCause == 5)
            {
                readClockConf();
                break;
            }
        case C_RP_NA_1://复位进程命令105
            if(papdu->asduh.transCause == ACTIVATION)
            {
                resetConf();
            }
            //待写设备重启函数！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！
            break;
        default:
            assert(0);
            break;
        }
    }

}
