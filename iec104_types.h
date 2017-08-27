#ifndef IEC104TYPE_H
#define IEC104TYPE_H

#pragma pack(push,1)

//定义时标
struct CP56Time2a
{
    unsigned short msec;
    unsigned char min :6;
    unsigned char res1 :1;
    unsigned char iv :1;
    unsigned char hour :5;
    unsigned char res2 :2;
    unsigned char su :1;
    unsigned char mday :5;
    unsigned char wday :3;
    unsigned char month :4;
    unsigned char res3 :4;
    unsigned char year :7;
    unsigned char res4 :1;
};

//定义瞬变状态指示值
struct VTI
{
    unsigned char value:7;
    unsigned char transient:1;
};

//定义３２比特串,状态或更改数据单元
struct BIT_STCD {
    union {
        unsigned short  st;
        struct {
            unsigned char st1 :1;
            unsigned char st2 :1;
            unsigned char st3 :1;
            unsigned char st4 :1;
            unsigned char st5 :1;
            unsigned char st6 :1;
            unsigned char st7 :1;
            unsigned char st8 :1;
            unsigned char st9 :1;
            unsigned char st10 :1;
            unsigned char st11 :1;
            unsigned char st12 :1;
            unsigned char st13 :1;
            unsigned char st14 :1;
            unsigned char st15 :1;
            unsigned char st16 :1;
        };
    };
    union {
        unsigned short  cd;
        struct {
            unsigned char cd1 :1;
            unsigned char cd2 :1;
            unsigned char cd3 :1;
            unsigned char cd4 :1;
            unsigned char cd5 :1;
            unsigned char cd6 :1;
            unsigned char cd7 :1;
            unsigned char cd8 :1;
            unsigned char cd9 :1;
            unsigned char cd10 :1;
            unsigned char cd11 :1;
            unsigned char cd12 :1;
            unsigned char cd13 :1;
            unsigned char cd14 :1;
            unsigned char cd15 :1;
            unsigned char cd16 :1;
        };
    };
};

//定义信息元素集
struct SIQ //不带时标的单点信息 M_SP_NA_1
{
    unsigned char SPI:1;
    unsigned char RES:3;
    unsigned char BL:1;
    unsigned char SB:1;
    unsigned char NT:1;
    unsigned char IV:1;
};

struct SIQ_T //带时标的单点信息 M_SP_TB_1
{
    unsigned char SPI:1;
    unsigned char RES:3;
    unsigned char BL:1;
    unsigned char SB:1;
    unsigned char NT:1;
    unsigned char IV:1;
    CP56Time2a time;
};

struct DIQ //不带时标的双点信息　Ｍ_DP_NA_1
{
    unsigned char DPI:2;
    unsigned char RES:2;
    unsigned char BL:1;
    unsigned char SB:1;
    unsigned char NT:1;
    unsigned char IV:1;
};

struct DIQ_T //带时标的双点信息　Ｍ_DP_TB_1
{
    unsigned char DPI:2;
    unsigned char RES:2;
    unsigned char BL:1;
    unsigned char SB:1;
    unsigned char NT:1;
    unsigned char IV:1;
    CP56Time2a time;
};

struct SLI //不带时标的步位置信息　ＭS_T_NA_1
{
    VTI vti;
    unsigned char OV:1;
    unsigned char RES:3;
    unsigned char BL:1;
    unsigned char SB:1;
    unsigned char NT:1;
    unsigned char IV:1;
};

struct SLI_T // 带时标的步位置信息 M_ST_TB_1
{
    VTI vti;
    unsigned char OV:1;
    unsigned char RES:3;
    unsigned char BL:1;
    unsigned char SB:1;
    unsigned char NT:1;
    unsigned char IV:1;
    CP56Time2a time;
};

struct BSI //32比特串 M_BO_NA_1
{
    BIT_STCD bit_stcd;
    unsigned char OV:1;
    unsigned char RES:3;
    unsigned char BL:1;
    unsigned char SB:1;
    unsigned char NT:1;
    unsigned char IV:1;
};

