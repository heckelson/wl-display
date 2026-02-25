#ifndef _USER_SETTINGS_H_
#define _USER_SETTINGS_H_

#include <map>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

struct WifiSettings {
    std::string SSID;
    std::string password;
};

struct WlSettings {
    using station_name_t = std::string;
    using line_name_t = std::string;
    using direction_name_t = std::string;

    // This results in something like:
    //  {
    //      Palffygasse -> [
    //          (43, Neuwaldegg),
    //          (43, Schottentor),
    //      ],
    //      Michelbeuern AKH -> [
    //          (42, Gersthof),
    //          (U6, Floridsdorf),
    //      ],
    // }
    // TODO: Replace this with a WL::Collection.
    std::map<station_name_t,
             std::vector<std::pair<line_name_t, direction_name_t>>>
        station_config;
};

struct UserSettings {
    WifiSettings wifi_settings;
    WlSettings wl_settings;
};

#endif
