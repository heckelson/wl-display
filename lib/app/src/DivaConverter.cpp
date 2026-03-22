#include "DivaConverter.h"

#include <FS.h>
#include <SPIFFS.h>

#include <cstddef>
#include <cstdint>
#include <fstream>
#include <stdexcept>
#include <string>
#include <utility>

#include "HardwareSerial.h"

// FIXME: The impl does not work on the ESP!!! Must rewrite!

DivaConverter::DivaConverter(std::string diva_lut_filename) {
    if (diva_lut_filename == "") {
        Serial.println("DIVA LUT filename missing!");
        throw std::runtime_error("DIVA LUT filename missing!");
    }

    Serial.print("Initializing DivaConverter with file ");
    Serial.print(diva_lut_filename.c_str());
    Serial.println(".");

    SPIFFS.begin(true);

    Serial.println("Testing fetching of DivaConverter.");

    try {
        Serial.println("Given Palffygasse, what's the diva of the station?");
        std::string diva_code = this->get_diva_by_name("Palffygasse");
        Serial.print("Answer (should be 60200988): ");
        Serial.println(diva_code.c_str());
    } catch (const std::runtime_error& err) {
        Serial.print("ERROR: ");
        Serial.println(err.what());
    }

    Serial.println("Given 60201920, what's the Name of the station?");
    std::string station_name = this->get_name_by_diva("60201920");
    Serial.print("Answer (should be Eduardgasse): ");
    Serial.println(station_name.c_str());

    this->diva_lut_filename = diva_lut_filename;
}

std::string DivaConverter::get_diva_by_name(const std::string& station_name) {
    if (auto found = this->diva_cache.find(station_name);
        found != this->diva_cache.end()) {
        return found->second;
    }

    std::pair<std::string, std::string> pair =
        this->fetch_from_file_by_name(station_name);

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

    this->diva_cache.insert({pair.first, pair.second});
    this->diva_cache.insert({pair.second, pair.first});

    return pair.second;
}

std::string DivaConverter::get_name_by_diva(uint32_t diva_code) {
    return this->get_diva_by_name(std::to_string(diva_code));
}

std::pair<std::string, std::string> DivaConverter::fetch_from_file_by_name(
    const std::string& name) {
    std::ifstream iss{this->diva_lut_filename};

    if (!iss.is_open()) {
        throw std::runtime_error("Cannot open file: " +
                                 this->diva_lut_filename);
    }

    std::string line;
    while (std::getline(iss, line)) {
        size_t sep = line.find(';');
        size_t end = line.length();

        if (sep == end) continue;  // TODO: Warn

        std::string cur_diva = line.substr(0, sep);
        std::string cur_name = line.substr(sep + 1, end - 1);

        if (cur_name == name) {
            return std::make_pair(cur_diva, cur_name);
        }
    }
    return std::make_pair("", "");
}

std::pair<std::string, std::string> DivaConverter::fetch_from_file_by_diva(
    const std::string& diva) {
    std::ifstream iss(this->diva_lut_filename);

    if (!iss.is_open()) {
        throw std::runtime_error("Cannot open file: " +
                                 this->diva_lut_filename);
    }

    std::string line;
    while (std::getline(iss, line)) {
        size_t sep = line.find(';');
        size_t end = line.length();

        if (sep == end) continue;  // TODO: Warn

        std::string cur_diva = line.substr(0, sep);
        std::string cur_name = line.substr(sep + 1, end - 1);

        if (cur_diva == diva) {
            return std::make_pair(cur_diva, cur_name);
        }
    }

    return std::make_pair("", "");
}
