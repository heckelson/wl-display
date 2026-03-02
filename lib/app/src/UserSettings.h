#ifndef _USER_SETTINGS_H_
#define _USER_SETTINGS_H_

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
