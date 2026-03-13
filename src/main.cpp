#include "AppMain/AppMain.h"
#include "Exceptions.h"
#include "esp32-hal.h"

// we cannot call this app_main because that name is reserved.
AppMain* my_app_main;

void setup() {
    delay(1000);

    try {
        my_app_main = new AppMain{};
    } catch (BaseException err) {
        Serial.begin(115200);
        Serial.print("Uncaught error: ");
        Serial.println(err.what());
    }
}

void loop() {
    // maybe, this is easier.
    my_app_main->loop();
}
