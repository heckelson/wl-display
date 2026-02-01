#include <SPI.h>
#include <cstdint>

#include <XPT2046_Touchscreen.h>

#include <TFT_eSPI.h>
#include <lvgl.h>

// Touchscreen pins
#define XPT2046_IRQ (36)  // T_IRQ
#define XPT2046_MOSI (32) // T_DIN
#define XPT2046_MISO (39) // T_OUT
#define XPT2046_CLK (25)  // T_CLK
#define XPT2046_CS (33)   // T_CS

#define SCREEN_WIDTH (240)
#define SCREEN_HEIGHT (320)
#define FONT_SIZE (2)

#define ROTATION_USB_ON_THE_LEFT (3)
#define ROTATION_USB_ON_THE_RIGHT (1)

#define DRAW_BUF_SIZE (SCREEN_WIDTH * SCREEN_HEIGHT / 10 * (LV_COLOR_DEPTH / 8))

SPIClass touchscreenSPI = SPIClass(VSPI);
XPT2046_Touchscreen touchscreen(XPT2046_CS, XPT2046_IRQ);

// Touchscreen coordinates: (x, y) and pressure (z)
int x, y, z;

uint32_t draw_buffer[DRAW_BUF_SIZE / 4];

static uint32_t tick_cb_func();

void setup() {
  Serial.begin(115200);

  touchscreenSPI.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);
  touchscreen.begin(touchscreenSPI);
  touchscreen.setRotation(2);

  lv_init();
  lv_tick_set_cb(tick_cb_func);
  lv_display_t *disp;

  disp = lv_tft_espi_create(SCREEN_WIDTH, SCREEN_HEIGHT, draw_buffer,
                            sizeof(draw_buffer));
  lv_display_set_rotation(disp, LV_DISPLAY_ROTATION_270);

  lv_obj_t *label = lv_label_create(lv_screen_active());
  lv_label_set_text(label, "Hello PlatformIO, I'm LVGL!");
  lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
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
