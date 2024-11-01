#include <GxEPD2_BW.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Adafruit_FT6206.h> // Include FT6206-compatible touch library

// Display initialization
GxEPD2_BW<GxEPD2_290_GDEY029T94, GxEPD2_290_GDEY029T94::HEIGHT> display(GxEPD2_290_GDEY029T94(/*CS=*/27, /*DC=*/14, /*RST=*/12, /*BUSY=*/13));

// Touch initialization
Adafruit_FT6206 ts;

void setup()
{
  // Initialize serial for debugging
  Serial.begin(115200);
  delay(100); // Give some time for serial to initialize

  // Configure touch controller reset pin
  pinMode(39, OUTPUT);
  digitalWrite(39, HIGH); // Set high to avoid resetting, adjust if needed

  // Initialize I2C bus with specific SDA and SCL pins
  Wire.begin(33, 32); // SDA on 33, SCL on 32

  // Initialize display
  display.init(115200); // You can try different baud rates here if needed
  display.setRotation(3);
  display.setFont(&FreeMonoBold9pt7b);
  display.setTextColor(GxEPD_BLACK);
  display.setFullWindow();

  // Draw initial message on display
  display.firstPage();
  do
  {
    display.fillScreen(GxEPD_WHITE);
    display.setCursor(10, 50);
    display.print("Hello, World!");
  } while (display.nextPage());

  // Add a delay before initializing the touch controller
  delay(100);

  // Initialize touch controller
  if (ts.begin())
  {
    Serial.println("Touch controller initialized successfully.");
  }
  else
  {
    Serial.println("Failed to initialize touch controller.");
  }
}

void loop()
{
  // Check for touch points
  if (ts.touched())
  {
    TS_Point point = ts.getPoint();
    Serial.print("Touch detected at: ");
    Serial.print("X = ");
    Serial.print(point.x);
    Serial.print(", Y = ");
    Serial.println(point.y);

    // Optional: Display touch coordinates on the screen
    display.firstPage();
    do
    {
      display.fillScreen(GxEPD_WHITE);
      display.setCursor(10, 50);
      display.print("Hello, World!");

      // Display touch coordinates
      display.setCursor(10, 100);
      display.print("Touch at: ");
      display.print(point.x);
      display.print(", ");
      display.print(point.y);
    } while (display.nextPage());
  }

  delay(100); // Delay to avoid rapid polling
}
