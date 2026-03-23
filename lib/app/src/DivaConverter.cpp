#include "DivaConverter.h"

#include <FS.h>
#include <SPIFFS.h>

#include <cstdint>
#include <cstdio>
#include <stdexcept>
#include <string>
#include <utility>

constexpr int max_cache_size = 10;

DivaConverter::DivaConverter(const std::string& diva_lut_filename) {
    SPIFFS.begin(true);

    if (diva_lut_filename == "") {
        Serial.println("DIVA LUT filename missing!");
        throw std::runtime_error("DIVA LUT filename missing!");
    }
    this->diva_lut_filename = diva_lut_filename;

    Serial.print("Initializing DivaConverter with file ");
    Serial.print(diva_lut_filename.c_str());
    Serial.println(".");
}

std::string DivaConverter::get_diva_by_name(const std::string& station_name) {
    if (auto found = this->diva_cache.find(station_name);
        found != this->diva_cache.end()) {
        return found->second;
    }

    std::pair<std::string, std::string> pair =
        this->fetch_from_file_by_name(station_name);

    if (this->diva_cache.size() >= max_cache_size) {
        this->diva_cache.clear();
    }

    this->diva_cache.insert({pair.first, pair.second});
    this->diva_cache.insert({pair.second, pair.first});

    return pair.first;
}

std::string DivaConverter::get_name_by_diva(const std::string& diva) {
    if (auto found = this->diva_cache.find(diva);
        found != this->diva_cache.end()) {
        return found->second;
    }
    std::pair<std::string, std::string> pair =
        this->fetch_from_file_by_diva(diva);

    if (this->diva_cache.size() >= max_cache_size) {
        this->diva_cache.clear();
    }

    this->diva_cache.insert({pair.first, pair.second});
    this->diva_cache.insert({pair.second, pair.first});

    return pair.second;
}

std::string DivaConverter::get_name_by_diva(uint32_t diva_code) {
    return this->get_diva_by_name(std::to_string(diva_code));
}

std::pair<std::string, std::string> DivaConverter::fetch_from_file_by_name(
    const std::string& name) {
    File file = SPIFFS.open(this->diva_lut_filename.c_str(), "r");
    if (!file.available()) {
        Serial.print("Cannot find file: ");
        Serial.println(this->diva_lut_filename.c_str());
        // (diva, name)
        return std::make_pair("", "");
    }

    char c = '\0';

    for (size_t i = 0; i < file.size(); i++) {
        std::string cur_diva;
        std::string cur_name;

        while (c != ';') {
            file.readBytes(&c, 1);

            if (c != ';') {
                cur_diva += c;
            }
        }
        while (c != '\n') {
            file.readBytes(&c, 1);

            if (c != '\n') {
                cur_name += c;
            }
        }

        if (cur_name == name) {
            return std::make_pair(cur_diva, cur_name);
        }
    }

    // (diva, name)
    return std::make_pair("", "");
}

std::pair<std::string, std::string> DivaConverter::fetch_from_file_by_diva(
    const std::string& diva) {
    File file = SPIFFS.open(this->diva_lut_filename.c_str(), "r");
    if (!file.available()) {
        Serial.print("ERROR: Cannot find file: ");
        Serial.println(this->diva_lut_filename.c_str());
        // (diva, name)
        return std::make_pair("", "");
    }

    char c = '\0';

    for (size_t i = 0; i < file.size(); i++) {
        std::string cur_diva;
        std::string cur_name;

        while (c != ';') {
            file.readBytes(&c, 1);

            if (c != ';') {
                cur_diva += c;
            }
        }
        while (c != '\n') {
            file.readBytes(&c, 1);

            if (c != '\n') {
                cur_name += c;
            }
        }

        if (cur_diva == diva) {
            return std::make_pair(cur_diva, cur_name);
        }
    }

    // (diva, name)
    return std::make_pair("", "");
}
