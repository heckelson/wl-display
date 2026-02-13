#ifndef WL_H
#define WL_H

#include <cstdint>
#include <string>
#include <vector>

namespace WL {
using duration_time_t = uint32_t;

class Departure {
   private:
    duration_time_t duration;

   public:
    Departure(duration_time_t);

    duration_time_t getDuration() const;
};

class Direction {
   private:
    std::string name;
    std::vector<Departure> departures;

   public:
    Direction(std::string);

    std::string get_name() const;
    // TODO: use pointers for list members. Maybe smart pointers?
    std::vector<Departure> get_departures() const;

    void add_departure(Departure);
};

class Line {
   private:
    std::string name;
    std::vector<Direction> directions;

   public:
    Line(std::string);

    std::string get_name() const;
    // TODO: use pointers for list members. Maybe smart pointers?
    std::vector<Direction> get_directions() const;
    Direction get_direction_by_name(std::string) const;

    void add_direction(Direction);
};

class Station {
   private:
    std::string name;
    std::vector<Line> lines;

   public:
    Station(std::string);

    std::string get_name() const;
    // TODO: use pointers for list members. Maybe smart pointers?
    std::vector<Line> get_lines() const;
    Line get_line_by_name(std::string) const;

    void add_line(Line);
};

}  // namespace WL

#endif
