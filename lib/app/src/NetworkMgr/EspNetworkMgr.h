#ifndef ESPNETWORKMGR_H
#define ESPNETWORKMGR_H

#include <WiFi.h>
#include <WiFiClientSecure.h>

#include <memory>

#include "DivaConverter.h"
#include "NetworkMgr/NetworkMgr.h"
#include "UserSettings.h"

class EspNetworkMgr : public NetworkMgr {
   private:
    std::shared_ptr<WiFiClientSecure> client;
    std::unique_ptr<DivaConverter> diva_converter;

   public:
    EspNetworkMgr();
    bool set_up(const WifiSettings&);
    bool is_set_up() const;
    bool check_connection() const;
    std::shared_ptr<WL::Collection> get(std::shared_ptr<WlSettings>);
};

#endif
