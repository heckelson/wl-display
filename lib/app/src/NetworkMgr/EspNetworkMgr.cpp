#include "NetworkMgr/EspNetworkMgr.h"

#include <WiFi.h>
#include <WiFiClientSecure.h>

#include <memory>
#include <sstream>
#include <stdexcept>
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

    this->client->println("GET / HTTP/1.1");
    this->client->print("Host: ");
    this->client->println(host.c_str());
    this->client->println("Connection: close\r\n\r\n");

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

    this->client->stop();

    const std::string status_code = ss.str();
    return status_code == "200";
}

std::shared_ptr<WL::Collection> EspNetworkMgr::get(
    std::shared_ptr<WlSettings> settings) {
    const std::string host = "www.wienerlinien.at";
    if (!client->connect(host.c_str(), 443)) {
        Serial.println("Cannot connect!!");
        return nullptr;
    }

    if (settings->station_config.get_stations().size() == 0) {
        Serial.println("Warning: No stations configured.");
        return nullptr;
    }

    std::string request = "GET /ogd_realtime/monitor?";
    bool first = true;
    Serial.println("Fetching stations: ");
    for (const auto& station : settings->station_config) {
        Serial.print("- ");
        Serial.println(station->get_name().c_str());

        std::string diva_code =
            this->diva_converter->get_diva_by_name(station->get_name());

        if (diva_code == "") {
            Serial.println("Error: Found invalid station:");
            Serial.println(station->get_name().c_str());
        }

        if (!first) request += "&";
        first = false;
        request += "diva=" + diva_code;
    }

    request +=
        " "
        "HTTP/1.1\r\n"
        "Host: www.wienerlinien.at\r\n"
        "Connection: close\r\n\r\n";

    Serial.println(request.c_str());
    this->client->print(request.c_str());

    delay(200);  // this seems to help with stuff not getting stuck.
    auto before = millis();
    while (!client->available() || millis() > before + 10'000) {
        delay(100);
    }

    if (!client->available()) {
        Serial.println("Connection timed out after 10s.");
        return nullptr;
    }

    Serial.println("Reading response.");
    std::stringstream ss;
    while (client->available()) {
        char c = client->read();
        ss << c;
    }

    this->client->stop();

    std::string response = ss.str();

    std::string::size_type sep = response.find(' ');
    if (sep == std::string::npos) {
        throw std::runtime_error("Cannot find status in response.");
    }
    std::string response_code = response.substr(sep + 1, 3);
    if (response_code != "200") {
        Serial.print("Error: got an invalid response code: ");
        Serial.println(response_code.c_str());
        return nullptr;
    }

    // fetch just the body.
    std::string::size_type start_of_body = response.find("\r\n\r\n");
    response = response.substr(start_of_body + 4, std::string::npos);

    std::shared_ptr<WL::Collection> resp_data =
        WL::deserialize_json_response(response);
    resp_data->intersect(settings->station_config);

    std::stringstream cfg;
    cfg << settings->station_config;
    Serial.println(cfg.str().c_str());

    std::stringstream rrrr;
    rrrr << *resp_data;
    Serial.println(rrrr.str().c_str());

    return resp_data;
}
