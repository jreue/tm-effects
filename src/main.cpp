#include <shared_hardware_config.h>

#include "Arduino.h"
#include "DFRobotDFPlayerMini.h"
#include "EspNowHelper.h"
#include "hardware_config.h"

DFRobotDFPlayerMini myDFPlayer;
EspNowHelper espNowHelper;

/*
001 - date change
002 - device connected
003 - device calibrated
004 - travel 1
005 - travel 2

*/
void handleScannerMessage(const ScannerMessage& msg);
void handleShieldModuleMessage(const ShieldModuleMessage& msg);
void handleTravelMessage(const DateMessage& msg);
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
  Serial.printf("Setting Volume Level: %d\n", VOLUME_LEVEL);
  myDFPlayer.volume(VOLUME_LEVEL);

  espNowHelper.begin(DEVICE_ID);
  espNowHelper.registerScannerMessageHandler(handleScannerMessage);
  espNowHelper.registerModuleMessageHandler(handleShieldModuleMessage);
  espNowHelper.registerDateMessageHandler(handleTravelMessage);

  // testEffects();
  playTravel1Effect();
}

void loop() {
}

void handleScannerMessage(const ScannerMessage& msg) {
  Serial.println("Handling Scanner Message:");
  Serial.printf("  Scanner ID: %d\n", msg.deviceId);
  Serial.printf("  Message Type: %d\n", msg.messageType);

  if (msg.messageType == MSG_TYPE_CONNECT) {
    Serial.println("Scanner Connected!");
    playDeviceCalibratedEffect();
  }
}

void handleShieldModuleMessage(const ShieldModuleMessage& msg) {
  Serial.println("Handling Shield Module Message:");
  Serial.printf("  Shield Module ID: %d\n", msg.deviceId);
  Serial.printf("  Message Type: %d\n", msg.messageType);
  Serial.printf("  Is Calibrated: %s\n", msg.isCalibrated ? "Yes" : "No");

  if (msg.messageType == MSG_TYPE_CONNECT) {
    Serial.println("Shield Module Connected!");
    playDeviceConnectedEffect();
  } else if (msg.messageType == MSG_TYPE_DATA) {
    if (msg.isCalibrated) {
      Serial.println("Shield Module Calibrated!");
      playDeviceCalibratedEffect();
    }
  }
}

void handleTravelMessage(const DateMessage& msg) {
  Serial.println("Handling Travel Message:");
  Serial.printf("  Date Module ID: %d\n", msg.deviceId);
  Serial.printf("  Message Type: %d\n", msg.messageType);
  Serial.printf("  Date: %02d/%02d/%04d\n", msg.month, msg.day, msg.year);

  if (msg.messageType == MSG_TYPE_DATA) {
    playTravel2Effect();
  }
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