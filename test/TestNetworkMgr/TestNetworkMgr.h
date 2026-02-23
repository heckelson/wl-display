#include <algorithm>
#include <memory>

#include "../../src/NetworkMgr/NetworkMgr.h"
#include "httplib.h"

class TestNetworkMgr : public NetworkMgr {
   private:
    std::unique_ptr<httplib::Client> client;

   public:
    TestNetworkMgr();
    TestNetworkMgr(NetworkOptions);

    std::string fetch_station_info_by_diva(uint32_t);
};
