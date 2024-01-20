#include <TFT_eSPI.h>
#include "demos/lv_demos.h"
#include <Arduino.h>
#include <lvgl.h>

#define HORIZONTAL_RESOLUTION 480
#define VERTICAL_RESOLUTION 320
#define DRAW_BUFFER_SIZE HORIZONTAL_RESOLUTION * 32

// Function Declarations
void initTFT();
void configureLVGL();
void lvglFlushCB(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);

// Variables
TFT_eSPI tft = TFT_eSPI();
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[DRAW_BUFFER_SIZE];

void setup()
{
  // Configure and start the TFT
  initTFT();

  // Configure and start LVGL
  configureLVGL();

  lv_demo_benchmark();
}

void loop()
{
  lv_task_handler();
}

void initTFT()
{
  tft.begin();
  tft.setRotation(3);

  // Turn on Backlight
  pinMode(PB9, OUTPUT);
  digitalWrite(PB9, LOW);
}

void configureLVGL()
{
  // Start LVGL
  lv_init();

  // init the drawing buffer
  lv_disp_draw_buf_init(&draw_buf, buf, NULL, DRAW_BUFFER_SIZE);

  // configure the screen driver
  static lv_disp_drv_t dispDrv;
  lv_disp_drv_init(&dispDrv);
  dispDrv.hor_res = HORIZONTAL_RESOLUTION;
  dispDrv.ver_res = VERTICAL_RESOLUTION;
  dispDrv.flush_cb = lvglFlushCB;
  dispDrv.draw_buf = &draw_buf;
  lv_disp_drv_register(&dispDrv);
}

// Flush Callback - This writes to the TFT
void lvglFlushCB(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.pushColors((uint16_t *)&color_p->full, w * h, true);
  tft.endWrite();

  // signal LVGL that the screen is written
  lv_disp_flush_ready(disp);
}