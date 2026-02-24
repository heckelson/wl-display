#include <algorithm>
#include <memory>

#include "AppMain/UserSettings.h"
#include "NetworkMgr/NetworkMgr.h"
#include "httplib.h"
#include "wl/wl.h"

class TestNetworkMgr : public NetworkMgr {
   private:
    std::unique_ptr<httplib::Client> client;

   public:
    TestNetworkMgr();
    TestNetworkMgr(WifiSettings&);

    [[deprecated]]
    std::string fetch_station_info_by_diva(uint32_t);

    bool check_connection() const;
    WL::Collection get(WlSettings&);
};
