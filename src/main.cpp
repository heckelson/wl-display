#include <SPI.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <XPT2046_Touchscreen.h>
#include <lvgl.h>

#include <cstdint>

#include "wifi_secrets.h"

int status = WL_IDLE_STATUS;
const char host[] = "example.org";
// IPAddress server(192, 168, 0, 46);
const int httpPort = 80;

// Touchscreen pins
#define XPT2046_IRQ (36)   // T_IRQ
#define XPT2046_MOSI (32)  // T_DIN
#define XPT2046_MISO (39)  // T_OUT
#define XPT2046_CLK (25)   // T_CLK
#define XPT2046_CS (33)    // T_CS

#define SCREEN_WIDTH (240)
#define SCREEN_HEIGHT (320)

#define DRAW_BUF_SIZE (SCREEN_WIDTH * SCREEN_HEIGHT / 10 * (LV_COLOR_DEPTH / 8))

SPIClass touchscreenSPI = SPIClass(VSPI);
XPT2046_Touchscreen touchscreen(XPT2046_CS, XPT2046_IRQ);

// Touchscreen coordinates: (x, y) and pressure (z)
int x, y, z;

lv_obj_t* label = nullptr;

uint32_t** draw_buffer =
    (uint32_t**)malloc(sizeof(uint32_t) * DRAW_BUF_SIZE / 4);

// callback functions
static uint32_t tick_cb_func();
void touchscreen_read_cb_func(lv_indev_t*, lv_indev_data_t*);

void readResponse(WiFiClient* client) {
        Serial.println("Reading response...");

        while (!client->available());

        while (client->available()) {
                char c = client->read();
                Serial.print(c);
        }

        Serial.printf("\nClosing connection\n\n");
}

void my_btn_event_cb(lv_event_t* e) { Serial.printf("Clicked\n"); }

void setup() {
        Serial.begin(115200);

        WiFi.begin(WIFI_SSID, WIFI_PASSWD);

        while (WiFi.status() != WL_CONNECTED) {
                delay(500);
                Serial.print(".");
        }

        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());

retry:
        WiFiClient client;
        if (!client.connect(host, httpPort)) {
                delay(500);
                Serial.println("Retrying");
                goto retry;
        }
        String footer = String(" HTTP/1.1\r\n") + "Host: " + String(host) +
                        "\r\n" + "Connection: close\r\n\r\n";

        client.print("GET /" + footer);
        readResponse(&client);

        touchscreenSPI.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI,
                             XPT2046_CS);
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

        Serial.println("Setup complete!");
        Serial.flush();
}

void loop() {
        lv_task_handler();
        lv_tick_inc(5);
        delay(5);
}

static uint32_t tick_cb_func() {
        // easy as pie.
        return millis();
}

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
