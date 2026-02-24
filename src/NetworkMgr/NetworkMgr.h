#ifndef _NETWORKMGR_H_
#define _NETWORKMGR_H_

#include <cstdint>
#include <string>

#include "../AppMain/UserSettings.h"
#include "../wl/wl.h"

class NetworkMgr {
   public:
    virtual std::string fetch_station_info_by_diva(uint32_t) = 0;
    virtual WL::Collection get(WlSettings&);
};

#endif
