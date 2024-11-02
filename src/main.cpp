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
  const uint32_t full_w = 296;
  const uint32_t full_h = 128;
  uint32_t buffer_size = (full_w * full_h) / 2;

  uint8_t *packed_buffer = new uint8_t[buffer_size];
  for (uint32_t y = 0; y < full_h; y++)
  {
    for (uint32_t x = 0; x < full_w; x += 2)
    {
      uint32_t color_index = y * full_w + x;

      lv_color_t pixel1 = color_p[color_index];
      lv_color_t pixel2 = color_p[color_index + 1];

      uint8_t grayscale1 = get_grayscale_4bit(pixel1);
      uint8_t grayscale2 = get_grayscale_4bit(pixel2);

      uint8_t packed_pixel = (grayscale1 << 4) | grayscale2;

      uint32_t packed_index = (y * (full_w / 2)) + (x / 2);
      packed_buffer[packed_index] = packed_pixel;
    }
  }

  display.fillScreen(GxEPD_WHITE); // Clear the display with white color

  display.drawImage(packed_buffer, 0, 0, full_w, full_h);
  display.display();
  delete[] packed_buffer;

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
