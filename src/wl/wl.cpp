#include "wl.h"

#include <algorithm>
#include <iostream>
#include <memory>
#include <ostream>
#include <vector>

#include "ArduinoJson.h"

namespace WL {

// In this whole file, we don't check for duplicates in the vectors
// since we trust Wiener Linien to never provide us any duplicates.

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

void Direction::set_departures(std::vector<departure_t> new_departures) {
    this->departures = new_departures;
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

void Line::add_direction(direction_t dir) { this->directions.push_back(dir); }

void Line::set_directions(std::vector<direction_t> new_directions) {
    this->directions = new_directions;
}

/* Station */

Station::Station(std::string new_name) : name{new_name} {}

std::string Station::get_name() const { return this->name; }

std::vector<line_t> Station::get_lines() const { return this->lines; }

void Station::add_line(line_t line) { this->lines.push_back(line); }

void Station::set_lines(std::vector<line_t> new_lines) {
    this->lines = new_lines;
}

line_t Station::get_line_by_name(std::string station_name) const {
    for (auto s : this->lines) {
        // this is fine since we only have a bunch of em
        if (s->get_name() == station_name) {
            return s;
        }
    }
    return nullptr;
}

/* Collection */

std::vector<std::shared_ptr<Station>> Collection::get_stations() const {
    return this->stations;
}

void Collection::add_station(std::shared_ptr<Station> station) {
    return this->stations.push_back(station);
}

std::shared_ptr<Station> Collection::get_station_by_name(
    std::string station_name) const {
    for (auto s : this->stations) {
        if (s->get_name() == station_name) {
            return s;
        }
    }
    return nullptr;
}

void Collection::intersect(const Collection& other) {
    // intersect Stations
    std::vector<std::shared_ptr<Station>> station_intersection;

    for (const auto& station : this->stations) {
        const auto& maybe_station =
            other.get_station_by_name(station->get_name());
        if (maybe_station == nullptr) {
            continue;
        }

        // intersect Lines
        std::vector<std::shared_ptr<Line>> line_intersection;

        for (const auto& line : station->get_lines()) {
            const auto& maybe_line =
                maybe_station->get_line_by_name(line->get_name());
            if (maybe_line == nullptr) {
                continue;
            }

            // intersect Directions
            std::vector<std::shared_ptr<Direction>> direction_intersection;

            for (const auto& direction : line->get_directions()) {
                const auto& maybe_direction =
                    maybe_line->get_direction_by_name(direction->get_name());

                if (maybe_direction == nullptr) {
                    continue;
                }

                direction_intersection.push_back(direction);
            }
            line->set_directions(direction_intersection);
            line_intersection.push_back(line);
        }
        station->set_lines(line_intersection);
        station_intersection.push_back(station);
    }

    this->stations = station_intersection;
}

/*
 * Parse JSON string into a list of stations, lines, directions, and durations.
 */
Collection deserialize_json_response(const std::string& json_str) {
    JsonDocument doc = JsonDocument();

    DeserializationError error =
        deserializeJson(doc, json_str, DeserializationOption::NestingLimit(20));
    if (error) {
        // TODO: throw an exception!
        std::cout << error << "\n";
        return {};
    }

    JsonArray monitors = doc["data"]["monitors"];

    Collection collection;

    for (JsonVariant monitor : monitors) {
        std::string station_name =
            monitor["locationStop"]["properties"]["title"];

        // TODO: There has to be a better way than this lmao.
        station_t current_station;

        for (const auto& s : collection.get_stations()) {
            if (s->get_name() == station_name) {
                current_station = s;
                goto found_station;
            }
        }
        current_station = std::make_shared<Station>(station_name);
        collection.add_station(current_station);
    found_station:

        std::string line_name = monitor["lines"][0]["name"];
        std::string line_direction = monitor["lines"][0]["towards"];

        std::vector<uint32_t> departures_for_direction;

        JsonArray departures = monitor["lines"][0]["departures"]["departure"];

        for (auto dep : departures) {
            uint32_t dep_time = dep["departureTime"]["countdown"];
            if (dep_time <= DEPARTURE_TIME_LIMIT) {
                departures_for_direction.push_back(dep_time);
            }
        }

        line_t line = current_station->get_line_by_name(line_name);

        if (line == nullptr) {
            line = std::make_shared<Line>(line_name);
            current_station->add_line(line);
        }

        direction_t direction = line->get_direction_by_name(line_direction);

        if (direction == nullptr) {
            direction = std::make_shared<Direction>(line_direction);
            line->add_direction(direction);
        }

        for (const auto& departure : departures_for_direction) {
            direction->add_departure(Departure(departure));
        }
    }

    return collection;
}

std::ostream& operator<<(std::ostream& os, const Station& station) {
    os << "Station: " << station.name << "\n";
    for (const auto& line : station.lines) {
        os << "  Line: " << line->get_name() << "\n";
        for (const auto& direction : line->get_directions()) {
            os << "  - " << direction->get_name() << "\t";
            for (const auto& dep : direction->get_departures()) {
                os << " " << dep->getDuration();
            }
            os << "\n";
        }
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const Collection& collection) {
    for (const auto& s : collection.stations) {
        os << *s << "\n";
    }
    return os;
}

}  // namespace WL
