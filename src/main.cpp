#include <memory>
#include <stdexcept>

#include "AppMain/AppMain.h"
#include "DisplayMgr/DisplayMgr.h"
#include "Exceptions.h"
#include "esp32-hal.h"
#include "wl/wl.h"

// we cannot call this app_main because that name is reserved.
AppMain* my_app_main;

void app_main_thread(void* _) {
    for (;;) {
        my_app_main->loop(NULL);
    }
}

void setup() {
    delay(1000);

    try {
        my_app_main = new AppMain{};

        if (my_app_main == nullptr) {
            throw std::runtime_error("MyAppMain creation failed!");
        }

        WL::Collection collection = WL::Collection();
        auto palffygasse = std::make_shared<WL::Station>("Palffygasse");
        auto line_43 = std::make_shared<WL::Line>("43");
        auto dir1 = std::make_shared<WL::Direction>("Neuwaldegg");
        auto dir2 = std::make_shared<WL::Direction>("Schottentor");

        line_43->add_direction(dir1);
        line_43->add_direction(dir2);
        palffygasse->add_line(line_43);
        collection.add_station(palffygasse);

        Serial.println(collection.serialize().c_str());
        delay(10000);

        xTaskCreatePinnedToCore(app_main_thread,  // fn
                                "AppMain_loop",   // name
                                1000,             // Stack size
                                NULL,             // input params
                                0,                // priority
                                NULL,             // task handle
                                0                 // which core
        );
    } catch (BaseException err) {
        Serial.begin(115200);
        Serial.print("Uncaught error: ");
        Serial.println(err.what());
    }
}

void loop() { DisplayMgr::loop(); }
