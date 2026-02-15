#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

#include "../src/wl/wl.h"

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"
#include "liblittletest.h"

std::string example_response;

LT_BEGIN_SUITE(wl_tests)
void set_up() {
    std::ifstream test_file("../docs/example-response.json");
    if (!test_file.is_open()) {
        std::cout << "Unable to open test file! Make sure you run the test "
                     "binary from inside the  the $(REPO_ROOT)/test/ directory."
                  << '\n';
        exit(EXIT_FAILURE);
    }

    std::stringstream sstr;
    test_file >> sstr.rdbuf();

    if (test_file.is_open()) {
        test_file.close();
    }
    example_response = sstr.str();
}
void tear_down() {}
LT_END_SUITE(wl_tests)

LT_BEGIN_AUTO_TEST(wl_tests, test_can_create_wl_objects) {
    auto station = std::make_unique<WL::Station>("Palffygasse");
    auto line = std::make_shared<WL::Line>("43");

    line->add_direction(std::make_shared<WL::Direction>("Schottentor"));
    line->add_direction(std::make_shared<WL::Direction>("Neuwaldegg"));

    station->add_line(line);

    auto schottentor = line->get_direction_by_name("Schottentor");
    auto neuwaldegg = line->get_direction_by_name("Neuwaldegg");

    schottentor->add_departure(WL::Departure{2});
    schottentor->add_departure(WL::Departure{5});
    schottentor->add_departure(WL::Departure{8});
    schottentor->add_departure(WL::Departure{12});

    neuwaldegg->add_departure(WL::Departure{4});
    neuwaldegg->add_departure(WL::Departure{6});
    neuwaldegg->add_departure(WL::Departure{33});

    LT_ASSERT_NEQ(schottentor, nullptr);
    LT_ASSERT_NEQ(neuwaldegg, nullptr);

    std::cout << *station << "\n";

    auto retrieve_line = station->get_line_by_name("43");
    LT_ASSERT_EQ(retrieve_line->get_name(), line->get_name());
}

LT_END_TEST(test_can_create_wl_objects)

LT_BEGIN_AUTO_TEST(wl_tests, test_can_parse_example_resp_into_datastructures) {
    auto stations = WL::deserialize_json_response(example_response);

    for (const auto& s : stations) {
        std::cout << *s << "\n";
    }
}
LT_END_TEST(test_can_parse_example_resp_into_datastructures)

LT_BEGIN_AUTO_TEST(wl_tests, test_can_parse_response_from_wl) {
    httplib::Client cli("https://www.wienerlinien.at");

    uint32_t DIVA = 60200874;  // Michelbeuern
    auto res = cli.Get("/ogd_realtime/monitor?diva=" + std::to_string(DIVA));

    LT_ASSERT(res);

    if (res) {
        auto stations = WL::deserialize_json_response(res->body);
        for (const auto& s : stations) {
            std::cout << *s << "\n";
        }
    }
}
LT_END_AUTO_TEST(test_can_parse_response_from_wl)

/* Test main function definition */
LT_BEGIN_AUTO_TEST_ENV();
AUTORUN_TESTS();
LT_END_AUTO_TEST_ENV();