struct BSI_T  //带时标的３２比特串 M_BO_TB_1
{
    BIT_STCD bit_stcd;
    unsigned char OV:1;
    unsigned char RES:3;
    unsigned char BL:1;
    unsigned char SB:1;
    unsigned char NT:1;
    unsigned char IV:1;
    CP56Time2a time;
};

struct NVA //定义归一化值　Ｍ_ME_NA_1
{
    short mv; // 归一化值
    unsigned char OV :1;
    unsigned char RES :3;
    unsigned char BL :1;
    unsigned char SB :1;
    unsigned char NT :1;
    unsigned char IV :1;
};

struct NVA_T //带时标的归一化值 Ｍ_ME_TD_1
{
    short mv; // 归一化值
    unsigned char OV :1;
    unsigned char RES :3;
    unsigned char BL :1;
    unsigned char SB :1;
    unsigned char NT :1;
    unsigned char IV :1;
    CP56Time2a time;
};

struct SVA //定义标度化值　Ｍ_ME_NＢ_1
{
    short mv; // 标度化值
    unsigned char OV :1;
    unsigned char RES :3;
    unsigned char BL :1;
    unsigned char SB :1;
    unsigned char NT :1;
    unsigned char IV :1;
};

struct SVA_T //带时标的标度化值 Ｍ_ME_TE_1
{
    short mv; // 标度化值
    unsigned char OV :1;
    unsigned char RES :3;
    unsigned char BL :1;
    unsigned char SB :1;
    unsigned char NT :1;
    unsigned char IV :1;
    CP56Time2a time;
};

struct STD //定义短浮点值　Ｍ_ME_NC_1
{
    float mv; // 浮点数值
    unsigned char OV :1;
    unsigned char RES :3;
    unsigned char BL :1;
    unsigned char SB :1;
    unsigned char NT :1;
    unsigned char IV :1;
};

struct STD_T //带时标的短浮点数Ｍ_ME_TF_1
{
    float mv; // 短浮点数
    unsigned char OV :1;
    unsigned char RES :3;
    unsigned char BL :1;
    unsigned char SB :1;
    unsigned char NT :1;
    unsigned char IV :1;
    CP56Time2a time;
};

struct BCR // 累计量  M_IT_NA_1
{
    int counter :32;
    unsigned char SQ :5;
    unsigned char CY :1;
    unsigned char CA :1;
    unsigned char IV :1;
};
struct BCR_T // 带时标的累计量  M_IT_TB_1
{
    int counter :32;
    unsigned char SQ :5;
    unsigned char CY :1;
    unsigned char CA :1;
    unsigned char IV :1;
    CP56Time2a time;
};

//在控制方向过程信息的信息元素

struct SCO //单命令　Ｃ_SC_NA_1
{
    unsigned char SCS :1;
    unsigned char RES :1;
    unsigned char QU :5;
    unsigned char ES :1;
};

struct DCO //双命令 C_DC_NA_1
{
    unsigned char DCS :2;
    unsigned char QU :5;
    unsigned char ES :1;
};

struct RCO //步调节命令　Ｃ_RC_NA_1
{
    unsigned char RCS :2;
    unsigned char QU :5;
    unsigned char ES :1;
};

struct QOS_NVA //　设定命令，规一化值 C_SE_NA_1
{
    short mv;
    unsigned char QL :7;
    unsigned char ES :1;
};

struct QOS_SVA //　设定命令，标度化值 C_SE_NＢ_1
{
    short mv;
    unsigned char QL :7;
    unsigned char ES :1;
};

struct QOS_FLOAT //　设定命令，短浮点数 C_SE_NＣ_1
{
    float mv;
    unsigned char QL :7;
    unsigned char ES :1;
};

struct BSI_C //控制方向32比特串　Ｃ_BO_NA_1
{
    BIT_STCD bit_stcd;
};

//在监视方向系统信息的应用服务数据单元

struct COI   //初始化结束　M_EI_NA_1
{
    unsigned char CUASE:7;
    unsigned char CHANGE:1;
};

