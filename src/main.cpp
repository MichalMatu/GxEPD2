#include <lvgl.h>
#include <GxEPD2_BW.h>
#include <GxEPD2_GFX.h>

// Display initialization
GxEPD2_BW<GxEPD2_290_GDEY029T94, GxEPD2_290_GDEY029T94::HEIGHT> display(GxEPD2_290_GDEY029T94(/*CS=*/27, /*DC=*/14, /*RST=*/12, /*BUSY=*/13));
#define LV_HOR_RES_MAX 296 // Set this to your display's width
#define LV_VER_RES_MAX 128 // Set this to your display's height

// Define LVGL-related variables and structures
static lv_disp_draw_buf_t draw_buf;
static lv_disp_drv_t disp_drv;
static lv_color_t buf[LV_HOR_RES_MAX * 10]; // Display buffer

// Function to convert lv_color_t to 4-bit grayscale value (0-15)
uint8_t get_grayscale_4bit(lv_color_t pixel)
{
  uint8_t r, g, b;

#if LV_COLOR_DEPTH == 16         // Assuming 16-bit RGB565 format
  r = (pixel.full >> 11) & 0x1F; // Red component (5 bits)
  g = (pixel.full >> 5) & 0x3F;  // Green component (6 bits)
  b = pixel.full & 0x1F;         // Blue component (5 bits)

  r = (r * 255) / 31;
  g = (g * 255) / 63;
  b = (b * 255) / 31;
#elif LV_COLOR_DEPTH == 32 // Assuming 32-bit ARGB8888 format
  r = pixel.ch.red;
  g = pixel.ch.green;
  b = pixel.ch.blue;
#else
  r = g = b = 0;
#endif

  uint8_t grayscale = (r + g + b) / 3;
  return grayscale >> 4;
}

// LVGL flush function to send buffer data to display
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
  // Check if area is out of bounds
  if (area->x2 < 0 || area->y2 < 0 || area->x1 > LV_HOR_RES_MAX || area->y1 > LV_VER_RES_MAX)
  {
    lv_disp_flush_ready(disp);
    return;
  }

  // display.fillScreen(GxEPD_WHITE); // Clear the display with white color

  // Draw each pixel in the LVGL buffer
  for (int y = area->y1; y <= area->y2; y++)
  {
    for (int x = area->x1; x <= area->x2; x++)
    {
      int index = (y - area->y1) * (area->x2 - area->x1 + 1) + (x - area->x1);
      lv_color_t color = color_p[index];
      uint8_t grayscale = get_grayscale_4bit(color);
      uint16_t color_val = grayscale > 7 ? GxEPD_BLACK : GxEPD_WHITE;
      display.drawPixel(x, y, color_val);
    }
  }

  display.display(); // Update the display with the drawn buffer
  lv_disp_flush_ready(disp);
}

void setup()
{
  Serial.begin(115200);

  // Initialize display and LVGL
  display.init();
  lv_init();

  // Initialize display buffer
  lv_disp_draw_buf_init(&draw_buf, buf, NULL, LV_HOR_RES_MAX * 10);

  // Set up the display driver
  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = 296;
  disp_drv.ver_res = 128;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  // Create a label to display
  lv_obj_t *label = lv_label_create(lv_scr_act());
  lv_label_set_text(label, "Hello, LVGL!");
  lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
}

void loop()
{
  // Handle LVGL tasks
  lv_timer_handler();
  delay(5);
}
