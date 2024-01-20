#include "demos/lv_demos.h"
#include <Arduino.h>
#include <lvgl.h>

#define HORIZONTAL_RESOLUTION 480
#define VERTICAL_RESOLUTION 320
#define DRAW_BUFFER_SIZE HORIZONTAL_RESOLUTION * 32

// Function Declarations
void initTFT();
void configureLVGL();

// Variables
static lv_display_t *mainDisplay;
static uint8_t drawBuffer[DRAW_BUFFER_SIZE];
unsigned long lastTickMillis = 0;

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
  // LVGL Tick Interface
  unsigned int tickPeriod = millis()-lastTickMillis;
  lv_tick_inc(tickPeriod);
  lastTickMillis = millis();

  // LVGL Task Handler
  lv_task_handler();
}

void initTFT()
{
  // Turn on Backlight
  pinMode(PB9, OUTPUT);
  digitalWrite(PB9, LOW);
}

void configureLVGL()
{
  // Start LVGL
  lv_init();

  // Configure the display driver
  mainDisplay = lv_tft_espi_create(HORIZONTAL_RESOLUTION, VERTICAL_RESOLUTION, drawBuffer, DRAW_BUFFER_SIZE);
}