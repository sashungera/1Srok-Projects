#include "ESP_I2S.h"
#include "BluetoothA2DPSink.h"


const uint8_t I2S_BCLK = 26;
const uint8_t I2S_LRC  = 27;
const uint8_t I2S_DOUT = 25;

const int BTN_LEFT  = 22;
const int BTN_RIGHT = 23;
const int BTN_BOTH  = 19;

const int AMP_LEFT_SD  = 32;
const int AMP_RIGHT_SD = 33;

I2SClass i2s;
BluetoothA2DPSink a2dp_sink(i2s);

void setup() {
  Serial.begin(115200);

  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);
  pinMode(BTN_BOTH, INPUT_PULLUP);

  pinMode(AMP_LEFT_SD, OUTPUT);
  pinMode(AMP_RIGHT_SD, OUTPUT);

  digitalWrite(AMP_LEFT_SD, HIGH);
  digitalWrite(AMP_RIGHT_SD, HIGH);

  i2s.setPins(I2S_BCLK, I2S_LRC, I2S_DOUT);

  if (!i2s.begin(
        I2S_MODE_STD,
        44100,
        I2S_DATA_BIT_WIDTH_16BIT,
        I2S_SLOT_MODE_STEREO,
        I2S_STD_SLOT_BOTH)) {
    Serial.println("❌ I2S init failed");
    while (1);
  }

  a2dp_sink.start("MyMusic");
  Serial.println("✅ Bluetooth speaker started: MyMusic");
  Serial.println("Waiting for connection...");
}

unsigned long lastPress = 0;
const unsigned long debounce = 200;

void loop() {
  unsigned long now = millis();

  if (digitalRead(BTN_LEFT) == LOW && now - lastPress > debounce) {
    digitalWrite(AMP_LEFT_SD, HIGH);
    digitalWrite(AMP_RIGHT_SD, LOW);
    Serial.println("🔊 LEFT speaker only");
    lastPress = now;
  }

  if (digitalRead(BTN_RIGHT) == LOW && now - lastPress > debounce) {
    digitalWrite(AMP_LEFT_SD, LOW);
    digitalWrite(AMP_RIGHT_SD, HIGH);
    Serial.println("🔊 RIGHT speaker only");
    lastPress = now;
  }

  if (digitalRead(BTN_BOTH) == LOW && now - lastPress > debounce) {
    digitalWrite(AMP_LEFT_SD, HIGH);
    digitalWrite(AMP_RIGHT_SD, HIGH);
    Serial.println("🔊 BOTH speakers");
    lastPress = now;
  }
}
