#include "AppMain.h"

#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

#include "UserSettings.h"

AppMain::AppMain(std::unique_ptr<NetworkMgr> network_mgr) {
    if (network_mgr == nullptr) {
        throw std::runtime_error("Cannot init with null network_mgr!");
    }

    this->user_settings = std::make_unique<UserSettings>();
}

void AppMain::add_entry_to_wl_settings(std::string station_name,
                                       std::string line,
                                       std::string direction) {
    auto insert_pair = std::make_pair(line, direction);

    auto& station_cfg = this->user_settings->wl_settings.station_config;
    auto maybe_station = station_cfg.find(station_name);

    if (maybe_station != station_cfg.end()) {
        // check that we don't have the pair yet!!
        for (auto p : station_cfg[station_name]) {
            if (p == insert_pair) return;
        }
        station_cfg[station_name].push_back(insert_pair);
    } else {
        station_cfg[station_name] = {insert_pair};
    }
}

const UserSettings& AppMain::get_user_settings() const {
    return *this->user_settings;
}

void AppMain::update_stations() {
    // 1. Lookup the station DIVA codes.

    // 2. Fetch the information for each station.

    // 3. Write into this->stations.

}
