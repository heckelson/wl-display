#ifndef USER_SETTINGS_H
#define USER_SETTINGS_H

#include <map>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "wl/wl.h"

struct WifiSettings {
    std::string SSID;
    std::string password;
};

struct WlSettings {
    WL::Collection station_config;
};

#endif
