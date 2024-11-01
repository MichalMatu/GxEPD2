#include <GxEPD2_BW.h>
#include <Fonts/FreeMonoBold9pt7b.h> // Include a font

// Define the specific display model and size
GxEPD2_BW<GxEPD2_290_GDEY029T94, GxEPD2_290_GDEY029T94::HEIGHT> display(GxEPD2_290_GDEY029T94(/*CS=*/27, /*DC=*/14, /*RST=*/12, /*BUSY=*/13));

void setup()
{
  display.init(); // Initialize display

  display.setRotation(3);              // Optional: Set rotation if you want landscape mode
  display.setFont(&FreeMonoBold9pt7b); // Set font
  display.setTextColor(GxEPD_BLACK);   // Set text color
  display.setFullWindow();             // Prepare for full-screen update

  // Start drawing "Hello, World!" to the screen
  display.firstPage();
  do
  {
    display.fillScreen(GxEPD_WHITE); // Clear the screen
    display.setCursor(10, 50);       // Set position (10, 50) from the top left
    display.print("Hello, World!");  // Display the text
  } while (display.nextPage()); // Display buffer to the screen
}

void loop()
{
  // No need to repeatedly update the screen in this example
}
