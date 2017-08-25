#ifndef DATACACHE_H
#define DATACACHE_H
#include<vector>
#include"iec104_types.h"

using std::vector;

struct ASDUINFO1
{
    union
    {
        unsigned char dados[255];
        //SIQ siq;
        struct {
            unsigned short ioa16;
            SIQ obj[1];
        } siq;

        struct {
            unsigned short ioa16;
            SIQ obj;
        } nsiq[1];
        // SIQ_T siq_t;
        struct {
            unsigned short ioa16;
            SIQ_T obj[1];
        } siq_t;

        struct {
            unsigned short ioa16;
            SIQ_T obj;
        } nsiq_t[1];
        // DIQ diq;
        struct {
            unsigned short ioa16;
            DIQ obj[1];
        } diq;

        struct {
            unsigned short ioa16;
            DIQ obj;
        } ndiq[1];

        //DIQ_T diq_t;
        struct {
            unsigned short ioa16;
            DIQ_T obj[1];
        } diq_t;

        struct {
            unsigned short ioa16;
            DIQ_T obj;
        } ndiq_t[1];
        //SLI sli;
        struct {
            unsigned short ioa16;
            SLI obj[1];
        } sli;

        struct {
            unsigned short ioa16;
            SLI obj;
        } nsli[1];
        //SLI_T sli_t;
        struct {
            unsigned short ioa16;
            SLI_T obj[1];
        } sli_t;

        struct {
            unsigned short ioa16;
            SLI_T obj;
        } nsli_t[1];
        //BSI bsi;
        struct {
            unsigned short ioa16;
            BSI obj[1];
        } bsi;

        struct {
            unsigned short ioa16;
            BSI obj;
        } nbsi[1];
        //BSI_T bsi_t;
        struct {
            unsigned short ioa16;
            BSI_T obj[1];
        } bsi_t;

        struct {
            unsigned short ioa16;
            BSI_T obj;
        } nbsi_t[1];
        //NVA nva;
        struct {
            unsigned short ioa16;
            NVA obj[1];
        } nva;

        struct {
            unsigned short ioa16;
            NVA obj;
        } nnva[1];
        //NVA_T nva_t;
        struct {
            unsigned short ioa16;
            NVA_T obj[1];
        } nva_t;

        struct {
            unsigned short ioa16;
            NVA_T obj;
        } nnva_t[1];
        //SVA sva;
        struct {
            unsigned short ioa16;
            SVA obj[1];
        } sva;

        struct {
            unsigned short ioa16;
            SVA obj;
        } nsva[1];
        //SVA_T sva_t;
        struct {
            unsigned short ioa16;
            SVA_T obj[1];
        } sva_t;

        struct {
            unsigned short ioa16;
            SVA_T obj;
        } nsva_t[1];
        //STD std;
        struct {
            unsigned short ioa16;
            STD obj[1];
        } std;

        struct {
            unsigned short ioa16;
            STD obj;
        } nstd[1];
        //STD_T std_t;
        struct {
            unsigned short ioa16;
            STD_T obj[1];
        } std_t;

        struct {
            unsigned short ioa16;
            STD_T obj;
        } nstd_t[1];
        //BCR brc;
        struct {
            unsigned short ioa16;
            BCR obj[1];
        } brc;

        struct {
            unsigned short ioa16;
            BCR obj;
        } nbrc[1];
        //BCR_T brc_t;
        struct {
            unsigned short ioa16;
            BCR_T obj[1];
        } brc_t;

        struct {
            unsigned short ioa16;
            BCR_T obj;
        } nbrc_t[1];
        //SCO sco;
        struct {
            unsigned short ioa16;
            SCO obj[1];
        } sco;

        struct {
            unsigned short ioa16;
            SCO obj;
        } nsco[1];
        //DCO dco;
        struct {
            unsigned short ioa16;
            DCO obj[1];
        } dco;

        struct {
            unsigned short ioa16;
            DCO obj;
        } ndco[1];
        //RCO rco;
        struct {
            unsigned short ioa16;
            RCO obj[1];
        } rco;

        struct {
            unsigned short ioa16;
            RCO obj;
        } nrco[1];
        //QOS_NVA qos_nva;
        struct {
            unsigned short ioa16;
            QOS_NVA obj[1];
        } qos_nva;

