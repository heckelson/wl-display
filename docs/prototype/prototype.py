#!/usr/bin/env -S uv run --script
#
# /// script
# requires-python = ">3.12"
# dependencies = [
#   "requests",
# ]
# ///
#
#
# This script can be run with uv.
# https://docs.astral.sh/uv/#installation
#
from dataclasses import dataclass, field

import requests


@dataclass
class Departure:
    duration: int


@dataclass
class Direction:
    name: str
    departures: list[Departure] = field(default_factory=list)


@dataclass
class Line:
    name: str
    directions: list[Direction] = field(default_factory=list)

    def get_direction_by_name(self, name):
        for dir in self.directions:
            if dir.name == name:
                return dir
            else:
                return None


@dataclass
class Station:
    name: str
    lines: list[Line] = field(default_factory=list)

    def get_line_by_name(self, name) -> Line | None:
        for line in self.lines:
            if line.name == name:
                return line
        return None

    def parse_lines(self, monitors: list):
        for monitor in monitors:
            line_name = monitor["lines"][0]["name"]
            line_direction = monitor["lines"][0]["towards"]

            departures = list(
                map(
                    lambda x: x["departureTime"]["countdown"],
                    monitor["lines"][0]["departures"]["departure"],
                )
            )

            line = None
            if (ll := self.get_line_by_name(line_name)) is None:
                line = Line(line_name)
                self.lines.append(line)
            else:
                line = ll

            direction = None
            if (d := line.get_direction_by_name(line_direction)) is None:
                direction = Direction(line_direction)
                line.directions.append(direction)
            else:
                direction = d

            direction.departures.extend(departures)


def main():
    diva = 60200874
    resp = requests.get(f"https://www.wienerlinien.at/ogd_realtime/monitor?diva={diva}")

    resp = resp.json()

    data = resp["data"]

    station = Station(name=data["monitors"][0]["locationStop"]["properties"]["title"])
    station.parse_lines(data["monitors"])
    print(station)


if __name__ == "__main__":
    main()
