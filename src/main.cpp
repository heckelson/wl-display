#include <memory>
#include <optional>
#include <string>

#include "AppMain/AppMain.h"
#include "DivaConverter.h"
#include "NetworkMgr/EspNetworkMgr.h"
#include "NetworkMgr/NetworkMgr.h"
#include "UserSettings.h"
#include "esp32-hal.h"

void setup() {
    delay(1000);

    AppMain* app_main = nullptr;
    std::shared_ptr<DivaConverter> diva_converter =
        std::make_shared<DivaConverter>("/name-diva-mapping.csv");

    app_main = new AppMain(diva_converter);

    EspNetworkMgr mgr = EspNetworkMgr();

    std::optional<WifiSettings> settings = WifiSettings::parse_from_file("/wifi-settings.json");
    if (settings.has_value()) {
        Serial.println(settings.value().SSID.c_str());
        Serial.println(settings.value().password.c_str());

        Serial.println(mgr.set_up(settings.value()));
    }

}

void loop() { delay(500); }