        struct {
            unsigned short ioa16;
            QOS_NVA obj;
        } nqos_nva[1];
        //QOS_SVA qos_sva;
        struct {
            unsigned short ioa16;
            QOS_SVA obj[1];
        } qos_sva;

        struct {
            unsigned short ioa16;
            QOS_SVA obj;
        } nqos_sva[1];
        //QOS_FLOAT qos_float;
        struct {
            unsigned short ioa16;
            QOS_FLOAT obj[1];
        } qos_float;

        struct {
            unsigned short ioa16;
            QOS_FLOAT obj;
        } nqos_float[1];
        //BSI_C bsi_c;
        struct {
            unsigned short ioa16;
            BSI_C obj[1];
        } bsi_c;

        struct {
            unsigned short ioa16;
            BSI_C obj;
        } nbsi_c[1];
        //COI coi;
        struct {
            unsigned short ioa16;
            COI obj[1];
        } coi;

        struct {
            unsigned short ioa16;
            COI obj;
        } ncoi[1];
        //QOI qoi;
        struct {
            unsigned short ioa16;
            QOI obj[1];
        } qoi;

        struct {
            unsigned short ioa16;
            QOI obj;
        } nqoi[1];
        //QCC qcc;
        struct {
            unsigned short ioa16;
            QCC obj[1];
        } qcc;

        struct {
            unsigned short ioa16;
            QCC obj;
        } nqcc[1];
        //CSC csc;
        struct {
            unsigned short ioa16;
            CSC obj[1];
        } csc;

        struct {
            unsigned short ioa16;
            CSC obj;
        } ncsc[1];
        //FBP fbp;
        struct {
            unsigned short ioa16;
            FBP obj[1];
        } fbp;

        struct {
            unsigned short ioa16;
            FBP obj;
        } nfbp[1];
        //GRP grp;
        struct {
            unsigned short ioa16;
            GRP obj[1];
        } grp;

        struct {
            unsigned short ioa16;
            GRP obj;
        } ngrp[1];
        //DGC dgc;
        struct {
            unsigned short ioa16;
            DGC obj[1];
        } dgc;

        struct {
            unsigned short ioa16;
            DGC obj;
        } ndgc[1];
        //NAQ naq;
        struct {
            unsigned short ioa16;
            NAQ obj[1];
        } naq;

        struct {
            unsigned short ioa16;
            NAQ obj;
        } nnaq[1];
        //SAQ saq;
        struct {
            unsigned short ioa16;
            SAQ obj[1];
        } saq;

        struct {
            unsigned short ioa16;
            SAQ obj;
        } nsaq[1];
        //FAQ faq;
        struct {
            unsigned short ioa16;
            FAQ obj[1];
        } faq;

        struct {
            unsigned short ioa16;
            FAQ obj;
        } nfaq[1];
        //PAN pan;
        struct {
            unsigned short ioa16;
            PAN obj[1];
        } pan;

        struct {
            unsigned short ioa16;
            PAN obj;
        } npan[1];
        //FRQ frq;
        struct {
            unsigned short ioa16;
            FRQ obj[1];
        } frq;

        struct {
            unsigned short ioa16;
            FRQ obj;
        } nfrq[1];
        //SRQ srq;
        struct {
            unsigned short ioa16;
            SRQ obj[1];
        } srq;

        struct {
            unsigned short ioa16;
            SRQ obj;
        } nsrq[1];
        //FSN fsn;
        struct {
            unsigned short ioa16;
            FSN obj[1];
        } fsn;

        struct {
            unsigned short ioa16;
            FSN obj;
        } nfsn[1];
        //AFQ afq;
        struct {
            unsigned short ioa16;
            AFQ obj[1];
        } afq;

        struct {
            unsigned short ioa16;
            AFQ obj;
        } nafq[1];

    }uinfo;
};

struct ASDU
{
    unsigned char len;
    ASDUH asduh;
    ASDUINFO1 asduinfo;
};


class DataCache
{
public:
    DataCache();
    ASDU DataFormatConver();//原意是数据转化为ASDU结构
    void WriteYXVec(const ASDU &asdu);//向遥信vector中写入数据
    //ASDU ReadYXVec();//取遥信数据的接口函数
    vector<ASDU> yxVec_;
    vector<ASDU> ycVec_;


};

#endif // DATACACHE_H
