#ifndef WL_H
#define WL_H

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

#include "ArduinoJson.h"

constexpr uint32_t DEPARTURE_TIME_LIMIT = 40;

// TODO: instead of std::vector, use std::map or std::multimap !!!
// Reason:
//   It has a find() function already built-in
//   and sorting is included.

namespace WL {
using duration_time_t = uint32_t;

class Departure {
   private:
    duration_time_t duration;

   public:
    Departure(duration_time_t);

    duration_time_t getDuration() const;
};

using departure_t = std::shared_ptr<Departure>;

class Direction {
   private:
    std::string name;
    std::vector<departure_t> departures;

   public:
    Direction(std::string);

    std::string get_name() const;
    std::vector<departure_t> get_departures() const;

    void add_departure(const Departure&);
};

using direction_t = std::shared_ptr<Direction>;

class Line {
   private:
    std::string name;
    std::vector<direction_t> directions;

   public:
    Line(std::string);

    std::string get_name() const;
    std::vector<direction_t> get_directions() const;
    direction_t get_direction_by_name(const std::string) const;

    void add_direction(std::shared_ptr<Direction>);
};

using line_t = std::shared_ptr<Line>;

class Station {
   private:
    std::string name;
    std::vector<line_t> lines;

   public:
    Station(std::string);

    std::string get_name() const;
    // TODO: use pointers for list members. Maybe smart pointers?
    std::vector<line_t> get_lines() const;
    line_t get_line_by_name(std::string) const;

    void add_line(std::shared_ptr<Line>);
};

std::vector<std::shared_ptr<Station>> deserialize_json_response(
    const std::string&);

}  // namespace WL

std::ostream& operator<<(std::ostream&, const WL::Station&);

#endif
