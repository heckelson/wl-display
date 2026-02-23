#ifndef _NETWORKMGR_H_
#define _NETWORKMGR_H_

#include <cstdint>
#include <string>

struct NetworkOptions {
    std::string SSID;
    std::string passwd;
};

class NetworkMgr {
   public:
    virtual std::string fetch_station_info_by_diva(uint32_t) = 0;
};

#endif
