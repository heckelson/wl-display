#include "wl.h"

#include <string>
#include <vector>

namespace WL {

Departure::Departure(duration_time_t dur) : duration{dur} {}

duration_time_t Departure::getDuration() const { return this->duration; }

Direction::Direction(std::string n) : name{n} {}

std::string Direction::get_name() const { return this->name; }

std::vector<Departure> Direction::get_departures() const {
    return this->departures;
}

void Direction::add_departure(Departure dep) {
    this->departures.push_back(dep);
}

Line::Line(std::string n) : name{n} {}

std::string Line::get_name() const { return this->name; }

std::vector<Direction> Line::get_directions() const { return this->directions; }

}  // namespace WL
