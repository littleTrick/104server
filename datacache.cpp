#include "datacache.h"
#include "logging.h"

DataCache::DataCache()
{

}

void DataCache::WriteYXVec(const ASDU &asdu)
{
    yxVec_.push_back(asdu);
}

//ASDU DataCache::ReadYXVec()
//{
//    if(!yxVec_.size())
//    {
//        LOG(WARN) << "-->YX vector is null";
//    }
//    else
//    {
//        for(int i = 0;i < yxVec_.size();i++)
//        {
//            yxVec_[i];
//        }
//    }
//}

