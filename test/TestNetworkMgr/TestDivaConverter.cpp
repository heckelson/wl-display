#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

#include "NetworkMgr/DivaConverter.h"

DivaConverter::DivaConverter(std::string diva_lut_filename) {
    if (diva_lut_filename == "") {
        throw std::runtime_error("DIVA LUT filename missing!");
    }

    this->diva_lut_filename = diva_lut_filename;
}

std::string DivaConverter::get_diva_by_name(const std::string& station_name) {
    if (auto found = this->diva_cache.find(station_name); found != nullptr) {
        return found->second;
    }
    std::pair<std::string, std::string> pair =
        this->fetch_from_file_by_name(station_name);
    return pair.second;
}

std::string DivaConverter::get_name_by_diva(const std::string& diva) {
    if (auto found = this->diva_cache.find(diva); found != nullptr) {
        return found->second;
    }
    std::pair<std::string, std::string> pair =
        this->fetch_from_file_by_diva(diva);
    return pair.first;
}

std::string DivaConverter::get_name_by_diva(uint32_t diva_code) {
    return this->get_diva_by_name(std::to_string(diva_code));
}

std::pair<std::string, std::string> DivaConverter::fetch_from_file_by_name(
    const std::string& name) {
    throw std::runtime_error("// TODO: Not implemented.");
}

std::pair<std::string, std::string> DivaConverter::fetch_from_file_by_diva(
    const std::string& diva) {
    throw std::runtime_error("// TODO: Not implemented.");
}
