#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>

#define CPPHTTPLIB_OPENSSL_SUPPORT

#include "../include/ArduinoJson.h"
#include "../src/wl/wl.h"
#include "httplib.h"

void fetch_realtime_information_for_diva(uint32_t);

int main() {
    fetch_realtime_information_for_diva(60200874);

    return 0;
}

void fetch_realtime_information_for_diva(uint32_t diva_code) {
    // TODO: better error handling for DIVA code.
    if (!(diva_code >= 60'000'000 && diva_code <= 70'000'000)) {
        throw std::runtime_error("Got bad DIVA code: " +
                                 std::to_string(diva_code));
    }

    auto cli = new httplib::Client("https://www.wienerlinien.at");
    cli->set_follow_location(true);

    std::string path =
        "/ogd_realtime/monitor?diva=" + std::to_string(diva_code);

    httplib::Result res = cli->Get(path);

    if (res.error() != httplib::Error::Success) {
        res.error();
    }
    if (!(200 <= res->status && res->status <= 300)) {
        throw std::runtime_error("Got bad response! Code: " +
                                 std::to_string(res->status));
    }

    JsonDocument doc;
    DeserializationError error = deserializeJson(
        doc, res->body, DeserializationOption::NestingLimit(20));
    if (error) {
        std::cout << error << "\n";
    }

    std::string message = doc["message"]["status"];
    std::cout << message << std::endl;

    delete cli;
}
