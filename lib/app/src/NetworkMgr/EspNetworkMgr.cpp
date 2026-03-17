#include "NetworkMgr/EspNetworkMgr.h"

#include <WiFi.h>
#include <WiFiClientSecure.h>

#include <memory>
#include <sstream>
#include <string>

#include "DivaConverter.h"
#include "HardwareSerial.h"
#include "UserSettings.h"
#include "WiFiType.h"
#include "wl/wl.h"

EspNetworkMgr::EspNetworkMgr() {
    this->diva_converter =
        std::make_unique<DivaConverter>("/name-diva-mapping.csv");
}

bool EspNetworkMgr::set_up(const WifiSettings& wifi_settings) {
    Serial.print("Connecting Wifi");

    wl_status_t status =
        WiFi.begin(wifi_settings.SSID.c_str(), wifi_settings.password.c_str());
    int num_retries = 16;
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");

        num_retries--;

        if (num_retries == 0) {
            Serial.println("Failed to connect to Wifi network!");
            return false;
        }
    }
    Serial.println("\nWifi connected.");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    Serial.println("Setting up Wifi client.");

    this->client = std::make_shared<WiFiClientSecure>();
    this->client->setInsecure();

    Serial.println("Finished setting up Wifi client.");
    return true;
}

bool EspNetworkMgr::is_set_up() const {
    return (WiFi.status() == WL_CONNECTED) && (this->client != nullptr);
}

bool EspNetworkMgr::check_connection() const {
    const std::string host = "ping.archlinux.org";

    if (!client->connect(host.c_str(), 443)) {
        return false;
    }

    const std::string footer =
        " HTTP/1.1\r\n"
        "Host: " +
        host + "\r\n" + "Connection: close\r\n\r\n";
    std::string request = "GET /" + footer;

    this->client->print(request.c_str());

    std::stringstream ss;
    while (!this->client->available());

    bool reading = false;
    while (client->available()) {
        char c = client->read();

        // We're skipping everything until the first space,
        // then reading the status code, then breaking from
        // the loop.
        if (c == ' ' && reading == false) {
            reading = true;
            continue;
        } else if (c == ' ' && reading == true) {
            break;
        }

        if (reading) {
            ss << c;
        }
    }

    const std::string status_code = ss.str();
    return status_code == "200";
}

std::shared_ptr<WL::Collection> EspNetworkMgr::get(
    std::shared_ptr<WlSettings> settings) {
    auto collection = std::make_shared<WL::Collection>();

    for (const auto& station : settings->station_config) {
        Serial.print("// TODO: GET ");
        Serial.println(station->get_name().c_str());
    }

    collection->intersect(settings->station_config);
    return collection;
}
