// Note that this code only works if the LDR detects the change
// within the LED on and off duration. Adjust as necessary.
//
// Test the pure latency of the measurement system (mostly because of the type of LED and LDR used) by holding 
// the LDR to the LED without any additional latency caused by external (video) transmission.
// This allows you to determine the average error in your measurements.

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ----------------------------------------------------------------------------
// Display  -------------------------------------------------------------------
// ----------------------------------------------------------------------------

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 

const int displayAddress = 0x3C;

// ----------------------------------------------------------------------------
// Pins -----------------------------------------------------------------------
// ----------------------------------------------------------------------------

int ledPin = 2;
int ldrPin = A0;

// ----------------------------------------------------------------------------
// Time -----------------------------------------------------------------------
// ----------------------------------------------------------------------------

int currentMillis;

// ----------------------------------------------------------------------------
// LED-------------------------------------------------------------------------
// ----------------------------------------------------------------------------

bool ledState = false;
int ledLightDurationInMillis = 1000;
int ledDarkDurationInMillis = 1000;
int ledChangeDuration;
int lastLedChangeMillis = 0;

// ----------------------------------------------------------------------------
// LDR ------------------------------------------------------------------------
// ----------------------------------------------------------------------------

int measuredLdrState;
int ldrThreshold = 750;
int lastLdrChangeMillis = 0;
int lastDetectedLdrState = false;

// ----------------------------------------------------------------------------
// Calculated values ----------------------------------------------------------
// ----------------------------------------------------------------------------

int offsetInMillis = 27; // Used to remove the LED and LDR latency when measuring
int latencyInMillis;

// ----------------------------------------------------------------------------
// Setup ----------------------------------------------------------------------
// ----------------------------------------------------------------------------

void setup() {
    Serial.begin(115200);
    Serial.println("Logging Latency in milliseconds");

    pinMode(ledPin, OUTPUT);
    pinMode(ldrPin, INPUT);

    display.begin(SSD1306_SWITCHCAPVCC, displayAddress);
    display.setTextColor(WHITE);
}

// ----------------------------------------------------------------------------
// Loop -----------------------------------------------------------------------
// ----------------------------------------------------------------------------

void loop() {
    currentMillis = millis();

    ledChangeDuration = (ledState == true? ledLightDurationInMillis : ledDarkDurationInMillis);
    
    if ((currentMillis - lastLedChangeMillis) >= ledChangeDuration) {
        digitalWrite(ledPin, (ledState == true ? LOW : HIGH));
        ledState = (ledState == true ? false : true);
        lastLedChangeMillis = currentMillis;
    }

    // Prevent consecutive measurements when near the threshold
    if (currentMillis >= (lastLdrChangeMillis + (ledChangeDuration / 2))) {

        measuredLdrState = analogRead(ldrPin);

        if (lastDetectedLdrState == false && measuredLdrState >= ldrThreshold) {
            // LED has turned on
            lastDetectedLdrState = true;
            lastLdrChangeMillis = currentMillis;
            updateLatency();
        
        } else if ((lastDetectedLdrState == true) && (measuredLdrState <= ldrThreshold)) {
            // LED has turned off
            lastDetectedLdrState = false;
            // Do not measure latency when turning LED off — TODO need a better sensor for that
            // lastLdrChangeMillis = currentMillis;
            // updateLatency();
        }
    }

    updateDisplay();
}

void updateLatency() {
    latencyInMillis = lastLdrChangeMillis - lastLedChangeMillis - offsetInMillis;
    Serial.println(latencyInMillis);
}

void updateDisplay() {
    display.clearDisplay();

    display.setTextSize(1);
    display.setCursor(0,0);
    display.println("Latency");
    display.setTextSize(2);
    display.println(String(latencyInMillis) + " ms");
    display.setTextSize(1);
    display.println("LDR " + String(measuredLdrState));

    display.display();
}

