#include "wl.h"

#include <iostream>
#include <memory>
#include <vector>

namespace WL {

/* Departure */

Departure::Departure(duration_time_t t) : duration{t} {}

duration_time_t Departure::getDuration() const { return this->duration; }

/* Direction */

Direction::Direction(std::string new_name) : name{new_name} {}

std::string Direction::get_name() const { return this->name; }

std::vector<departure_t> Direction::get_departures() const {
    return this->departures;
}

void Direction::add_departure(const Departure& t) {
    this->departures.push_back(std::make_shared<Departure>(t));
}

/* Line */

Line::Line(std::string new_name) : name{new_name} {}

std::string Line::get_name() const { return this->name; }

std::vector<direction_t> Line::get_directions() const {
    return this->directions;
}

direction_t Line::get_direction_by_name(
    const std::string direction_name) const {
    // this is fine since we only have a bunch of em
    for (auto d : this->directions) {
        if (d->get_name() == direction_name) {
            return d;
        }
    }
    return nullptr;
}

void Line::add_direction(const Direction& dir) {
    this->directions.push_back(std::make_shared<Direction>(dir));
}

/* Station */

Station::Station(std::string new_name) : name{new_name} {}

std::string Station::get_name() const { return this->name; }

std::vector<line_t> Station::get_lines() const { return this->lines; }

void Station::add_line(const Line& line) {
    this->lines.push_back(std::make_shared<Line>(line));
}

line_t Station::get_line_by_name(std::string station_name) const {
    for (auto s : this->lines) {
        if (s->get_name() == station_name) {
            return s;
        }
    }
    return nullptr;
}

}  // namespace WL
