#ifndef DISPLAYMGR_H
#define DISPLAYMGR_H

#include <Arduino.h>
#include <lvgl.h>

#include <cstdint>

namespace DisplayMgr {

namespace {
// pinouts
const uint8_t XPT2046_IRQ = 36;   // T_IRQ
const uint8_t XPT2046_MOSI = 32;  // T_DIN
const uint8_t XPT2046_MISO = 39;  // T_OUT
const uint8_t XPT2046_CLK = 25;   // T_CLK
const uint8_t XPT2046_CS = 33;    // T_CS

// screen dimensions
const uint16_t SCREEN_WIDTH = 240;
const uint16_t SCREEN_HEIGHT = 320;

constexpr uint16_t DRAW_BUF_SIZE =
    (SCREEN_WIDTH * SCREEN_HEIGHT / 10 * (LV_COLOR_DEPTH / 8));
}  // namespace

static uint32_t tick_cb_func() { return millis(); }

void update_system_message(const std::string&);
void init();
void loop();
}  // namespace DisplayMgr

#endif
