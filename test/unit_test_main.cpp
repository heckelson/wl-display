#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

#include "AppMain/AppMain.h"
#include "NetworkMgr/DivaConverter.h"
#include "TestNetworkMgr/TestNetworkMgr.h"
#include "wl/wl.h"

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "TestNetworkMgr/httplib.h"
#include "liblittletest.h"

std::string example_response;

#define LT_SKIP()                     \
    std::cout << "[Skipping test]\n"; \
    return;

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
    LT_SKIP()

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
    LT_SKIP()

    auto collection = WL::deserialize_json_response(example_response);

    for (const auto& s : collection.get_stations()) {
        std::cout << *s << "\n";
    }
}
LT_END_TEST(test_can_parse_example_resp_into_datastructures)

LT_BEGIN_AUTO_TEST(wl_tests, test_can_parse_response_from_wl) {
    LT_SKIP()

    httplib::Client cli("https://www.wienerlinien.at");

    uint32_t DIVA = 60200874;  // Michelbeuern
    auto res = cli.Get("/ogd_realtime/monitor?diva=" + std::to_string(DIVA));

    LT_ASSERT(res);

    if (res) {
        auto collection = WL::deserialize_json_response(res->body);
        for (const auto& s : collection.get_stations()) {
            std::cout << *s << "\n";
        }
    }
}
LT_END_AUTO_TEST(test_can_parse_response_from_wl)

LT_BEGIN_AUTO_TEST(wl_tests, test_can_intersect_collections) {
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
LT_END_AUTO_TEST(test_can_intersect_collections)

LT_BEGIN_AUTO_TEST(wl_tests, test_diva_converter) {
    DivaConverter converter = DivaConverter("../data/name-diva-mapping.csv");

    for (size_t i = 0; i < 10; ++i) {
        std::string name = "Palffygasse";
        auto before = std::chrono::high_resolution_clock::now();
        std::string diva = converter.get_diva_by_name(name);
        auto after = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(
            after - before);
        std::cout << "invocation took " << duration.count() / 1000.0 << "us\n";

        LT_ASSERT_EQ(diva, "60200988")
    }

}
LT_END_AUTO_TEST(test_diva_converter)

LT_BEGIN_AUTO_TEST(wl_tests, test_setup_appmain_works) {
    TestNetworkMgr network_mgr = TestNetworkMgr{};

    uint32_t palffy_diva = 60200988;
    std::string resp = network_mgr.fetch_station_info_by_diva(palffy_diva);
    auto stations = WL::deserialize_json_response(resp);

    AppMain main(std::make_unique<TestNetworkMgr>());

    main.add_entry_to_wl_settings("Palffygasse", "43", "Neuwaldegg");
    main.add_entry_to_wl_settings("Palffygasse", "43", "Neuwaldegg");
    main.add_entry_to_wl_settings("Palffygasse", "43", "Schottentor");

    UserSettings settings = main.get_user_settings();

    for (auto station : settings.wl_settings.station_config) {
        std::cout << station.first << ":\n";
        for (auto pair : station.second) {
            std::cout << " - " << pair.first << " -> " << pair.second << "\n";
        }
    }
}
LT_END_AUTO_TEST(test_setup_appmain_works)

/* Test main function definition */
LT_BEGIN_AUTO_TEST_ENV();
AUTORUN_TESTS();
LT_END_AUTO_TEST_ENV();
