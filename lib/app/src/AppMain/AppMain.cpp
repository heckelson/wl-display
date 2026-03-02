#include "AppMain.h"

#include <memory>

#include "HardwareSerial.h"

AppMain::AppMain(std::shared_ptr<DivaConverter> diva_converter) {
    Serial.begin(115200);

    Serial.println("Initializing AppMain!");

    this->diva_converter = diva_converter;
}

std::shared_ptr<WifiSettings> AppMain::get_wifi_settings() const {
    return this->wifi_settings;
}
