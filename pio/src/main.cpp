#include <Arduino.h>
#include <Wire.h>

#define I2C_ADDR 0xA5

#define LED_ARRAY_RED_PIN 6
#define LED_ARRAY_GREEN_PIN 7
#define LED_ARRAY_BLUE_PIN 8

volatile uint8_t lastCmd = 0;
volatile uint8_t lastArg = 0;

void onReceive(int len);
void onRequest();
bool setLEDState(uint8_t red, uint8_t green, uint8_t blue);
void CommandParser(int command);


typedef enum Command {
    CMD_NONE = 0x00,
    CMD_LED_ON = 0x01,
    CMD_LED_OFF = 0x02,
    CMD_LED_RED = 0x03,
    CMD_LED_GREEN = 0x04,
    CMD_LED_BLUE = 0x05,
} Command;

void setup() {
    Serial.begin(115200);
    Serial.println("Launching nx-led");
    Wire.setSDA(4); // GPIO4
    Wire.setSCL(5); // GPIO5
    Wire.begin(I2C_ADDR);
    pinMode(LED_ARRAY_RED_PIN, OUTPUT);
    pinMode(LED_ARRAY_GREEN_PIN, OUTPUT);
    pinMode(LED_ARRAY_BLUE_PIN, OUTPUT);
    Wire.onReceive(onReceive);
    Wire.onRequest(onRequest);
}

void onReceive(int len) {
  if (len < 1) return;

  lastCmd = Wire.read();

  if (Wire.available()) {
    lastArg = Wire.read();
  }
}

void onRequest() {
  Wire.write(0xFF);  // OK
}

void loop() {
    CommandParser(lastCmd);
    delay(1000);
}

bool setLEDState(uint8_t red, uint8_t green, uint8_t blue) {
    analogWrite(LED_ARRAY_RED_PIN, red );
    analogWrite(LED_ARRAY_GREEN_PIN, green);
    analogWrite(LED_ARRAY_BLUE_PIN, blue);
    return true;
}

void CommandParser(int command) {
    switch (command) {
        case CMD_LED_ON:
            setLEDState(255, 255, 255);
            break;
        case CMD_LED_OFF:
            setLEDState(0, 0, 0);
            break;
        case CMD_LED_RED:
            analogWrite(LED_ARRAY_RED_PIN, lastArg);
            break;
        case CMD_LED_GREEN:
            analogWrite(LED_ARRAY_GREEN_PIN, lastArg);
            break;
        case CMD_LED_BLUE:
            analogWrite(LED_ARRAY_BLUE_PIN, lastArg);
            break;
        default:
            Serial.println("Unknown command");
            break;
    }
}

