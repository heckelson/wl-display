#ifndef USER_SETTINGS_H
#define USER_SETTINGS_H

#include <ArduinoJson.h>
#include <FS.h>
#include <SPIFFS.h>

#include <map>
#include <optional>
#include <stdexcept>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "HardwareSerial.h"
#include "wl/wl.h"

struct WifiSettings {
    std::string SSID;
    std::string password;

    static std::optional<WifiSettings> parse_from_file(
        const std::string& filepath) {
        SPIFFS.begin(true);

        File file = SPIFFS.open(filepath.c_str());

        if (!file.available()) {
            Serial.println("Cannot find the WifiSettings file.");
            return std::nullopt;
        }

        std::string input = "";
        while (file.available()) {
            char c = file.read();
            input += c;
        }

        JsonDocument doc;
        deserializeJson(doc, input.c_str());

        const char* ssid = doc["ssid"];
        const char* password = doc["password"];

        if (ssid == nullptr) {
            Serial.println("SSID is empty in settings file.");
            return std::nullopt;
        }
        if (password == nullptr) {
            Serial.println("Password is empty in file.");
            return std::nullopt;
        }

        return WifiSettings{.SSID = std::string{ssid}, .password = std::string{password}};
    }

    void serialize(const std::string& filepath) {
        SPIFFS.begin(true);
        File file = SPIFFS.open(filepath.c_str());


    }
};

struct WlSettings {
    WL::Collection station_config;
};

#endif
