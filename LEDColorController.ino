/*
  Simple example for receiving
  
  https://github.com/sui77/rc-switch/
*/
#include <Adafruit_NeoPixel.h>
#include <RCSwitch.h>

#define arr_len(x) (sizeof(x) / sizeof(*x))

#define BUTTON_A (16646692)
#define BUTTON_B (16646689)
#define BUTTON_C (16646696)
#define BUTTON_D (16646690)

#define BUTTON_BRIGHTER BUTTON_A
#define BUTTON_DARKER BUTTON_C
#define BUTTON_NEXT BUTTON_B
#define BUTTON_PREVIOUS BUTTON_D

#define LED_DATA_OUT_PIN 10

Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, LED_DATA_OUT_PIN, NEO_RGB + NEO_KHZ400);
RCSwitch rf = RCSwitch();

uint32_t colors[] = {
  0xbdff0000,
  0xbdffff00,
  0xbd00ff00,
  0xbd00ffff,
  0xbd0000ff,
  0xbdff00ff,
  0xbdffffff,
  0xbd000000,
  
};

int color_index = 0;
uint32_t color = colors[color_index];

#define INTENSITY_MAX (1.0)
#define INTENSITY_MIN (0.1)
#define INTENSITY_STEP (0.1)
float intensity = INTENSITY_MAX;

void setup() {
  strip.begin();
  strip.show();
  rf.enableReceive(0);  // Receiver on interrupt 0 => that is pin #2
}

void loop() {
  if (rf.available()) {
    switch (rf.getReceivedValue()) {
      case BUTTON_BRIGHTER:
        if (intensity < INTENSITY_MAX) {
          if (intensity + INTENSITY_STEP >= INTENSITY_MAX){
            intensity = INTENSITY_MAX;
          } else {
            intensity += INTENSITY_STEP;
          }
        }
        break;
      case BUTTON_DARKER:
        if (intensity > INTENSITY_MIN) {
          if (intensity - INTENSITY_STEP <= INTENSITY_MIN){
            intensity = INTENSITY_MIN;
          } else {
            intensity -= INTENSITY_STEP;
          }
        }
        break;
      case BUTTON_NEXT:
        color_index = (color_index + 1) % arr_len(colors);
        color = colors[color_index];
        break;
      case BUTTON_PREVIOUS:
        color_index = (color_index - 1) % arr_len(colors);
        color = colors[color_index];
        break;
    }
    setStripColor(color, intensity);
    rf.resetAvailable();
  }
}

void setStripColor(uint32_t color, float intensity) {
  color = applyIntensity(color, intensity);
  for(uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, color);
  }
  strip.setPixelColor(2, 0xffffffff);
  strip.show();
  strip.show();
}

uint32_t applyIntensity(uint32_t color, float intensity) {
  uint32_t red = ((uint32_t) (((color & 0xFF0000) >> 16) * intensity)) << 16;
  uint32_t green = ((uint32_t) (((color & 0xFF00) >> 8) * intensity)) << 8;
  uint32_t blue = (uint32_t) (((color & 0xFF) * intensity));
  return (red | green | blue);
}

