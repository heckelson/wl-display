#ifndef USER_SETTINGS_H
#define USER_SETTINGS_H

#include <ArduinoJson.h>
#include <FS.h>
#include <SPIFFS.h>

#include <algorithm>
#include <map>
#include <optional>
#include <ostream>
#include <sstream>
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

    WifiSettings() {}
    WifiSettings(const std::string& SSID, const std::string& password) {
        this->SSID = SSID;
        this->password = password;
    }
    WifiSettings(const WifiSettings& other) {
        this->SSID = other.SSID;
        this->password = other.password;
    }

    static std::optional<WifiSettings> parse_from_file(
        const std::string& filepath) {
        SPIFFS.begin(true);

        File file = SPIFFS.open(filepath.c_str(), "r");

        if (!file.available()) {
            Serial.println("Cannot find the WifiSettings file.");
            return std::nullopt;
        }

        std::string input = "";
        while (file.available()) {
            char c = file.read();
            input += c;
        }

        file.close();

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

        return WifiSettings{.SSID = std::string{ssid},
                            .password = std::string{password}};
    }

    void serialize(const std::string& filepath) {
        Serial.println("Saving Wifi credentials.");

        SPIFFS.begin(true);
        File file = SPIFFS.open(filepath.c_str(), "w");

        if (!file) {
            Serial.println("Cannot open file.");
            throw std::runtime_error("Cannot open file " + filepath +
                                     " for writing.");
        }

        JsonDocument doc;
        std::string result;
        doc["ssid"] = this->SSID;
        doc["password"] = this->password;
        serializeJson(doc, result);

        file.print(result.c_str());

        file.close();
    }
};

struct WlSettings {
    WL::Collection station_config;

    static std::optional<WlSettings> parse_from_file(
        const std::string& filename) {
        SPIFFS.begin(true);

        File file = SPIFFS.open(filepath.c_str(), "r");

        if (!file.available()) {
            Serial.println("Cannot find the WlSettings file.");
            return std::nullopt;
        }

        std::string input = "";
        while (file.available()) {
            char c = file.read();
            input += c;
        }

        file.close();

        WL::Collection coll = WL::deserialize_settings_json(input);

        if (coll.get_stations().size() == 0) {
            Serial.println("Collection settings are empty!");
            return std::nullopt;
        }
    }

    void serialize(const std::string& filepath) {
        Serial.println("Saving WlSettings.");

        SPIFFS.begin(true);
        File file = SPIFFS.open(filepath.c_str(), "w");

        if (!file) {
            Serial.println("Cannot open file.");
            throw std::runtime_error("Cannot open file " + filepath +
                                     " for writing.");
        }

        JsonDocument doc;
        std::string result;
        doc["station_config"] = this->station_config.serialize();
        serializeJson(doc, result);

        file.print(result.c_str());
        file.close();
    }
};

#endif
