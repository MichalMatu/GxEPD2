#include <GxEPD2_BW.h>

// Define the specific display model and size
GxEPD2_BW<GxEPD2_290_GDEY029T94, GxEPD2_290_GDEY029T94::HEIGHT> display(GxEPD2_290_GDEY029T94(/*CS=*/27, /*DC=*/14, /*RST=*/12, /*BUSY=*/13));

void setup()
{
  display.init(); // Initialize display
                  // Additional setup code
}

void loop()
{
  // Display code
}
