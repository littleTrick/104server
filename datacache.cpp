#include "datacache.h"
#include "logging.h"

extern DataCache g_dataCache;

DataCache::DataCache()
{

}

void DataCache::WriteYXVec(const ASDU &asdu)
{
    yxVec_.push_back(asdu);
}

void DataCache::WriteYCVec(const ASDU &asdu)
{
    ycVec_.push_back(asdu);
}

void DataCache::AutoSendSOE(const ASDU &asdu)
{

    SOEVec_.push_back(asdu);
    for(size_t i = 0; i < server_.size(); i++)
    {
        server_[i]->sendSOE();//向每个主站发送SOE
    }
    vector <ASDU>().swap(g_dataCache.SOEVec_);//清空遥测缓存区并释放内存
}

void DataCache::AutoSendVarYC(const ASDU &asdu)
{
    VarYCVec_.push_back(asdu);
    for(size_t i = 0; i < server_.size(); i++)
    {
        server_[i]->sendVarYC();//向每个被记录的主站发送SOE
    }
    vector <ASDU>().swap(g_dataCache.VarYCVec_);//清空遥测缓存区并释放内存
}

void DataCache::registerObserver(IEC104Parser *parser)
{
    server_.push_back(parser);
}

void DataCache::unregisterObserver(IEC104Parser *parser)
{
    vector <IEC104Parser*>::iterator iter;

    for(iter = server_.begin();iter != server_.end(); iter++)
    {
        if((*iter) == parser)
        {
            server_.erase(iter);
            break;
        }
    }
}

