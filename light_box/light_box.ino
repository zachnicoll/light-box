/* --- NEOPIXEL --- */
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#define PIXELS_PIN 6
#define NUM_PIXELS 24
#define MAX_BRIGHTNESS 255

/* --- RESISTOR --- */
#define RESISTOR_PIN A4
#define MAX_ANALOGUE_READ 1024

/* --- BUTTON --- */
#define BUTTON_PIN 2

/* --- COLOURS --- */
#define WHITE 0xFFFFFF
#define COBALT 0x0047AB
#define HOT_PINK 0xE30022
#define LIGHT_BLUE 0x2E8B57
#define LIGHT_RED 0xCD4F39
#define RED 0xFF2800
#define GREEN 0x228B22
// #define ANOTHER_COLOUR 0xFFFFFF

// Increase this if more colours are added
#define NUM_COLOURS 7

// Add new colours to this array
uint32_t colours[NUM_COLOURS] = {WHITE, LIGHT_BLUE, COBALT, LIGHT_RED, HOT_PINK, RED, GREEN};
int current_colour_index = 0;

/* --- GLOBALS --- */
Adafruit_NeoPixel pixels(NUM_PIXELS, PIXELS_PIN, NEO_GRB + NEO_KHZ800);
int previous_resistor_read = -1;
int previous_button_state = HIGH;

/**
   Keeps track of how many cycles the button has been held down for
*/
int debounce_count = 0;

void setup()
{
  Serial.begin(9600);

  // Initialise button
  pinMode(BUTTON_PIN, INPUT);
  digitalWrite(BUTTON_PIN, HIGH);

  // Initialise potentiometer
  pinMode(RESISTOR_PIN, INPUT);

  // Initialise NeoPixels
  pixels.setBrightness(100);
  pixels.begin();
  pixels.clear();
}

/**
   Converts an analogueRead() value between 1 and 1024 to
   a NeoPixel brightness value beween 0 and 255.
*/
int analogueToBrightness(int analogue_value)
{
  float fraction = (float)analogue_value / (float)MAX_ANALOGUE_READ;
  int brightness = fraction * MAX_BRIGHTNESS;

  return brightness;
}

/**
   Read the potentiometer on RESISTOR_PIN and convert the
   analogue value to a scaled brightness, then set the NeoPixels
   to this brightness.
*/
void adjustBrightness()
{
  int resistor_read = analogRead(RESISTOR_PIN);
  int threshold;

  // The analog read on the potentiometer seems to get more sensistive towards the
  // smaller values. Increasing the threshold for changing brightness at these levels
  // reduces flickering.
  if (resistor_read < 300)
    threshold = 15;
  else if (resistor_read < 150)
    threshold = 20;
  else
    threshold = 5;

  // Only change brightness if resistor value has changed within a tolerance (1 either side of prevous read)
  if (resistor_read > previous_resistor_read + threshold || resistor_read < previous_resistor_read - threshold)
  {
    if (resistor_read < 20)
    {
      resistor_read = 0;
    }

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
   Check if the push-button has been pressed (after its been released)
   and cycle to the next colour in the colours array if so. Applies
   this colour to all NeoPixels.
*/
void checkChangeColour()
{
  int button_state = digitalRead(BUTTON_PIN);

  /**
   * LOW == button pressed
   * HIGH == button not pressed
   */

  // Check if button was pressed after it was released, and that its been held down
  // for sufficiently long (debounced)
  if (button_state == LOW && previous_button_state == HIGH && debounce_count > 100)
  {
    previous_button_state = LOW;
    debounce_count = 0;

    // Select the next colour, wrap around if we reached the end of the array
    if (current_colour_index < (NUM_COLOURS - 1))
    {
      current_colour_index++;
    }
    else
    {
      current_colour_index = 0;
    }

    uint32_t colour = colours[current_colour_index];

    // Set pixels to new colour
    pixels.fill(colour, 0, NUM_PIXELS - 1);
    pixels.show();
  }
  else if (button_state == LOW)
  {
    // Button is being held down, increase debounce count
    debounce_count++;
  }
  else if (button_state == HIGH)
  {
    // Set previous state to low if button is not pressed, so we can register another press
    previous_button_state = HIGH;
    debounce_count = 0;
  }
}

void loop()
{
  // Adjust brightness via potentiometer
  adjustBrightness();

  // Change colour if button pressed
  checkChangeColour();
}
