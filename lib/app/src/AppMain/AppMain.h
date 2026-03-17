#ifndef APPMAIN_H
#define APPMAIN_H

#include <algorithm>
#include <memory>
#include <utility>
#include <vector>

#include "DivaConverter.h"
#include "NetworkMgr/NetworkMgr.h"
#include "UserSettings.h"
#include "wl/wl.h"

class AppMain {
   private:
    // has to be set up after the fact!
    std::unique_ptr<NetworkMgr> network_mgr;

    std::vector<std::shared_ptr<WL::Station>> last_response;

    // dependency injected
    std::shared_ptr<DivaConverter> diva_converter;

    std::shared_ptr<WlSettings> wl_settings;
    std::shared_ptr<WifiSettings> wifi_settings;

   public:
    AppMain();

    void add_entry_to_wl_settings(std::string, std::string, std::string);
    void update_stations();

    std::shared_ptr<WifiSettings> get_wifi_settings() const;

    void loop(void*) const;
};

#endif
