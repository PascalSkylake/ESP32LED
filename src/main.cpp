#include <Arduino.h>
#include <FastLED.h>
#define FASTLED_INTERNAL

#define NUM_LEDS  157
#define LED_PIN   23

CRGB leds[NUM_LEDS] = {0};
int counter = 0;
uint8_t* a = (u_int8_t*) malloc(((NUM_LEDS / 8)) * sizeof(uint8_t));

TaskHandle_t renderer;

float currentPosition = 0;
float sections = 2;
float speedFactor = 0.2;

void allOff();
void solidColor();
void countBinary();
void rainbow();
void render(void *parameter);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  


  //Serial.begin(115200);
  //Serial.println("start");
  //Serial.println(((NUM_LEDS / 8)) * sizeof(uint8_t));
  FastLED.addLeds<WS2812B, LED_PIN, BRG>(leds, NUM_LEDS);
  FastLED.setBrightness(255);

  for (int i = 0; i < ((NUM_LEDS / 8)) * sizeof(uint8_t); i++) {
    a[i] = 0;
  }

  /*
  xTaskCreatePinnedToCore(
    render,
    "render",
    10000,
    NULL,
    0,
    &renderer,
    1
  );
  */
}

void render(void *parameter) {
  for (;;) {
    //countBinary();
  }
}

void setBit(int i, uint8_t* array, int value) {
  u_int8_t byte = i / 8;
  u_int8_t bit = i % 8;

  if (value == 1) {
    array[byte] |= (1 << (bit));
  }

  if (value == 0) {
    array[byte] &= ~(1 << (bit));
  }
}

u_int8_t getBit(int i, uint8_t* array) { 
  u_int8_t byte = i / 8;
  u_int8_t bit = i % 8;

  return (array[byte] >> (bit)) & 1;


}

void addBit(uint8_t* array) {
  int i = 0;

  while (i < ((NUM_LEDS / 8)) * sizeof(uint8_t) * 8) {
    int bit = getBit(i, array);

    if (bit == 0) {
      setBit(i, array, 1);
      break;
    } else {
      setBit(i, array, 0);
      i++;
    }
  }


}

void printBits(uint8_t* array) {
  for (int i = 0; i < ((NUM_LEDS / 8)) * sizeof(uint8_t) * 8; i++) {
    Serial.print(getBit(i, array));
  }
  Serial.print("\n");
}

void loop() {
  countBinary();
  FastLED.show();
}

void solidColor() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].setRGB(255, 255, 200);
  }
}

void allOff() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = 0;
  }
}

void countBinary() {
  for (int i = 0; i < NUM_LEDS; i++) {
    if (getBit(i, a) == 1) {
      leds[i].setRGB(1, 0, 0);
    } else {
      leds[i] = 0;
    }
  }
  addBit(a);
}

void rainbow() {
  currentPosition += speedFactor;

  for (int i = 0; i < NUM_LEDS; i++) {
      float adjustedPosition = fmod(i + currentPosition, (float) NUM_LEDS);

      if (adjustedPosition < 0) {
        adjustedPosition += NUM_LEDS;
      }

      float hue = (adjustedPosition / (NUM_LEDS / sections));

      leds[i].setHSV((uint8_t) (hue * 255), 255, 100);
    }
}


