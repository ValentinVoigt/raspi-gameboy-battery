#include <Keyboard.h>

#define BATTERY_VOLTAGE A1
#define NUMBER_OF_BUTTONS 15
#define ADC_OVERSAMPLE_BITS 6

const int BUTTON_PINS[NUMBER_OF_BUTTONS] = {
  2,
  3,
  4,
  5,

  6,
  7,
  8,
  9,

  10,
  16,
  14,
  15,

  18,
  20,
  21
};

const int BUTTON_KEYS[NUMBER_OF_BUTTONS] = {
  '1',
  '2',
  '3',
  '4',

  'x',
  'y',
  KEY_RETURN,
  KEY_BACKSPACE,

  KEY_F1,
  KEY_F2,
  KEY_F3,
  KEY_DOWN_ARROW,
  
  KEY_LEFT_ARROW,
  KEY_UP_ARROW,
  KEY_RIGHT_ARROW
};

unsigned long BUTTON_TIMEOUTS[NUMBER_OF_BUTTONS];
int BUTTON_STATES[NUMBER_OF_BUTTONS];
unsigned int adc = 0;
unsigned char measurements = 0;
unsigned long next_voltage_send = 0;
unsigned long nextSerialDump = 0;

void setup() {
  // Set pin modes and init variables
  pinMode(LED_BUILTIN_RX, OUTPUT);
  pinMode(LED_BUILTIN_TX, OUTPUT);
  pinMode(BATTERY_VOLTAGE, INPUT);
  analogReference(INTERNAL);

  for (int idx = 0; idx < NUMBER_OF_BUTTONS; idx++) {
    pinMode(BUTTON_PINS[idx], INPUT_PULLUP);
    BUTTON_TIMEOUTS[idx] = 0;
    BUTTON_STATES[idx] = HIGH;
  }

  // Blink for 5 seconds
  for (int i = 0; i < 25; i++) {
    digitalWrite(LED_BUILTIN_RX, LOW);
    digitalWrite(LED_BUILTIN_TX, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN_RX, HIGH);
    digitalWrite(LED_BUILTIN_TX, LOW);
    delay(100);
  }

  // Turn off LEDs
  digitalWrite(LED_BUILTIN_RX, HIGH);
  digitalWrite(LED_BUILTIN_TX, HIGH);

  // Start keyboard mode
  Keyboard.begin();
  Serial.begin(115200);
}

void loop() {
  unsigned long currentTime = millis();

  for (int idx = 0; idx < NUMBER_OF_BUTTONS; idx++) {
    int newState = digitalRead(BUTTON_PINS[idx]);
    if (newState != BUTTON_STATES[idx] && BUTTON_TIMEOUTS[idx] < currentTime) {
      BUTTON_STATES[idx] = newState;
      BUTTON_TIMEOUTS[idx] = currentTime + 50;
      if (newState == HIGH) {
        Keyboard.release(BUTTON_KEYS[idx]);
      } else {
        Keyboard.press(BUTTON_KEYS[idx]);
      }
    }
  }

  if (currentTime > next_voltage_send) {
    if (measurements < (1 << ADC_OVERSAMPLE_BITS)) {
      adc += analogRead(BATTERY_VOLTAGE);
      measurements++;
    } else {
      Serial.println(adc >> ADC_OVERSAMPLE_BITS);
      adc = 0;
      measurements = 0;
      next_voltage_send = currentTime + 5000;
    }
  }
}
