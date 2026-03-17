#include <stdexcept>

#include "AppMain/AppMain.h"
#include "DisplayMgr/DisplayMgr.h"
#include "Exceptions.h"
#include "esp32-hal.h"

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

        DisplayMgr::update_system_message(err.what());
    }
}

void loop() { DisplayMgr::loop(); }
