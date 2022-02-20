/* --- NEOPIXEL --- */
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#define PIXELS_PIN 6
#define NUM_PIXELS 16
#define MAX_BRIGHTNESS 255

/* --- RESISTOR --- */
#define RESISTOR_PIN A0
#define MAX_ANALOGUE_READ 1024

/* --- BUTTON --- */
#define BUTTON_PIN 2

/* --- COLOURS --- */
#define WHITE 0xFFFFFF
#define OFF_WHITE 0xFAF9F6
#define BEIGE 0xF5F5DC
#define ANTIQUE_WHITE 0xFAEBD7
#define PEACH 0xFFE5B4
// #define ANOTHER_COLOUR 0xFFFFFF

// Increase this if more colours are added
#define NUM_COLOURS 5

/* --- GLOBALS --- */
Adafruit_NeoPixel pixels(NUM_PIXELS, PIXELS_PIN, NEO_GRB + NEO_KHZ800);
int previous_resistor_read = -1;
int previous_button_state = LOW;

// Add new colours to this array
uint32_t colours[NUM_COLOURS] = {WHITE, OFF_WHITE, BEIGE, ANTIQUE_WHITE, PEACH};
int current_colour_index = 0;

void setup() {
  Serial.begin(9600);

  // Initialise button
  pinMode(BUTTON_PIN, INPUT);

#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif

  // Initialise NeoPixels
  pixels.setBrightness(100);
  pixels.begin();
  pixels.clear();
}

/**
 * Converts an analogueRead() value between 1 and 1024 to
 * a NeoPixel brightness value beween 0 and 255.
 */
int analogueToBrightness(int analogue_value) {
  float fraction = (float)analogue_value / (float)MAX_ANALOGUE_READ;
  int brightness = fraction * MAX_BRIGHTNESS;

  return brightness;
}

/**
 * Read the potentiometer on RESISTOR_PIN and convert the
 * analogue value to a scaled brightness, then set the NeoPixels
 * to this brightness.
 */
void adjustBrightness() {
  int resistor_read = analogRead(RESISTOR_PIN);

  // Only change brightness if resistor value has changed within a tolerance (1 either side of prevous read)
  if (resistor_read > previous_resistor_read + 1 || resistor_read < previous_resistor_read - 1) {
    previous_resistor_read = resistor_read;

    // Convert resistor value to led brightness
    int brightness = analogueToBrightness(resistor_read + 1);

    // Set pixels brightness and redraw
    pixels.setBrightness(brightness);
    pixels.fill(colours[current_colour_index], 0, NUM_PIXELS - 1);
    pixels.show();
  }
}

/**
 * Check if the push-button has been pressed (after its been released)
 * and cycle to the next colour in the colours array if so. Applies
 * this colour to all NeoPixels.
 */
void checkChangeColour() {
  int button_state = digitalRead(BUTTON_PIN);

  // Check if button was pressed after it was released (poor-man's debouncing)
  if (button_state == HIGH && previous_button_state == LOW) {
    previous_button_state = HIGH;

    // Select the next colour, wrap around if we reached the end of the array
    if (current_colour_index < (NUM_COLOURS - 1)) {
      current_colour_index++;
    } else {
      current_colour_index = 0;
    }

    // Set pixels to new colour
    pixels.fill(colours[current_colour_index], 0, NUM_PIXELS - 1);
    pixels.show();
  } else if (button_state == LOW) {
    // Set previous state to low if button is not pressed, so we can register another press
    previous_button_state = LOW;
  }
}

void loop() {
  // Adjust brightness via potentiometer
  adjustBrightness();

  // Change colour if button pressed
  checkChangeColour();
}
