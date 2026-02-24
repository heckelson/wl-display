#include "TestNetworkMgr.h"

#include <cstdint>
#include <memory>
#include <stdexcept>
#include <string>

#include "AppMain/UserSettings.h"
#include "wl/wl.h"

TestNetworkMgr::TestNetworkMgr() {
    this->client =
        std::make_unique<httplib::Client>("https://www.wienerlinien.at");
}

std::string TestNetworkMgr::fetch_station_info_by_diva(uint32_t diva_code) {
    if (diva_code < 60200001 || diva_code > 60259921) {
        throw std::runtime_error(
            "Invalid diva code - out of range (60200001-60259921): " +
            std::to_string(diva_code));
    }

    httplib::Result result = this->client->Get("/ogd_realtime/monitor?diva=" +
                                               std::to_string(diva_code));

    if (result->status != 200) {
        throw std::runtime_error("Cannot fetch diva code: " +
                                 std::to_string(diva_code));
    }

    return result->body;
}

bool TestNetworkMgr::check_connection() const {
    throw std::runtime_error("// TODO: Not implemented");
}

WL::Collection TestNetworkMgr::get(WlSettings& settings) {
    throw std::runtime_error("// TODO: Not implemented");
}