//在控制方向系统信息的应用服务数据单元
struct QOI //召唤命令 C_IC_NA_1
{
    unsigned char qoi;
};

struct QCC //计数量召唤命令 C_CI_NA_1
{
    unsigned char RQT :6;
    unsigned char FRZ :2;
};

struct CSC //时钟同步命令　C_CS_NA_1
{
    CP56Time2a time;
};

struct FBP  //测试命令　C_TS_NA_1  = 0x55AA
{
    unsigned short VFB;
};

struct GRP  //复位进程命令　C_RP_NA_1
{
    unsigned char VGR;
};

struct DGC  //延时获得命令 C_CD_NA_1
{
    CP56Time2a time;
};

//在控制方向参数的应用服务数据单元
struct NAQ  //测量参数值，规一化值　P_ME_NA_1
{
    unsigned short NVA;
    unsigned char KPA :6;
    unsigned char LPC :1;
    unsigned char POP :1;
};

struct SAQ  //测量值参数，标度化值 P_ME_NB_1
{
    unsigned short NSA;
    unsigned char KPA :6;
    unsigned char LPC :1;
    unsigned char POP :1;
};

struct FAQ  //测量值参数，短浮点数 P_ME_NC_1
{
    float NFA;
    unsigned char KPA :6;
    unsigned char LPC :1;
    unsigned char POP :1;
};

struct PAN //参数激活 P_AC_NA_1
{
    unsigned char QPA;
};

struct FRQ //文件准备就绪 F_FR_NA_1
{
    unsigned short NOF;
    unsigned short LOFH;
    unsigned char LOFL;
    unsigned char U17 :7;
    unsigned char BS1 :1;
};

struct SRQ //节准备就绪 F_SR_NA_1
{
    unsigned short NOF;
    unsigned char NOS;
    unsigned short LOFH;
    unsigned char LOFL;
    unsigned char U17 :7;
    unsigned char BS1 :1;
};

struct FSN //召唤目录，选择文件，召唤文件，召唤节 F_SC_NA_1
{
    unsigned short NOF;
    unsigned char NOS;
    unsigned char U17 :7;
    unsigned char BS1 :1;
};

struct AFQ //认可文件，认可节 F_AF_NA_1
{
    unsigned short NOF;
    unsigned char NOS;
    unsigned char HUI :4;
    unsigned char LUI :4;
};


//定义ＡＰＣＩ

struct APCI
{
    unsigned char start;
    unsigned char lenth;
    unsigned short NS;
    unsigned short NR;
};

//定义ＡＳＤＵ头

//定义数据单元标识符
struct ASDUH
{
    unsigned char ti;
    unsigned char number :7;
    unsigned char sq :1;
    unsigned short cot;
    unsigned short comAddr;
};

