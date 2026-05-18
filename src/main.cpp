#include "Arduino.h"
#include "DFRobotDFPlayerMini.h"
#include "hardware_config.h"

DFRobotDFPlayerMini myDFPlayer;

/*
001 - date change
002 - device connected
003 - device calibrated
004 - travel 1
005 - travel 2

*/
void playDateChangeEffect();
void playDeviceConnectedEffect();
void playDeviceCalibratedEffect();
void playTravel1Effect();
void playTravel2Effect();
void testEffects();
void waitForEffectEnd(int track);

void setup() {
  Serial.begin(115200);

  // Start DFPlayer Serial Connection
  Serial2.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);

  Serial.println("Initializing DFPlayer Mini...");

  // Connect to DFPlayer
  if (!myDFPlayer.begin(Serial2)) {
    Serial.println("DFPlayer Mini NOT detected!");

    while (true);
  }

  Serial.println("DFPlayer Mini Online!");

  Serial.printf("Volume Level: %d\n", VOLUME_LEVEL);
  Serial.println("Playing Track 1");
  myDFPlayer.volume(VOLUME_LEVEL);

  testEffects();
}

void loop() {
}

void testEffects() {
  playDateChangeEffect();
  waitForEffectEnd(1);

  playDeviceConnectedEffect();
  waitForEffectEnd(2);

  playDeviceCalibratedEffect();
  waitForEffectEnd(3);

  playTravel1Effect();
  waitForEffectEnd(4);

  playTravel2Effect();
  waitForEffectEnd(5);
}

void playDateChangeEffect() {
  myDFPlayer.play(1);
}

void playDeviceConnectedEffect() {
  myDFPlayer.play(2);
}

void playDeviceCalibratedEffect() {
  myDFPlayer.play(3);
}

void playTravel1Effect() {
  myDFPlayer.play(4);
}

void playTravel2Effect() {
  myDFPlayer.play(5);
}

void waitForEffectEnd(int track) {
  Serial.printf("[waitForEffectEnd] Waiting for track %d...\n", track);
  while (true) {
    if (myDFPlayer.available()) {
      uint8_t type = myDFPlayer.readType();
      int value = myDFPlayer.read();
      Serial.printf("[waitForEffectEnd] Event type=%d value=%d\n", type, value);
      if (type == DFPlayerPlayFinished && value == track) {
        Serial.printf("[waitForEffectEnd] Track %d finished.\n\n", track);
        break;
      }
    }
  }
}