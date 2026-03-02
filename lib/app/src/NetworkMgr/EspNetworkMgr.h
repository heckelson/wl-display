#ifndef ESPNETWORKMGR_H
#define ESPNETWORKMGR_H

#include <WiFi.h>
#include <WiFiClientSecure.h>

#include "NetworkMgr/NetworkMgr.h"
#include "UserSettings.h"

class EspNetworkMgr : public NetworkMgr {
   private:
    std::shared_ptr<WiFiClientSecure> client;

   public:
    bool set_up(const WifiSettings&);
    bool is_set_up() const;
    bool check_connection() const;
    WL::Collection get(WlSettings&);
};

#endif
