#include <unity.h>

#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <chrono>

#include "wl/wl.h"
#include "DivaConverter.h"

#ifndef TESTING_ENV
#include <Arduino.h>
#endif

std::string example_response;

void setUp(void) {
    std::ifstream test_file("docs/example-response.json");
    if (!test_file.is_open()) {
        std::cout << "Unable to open test file! Make sure you run the test "
                     "binary from inside the  the $(REPO_ROOT)/ directory."
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

void tearDown(void) {}

void test__can_create_wl_objects() {
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

    std::cout << *station << "\n";

    auto retrieve_line = station->get_line_by_name("43");
}

void test__can_parse_example_resp_into_datastructure() {
    auto collection = WL::deserialize_json_response(example_response);

    for (const auto& s : collection.get_stations()) {
        std::cout << *s << "\n";
    }
}

void test__can_intersect_collections() {
    auto collection = WL::deserialize_json_response(example_response);

    WL::Collection other = WL::Collection();
    auto station = std::make_shared<WL::Station>("Palffygasse");
    auto line = std::make_shared<WL::Line>("43");
    auto direction = std::make_shared<WL::Direction>("Neuwaldegg");

    other.add_station(station);
    station->add_line(line);
    line->add_direction(direction);
    direction->add_departure(WL::Departure(2));
    direction->add_departure(WL::Departure(4));
    direction->add_departure(WL::Departure(6));

    std::cout << "Before:\n";
    for (const auto& s : collection.get_stations()) {
        std::cout << *s << "\n";
    }

    collection.intersect(other);
    std::cout << "After:\n";
    for (const auto& s : collection.get_stations()) {
        std::cout << *s << "\n";
    }
}

void test__can_convert_between_diva_and_name() {
    DivaConverter converter = DivaConverter("data/name-diva-mapping.csv");

    for (size_t i = 0; i < 10; ++i) {
        std::string name = "Palffygasse";
        auto before = std::chrono::high_resolution_clock::now();
        std::string diva = converter.get_diva_by_name(name);
        auto after = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(
            after - before);
        std::cout << "invocation took " << duration.count() / 1000.0 << "us\n";
    }
}

int run_unit_tests(void) {
    UNITY_BEGIN();

    RUN_TEST(test__can_create_wl_objects);
    RUN_TEST(test__can_parse_example_resp_into_datastructure);
    RUN_TEST(test__can_intersect_collections);
    RUN_TEST(test__can_convert_between_diva_and_name);

    return UNITY_END();
}

// WARNING!!! PLEASE REMOVE UNNECESSARY MAIN IMPLEMENTATIONS //

/**
 * For native dev-platform or for some embedded frameworks
 */
#ifdef TESTING_ENV
int main() { return run_unit_tests(); }
#endif

#ifndef TESTING_ENV

/**
 * For Arduino framework
 */
void setup() {
    // Wait ~2 seconds before the Unity test runner
    // establishes connection with a board Serial interface
    delay(2000);

    run_unit_tests();
}
void loop() {}
#endif
