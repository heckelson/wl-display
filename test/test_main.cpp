#include <cstdint>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

// #include "../include/ArduinoJson.h"
#include "../src/wl/wl.h"

// #define CPPHTTPLIB_OPENSSL_SUPPORT
// #include "httplib.h"

#include "liblittletest.h"

LT_BEGIN_SUITE(test_main)
void set_up() {}
void tear_down() {}
LT_END_SUITE(test_main)

LT_BEGIN_AUTO_TEST(test_main, test_can_test)
auto station = std::make_unique<WL::Station>("Palffygasse");
auto line = WL::Line{"43"};

station->add_line(line);

line.add_direction(WL::Direction{"Schottentor"});
line.add_direction(WL::Direction{"Neuwaldegg"});

auto schottentor = line.get_direction_by_name("Schottentor");
auto neuwaldegg = line.get_direction_by_name("Neuwaldegg");

schottentor->add_departure(WL::Departure{2});
schottentor->add_departure(WL::Departure{5});
schottentor->add_departure(WL::Departure{8});
schottentor->add_departure(WL::Departure{12});

neuwaldegg->add_departure(WL::Departure{4});
neuwaldegg->add_departure(WL::Departure{6});
neuwaldegg->add_departure(WL::Departure{33});

LT_ASSERT_NEQ(schottentor, nullptr);
LT_ASSERT_NEQ(neuwaldegg, nullptr);

std::cout << "Station: " << station->get_name() << "\n";
for (auto s : station->get_lines()) {
    std::cout << "  Line: " << s->get_name() << "\n";
    for (auto direction : line.get_directions()) {
        std::cout << "  - " << direction->get_name() << "\t";
        for (auto dep : direction->get_departures()) {
            std::cout << " " << dep->getDuration();
        }
        std::cout << "\n";
    }
}

auto retrieve_line = station->get_line_by_name("43");
LT_ASSERT_EQ(retrieve_line->get_name(), line.get_name());

LT_END_TEST(test_can_test)

/* Test main function definition */
LT_BEGIN_AUTO_TEST_ENV()
AUTORUN_TESTS()
LT_END_AUTO_TEST_ENV()

// void fetch_realtime_information_for_diva(uint32_t diva_code) {
//     // TODO: better error handling for DIVA code.
//     if (!(diva_code >= 60'000'000 && diva_code <= 70'000'000)) {
//         throw std::runtime_error("Got bad DIVA code: " +
//                                  std::to_string(diva_code));
//     }

//     auto cli = new httplib::Client("https://www.wienerlinien.at");
//     cli->set_follow_location(true);

//     std::string path =
//         "/ogd_realtime/monitor?diva=" + std::to_string(diva_code);

//     httplib::Result res = cli->Get(path);

//     if (res.error() != httplib::Error::Success) {
//         res.error();
//     }
//     if (!(200 <= res->status && res->status <= 300)) {
//         throw std::runtime_error("Got bad response! Code: " +
//                                  std::to_string(res->status));
//     }

//     JsonDocument doc;
//     DeserializationError error = deserializeJson(
//         doc, res->body, DeserializationOption::NestingLimit(20));
//     if (error) {
//         std::cout << error << "\n";
//     }

//     std::string message = doc["message"]["status"];
//     std::cout << message << std::endl;

//     delete cli;
// }