//定义信息对象
struct ASDUINFO
{
    union
    {
        unsigned char dados[255];
        //SIQ siq;
        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            SIQ obj[1];
        } siq;

        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            SIQ obj;
        } nsiq[1];
        // SIQ_T siq_t;
        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            SIQ_T obj[1];
        } siq_t;

        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            SIQ_T obj;
        } nsiq_t[1];
        // DIQ diq;
        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            DIQ obj[1];
        } diq;

        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            DIQ obj;
        } ndiq[1];
        
        //DIQ_T diq_t;
        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            DIQ_T obj[1];
        } diq_t;

        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            DIQ_T obj;
        } ndiq_t[1];
        //SLI sli;
        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            SLI obj[1];
        } sli;

        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            SLI obj;
        } nsli[1];
        //SLI_T sli_t;
        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            SLI_T obj[1];
        } sli_t;

        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            SLI_T obj;
        } nsli_t[1];
        //BSI bsi;
        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            BSI obj[1];
        } bsi;

        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            BSI obj;
        } nbsi[1];
        //BSI_T bsi_t;
        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            BSI_T obj[1];
        } bsi_t;

        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            BSI_T obj;
        } nbsi_t[1];
        //NVA nva;
        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            NVA obj[1];
        } nva;

        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            NVA obj;
        } nnva[1];
        //NVA_T nva_t;
        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            NVA_T obj[1];
        } nva_t;

        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            NVA_T obj;
        } nnva_t[1];
        //SVA sva;
        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            SVA obj[1];
        } sva;

        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            SVA obj;
        } nsva[1];
        //SVA_T sva_t;
        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            SVA_T obj[1];
        } sva_t;

        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            SVA_T obj;
        } nsva_t[1];
        //STD std;
        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            STD obj[1];
        } std;

        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            STD obj;
        } nstd[1];
        //STD_T std_t;
        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            STD_T obj[1];
        } std_t;

        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            STD_T obj;
        } nstd_t[1];
        //BCR brc;
        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            BCR obj[1];
        } brc;

        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            BCR obj;
        } nbrc[1];
        //BCR_T brc_t;
        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            BCR_T obj[1];
        } brc_t;

        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            BCR_T obj;
        } nbrc_t[1];
        //SCO sco;
        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            SCO obj[1];
        } sco;

        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            SCO obj;
        } nsco[1];
        //DCO dco;
        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            DCO obj[1];
        } dco;

        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            DCO obj;
        } ndco[1];
        //RCO rco;
        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            RCO obj[1];
        } rco;

        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            RCO obj;
        } nrco[1];
        //QOS_NVA qos_nva;
        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            QOS_NVA obj[1];
        } qos_nva;

        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            QOS_NVA obj;
        } nqos_nva[1];
        //QOS_SVA qos_sva;
        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            QOS_SVA obj[1];
        } qos_sva;

        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            QOS_SVA obj;
        } nqos_sva[1];
        //QOS_FLOAT qos_float;
        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            QOS_FLOAT obj[1];
        } qos_float;

        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            QOS_FLOAT obj;
        } nqos_float[1];
        //BSI_C bsi_c;
        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            BSI_C obj[1];
        } bsi_c;

        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            BSI_C obj;
        } nbsi_c[1];
        //COI coi;
        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            COI obj[1];
        } coi;

        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            COI obj;
        } ncoi[1];
        //QOI qoi;
        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            QOI obj[1];
        } qoi;

        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            QOI obj;
        } nqoi[1];
        //QCC qcc;
        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            QCC obj[1];
        } qcc;

        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            QCC obj;
        } nqcc[1];
        //CSC csc;
        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            CSC obj[1];
        } csc;

        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            CSC obj;
        } ncsc[1];
        //FBP fbp;
        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            FBP obj[1];
        } fbp;

        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            FBP obj;
        } nfbp[1];
        //GRP grp;
        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            GRP obj[1];
        } grp;

        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            GRP obj;
        } ngrp[1];
        //DGC dgc;
        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            DGC obj[1];
        } dgc;

        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            DGC obj;
        } ndgc[1];
        //NAQ naq;
        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            NAQ obj[1];
        } naq;

        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            NAQ obj;
        } nnaq[1];
        //SAQ saq;
        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            SAQ obj[1];
        } saq;

        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            SAQ obj;
        } nsaq[1];
        //FAQ faq;
        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            FAQ obj[1];
        } faq;

        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            FAQ obj;
        } nfaq[1];
        //PAN pan;
        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            PAN obj[1];
        } pan;

        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            PAN obj;
        } npan[1];
        //FRQ frq;
        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            FRQ obj[1];
        } frq;

        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            FRQ obj;
        } nfrq[1];
        //SRQ srq;
        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            SRQ obj[1];
        } srq;

        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            SRQ obj;
        } nsrq[1];
        //FSN fsn;
        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            FSN obj[1];
        } fsn;

        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            FSN obj;
        } nfsn[1];
        //AFQ afq;
        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            AFQ obj[1];
        } afq;

        struct {
            unsigned short ioa16;
            unsigned char ioa8;
            AFQ obj;
        } nafq[1];

    }uinfo;

};

struct APDU
{
    APCI apci;
    ASDUH asduh;
    ASDUINFO asduinfo;
};

#pragma pack(pop)


#endif // IEC104TYPE_H
