#include "AppMain.h"

#include <Arduino.h>
#include <XPT2046_Touchscreen.h>
#include <lvgl.h>

#include <cstddef>
#include <memory>
#include <stdexcept>

#include "DisplayMgr/DisplayMgr.h"
#include "NetworkMgr/EspNetworkMgr.h"
#include "UserSettings.h"

AppMain::AppMain(std::shared_ptr<DivaConverter> diva_converter) {
    Serial.begin(115200);
    Serial.println("\n\nInitializing AppMain.");

    this->diva_converter = diva_converter;

    DisplayMgr::init();

    std::optional<WifiSettings> wifi_settings =
        WifiSettings::parse_from_file("/wifi-settings.json");

    if (wifi_settings.has_value()) {
        Serial.println("Using stored wifi credentials:");
        WifiSettings ws = wifi_settings.value();

        Serial.print("  SSID: ");
        Serial.println(ws.SSID.c_str());
        Serial.print("  Password: ");
        for (size_t i{0}; i < ws.password.length(); ++i) {
            Serial.print("*");
        }
        Serial.println("");

        this->wifi_settings = std::make_shared<WifiSettings>(ws);
    } else {
        Serial.println("No wifi credentials to load.");
        throw std::runtime_error("// TODO: Implement way to set wifi creds. ");
    }

    if (this->wifi_settings != nullptr) {
        this->network_mgr = std::make_unique<EspNetworkMgr>();
        bool set_up_nm =
            this->network_mgr->set_up(*(this->get_wifi_settings()));
        if (!set_up_nm) {
            Serial.println(
                "Failed to set up Network manager. Maybe we have false "
                "credentials?");
        } else {
            bool conn_works = this->network_mgr->check_connection();
            if (conn_works) {
                Serial.println("Connection test successful!");
            }
        }
    }
}

std::shared_ptr<WifiSettings> AppMain::get_wifi_settings() const {
    return this->wifi_settings;
}

void AppMain::loop() const { DisplayMgr::loop(); }
