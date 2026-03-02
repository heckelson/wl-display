#ifndef _APPMAIN_H_
#define _APPMAIN_H_

#include <algorithm>
#include <memory>
#include <utility>
#include <vector>

#include "NetworkMgr.h"
#include "wl/wl.h"
#include "UserSettings.h"

class AppMain {
   private:
    std::unique_ptr<NetworkMgr> network_mgr;
    std::vector<std::shared_ptr<WL::Station>> stations;

    std::unique_ptr<WlSettings> wl_settings;
    std::unique_ptr<WifiSettings> wifi_settings;

   public:
    AppMain(std::unique_ptr<NetworkMgr>, std::unique_ptr<WlSettings>,
            std::unique_ptr<WifiSettings>);

    void add_entry_to_wl_settings(std::string, std::string, std::string);
    void update_stations();
};

#endif
