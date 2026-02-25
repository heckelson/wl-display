#include "DivaConverter.h"

#include <cstddef>
#include <cstdint>
#include <fstream>
#include <stdexcept>
#include <string>
#include <utility>

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
    std::ifstream file;
    file.open(this->diva_lut_filename);

    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " +
                                 this->diva_lut_filename);
    }

    size_t file_size = file.tellg();
    size_t line_begin, line_sep, line_end;
    line_begin = line_sep = line_end = 0;

    while (line_begin < file_size) {
        std::string cur_diva_code;
        std::string cur_name;

        char sep = '\0';
        char end = '\0';

        while (sep != '\n') {
            ++line_sep;
            ++line_end;

            file.seekg(line_sep);
            file.read(&sep, 1);
        }

        while (end != '\n') {
            ++line_end;
            file.read(&end, 1);
        }

        // read both elements into the strings.
        for (size_t i = line_begin; i < line_sep; i++) {
            file.seekg(i);
            char c;
            file.read(&c, 1) >> cur_diva_code;
        }

        for (size_t i = line_sep + 1; i < line_end; i++) {
            file.seekg(i);
            char c;
            file.read(&c, 1) >> cur_name;
        }

        if (name == cur_name) {
            return std::make_pair(cur_diva_code, cur_name);
        }
    }

    throw std::runtime_error("Cannot find station named " + name + " in list.");
}
