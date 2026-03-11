#include "DisplayMgr.h"

#include <SPI.h>
#include <XPT2046_Touchscreen.h>
#include <lvgl.h>

#include "icons/wifi.h"

namespace DisplayMgr {
SPIClass touchscreenSPI = SPIClass(VSPI);
XPT2046_Touchscreen touchscreen(XPT2046_CS, XPT2046_IRQ);
int x, y, z;

uint32_t** draw_buffer =
    (uint32_t**)malloc(sizeof(uint32_t) * DRAW_BUF_SIZE / 4);

lv_obj_t* label = nullptr;

void touchscreen_read_cb_func(lv_indev_t* indev, lv_indev_data_t* data) {
    if (touchscreen.tirqTouched() && touchscreen.touched()) {
        TS_Point p = touchscreen.getPoint();
        // Calibrate Touchscreen points with map function to the correct
        // width and height
        x = map(p.x, 200, 3700, 1, SCREEN_WIDTH);
        y = map(p.y, 240, 3800, 1, SCREEN_HEIGHT);
        z = p.z;

        data->state = LV_INDEV_STATE_PRESSED;

        data->point.x = x;
        data->point.y = y;

        char* label_text;
        asprintf(&label_text, "Coords: x=%d, y=%d", x, y);
        lv_label_set_text(label, label_text);
        free(label_text);

        // Print Touchscreen info about X, Y and Pressure (Z) on the
        // Serial Monitor
        Serial.print("X = ");
        Serial.print(x);
        Serial.print(" | Y = ");
        Serial.print(y);
        Serial.print(" | Pressure = ");
        Serial.print(z);
        Serial.println();
    } else {
        data->state = LV_INDEV_STATE_RELEASED;
    }
}

void my_btn_event_cb(lv_event_t* e) { Serial.printf("Clicked\n"); }

void init() {
    Serial.println("Initializing DisplayMgr.");
    touchscreenSPI.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);
    touchscreen.begin(touchscreenSPI);
    touchscreen.setRotation(2);

    lv_init();

    lv_tick_set_cb(tick_cb_func);
    lv_display_t* disp;

    disp = lv_tft_espi_create(SCREEN_WIDTH, SCREEN_HEIGHT, draw_buffer,
                              sizeof(uint32_t) * DRAW_BUF_SIZE / 4);
    lv_display_set_rotation(disp, LV_DISPLAY_ROTATION_270);
    lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(0x003a57),
                              LV_PART_MAIN);

    lv_indev_t* indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev, touchscreen_read_cb_func);

    label = lv_label_create(lv_screen_active());

    lv_obj_t* my_button1 = lv_button_create(lv_screen_active());
    lv_obj_t* my_label1 = lv_label_create(my_button1);

    lv_label_set_text_fmt(my_label1, "Click me!");

    lv_obj_add_event_cb(my_button1, my_btn_event_cb, LV_EVENT_CLICKED, NULL);

    lv_label_set_text(label, "Hello PlatformIO, I'm LVGL!");
    lv_obj_set_style_text_color(lv_screen_active(), lv_color_hex(0xffffff),
                                LV_PART_MAIN);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
    Serial.println("Finished initializing DisplayMgr.");
}

void loop() { lv_timer_handler(); }

}  // namespace DisplayMgr
