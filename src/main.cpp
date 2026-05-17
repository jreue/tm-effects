#include "Arduino.h"
#include "DFRobotDFPlayerMini.h"

// =======================
// DFPlayer Mini Pins
// =======================
#define RXD2 16
#define TXD2 17

// =======================
// Button Pins
// =======================
#define BTN_PLAYPAUSE 22
#define BTN_NEXT 21
#define BTN_PREV 19
#define BTN_VOLUP 18
#define BTN_VOLDOWN 5

// =======================
// DFPlayer Object
// =======================
DFRobotDFPlayerMini myDFPlayer;

// =======================
// Variables
// =======================
int volumeLevel = 20;  // Volume range: 0-30
bool isPaused = false;

void setup() {
  // Start Serial Monitor
  Serial.begin(115200);

  // Start DFPlayer Serial Connection
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);

  Serial.println("Initializing DFPlayer Mini...");

  // Connect to DFPlayer
  if (!myDFPlayer.begin(Serial2)) {
    Serial.println("DFPlayer Mini NOT detected!");
    Serial.println("Check wiring and SD card.");

    while (true);
  }

  Serial.println("DFPlayer Mini Online!");

  // =======================
  // Button Setup
  // =======================
  pinMode(BTN_PLAYPAUSE, INPUT_PULLUP);
  pinMode(BTN_NEXT, INPUT_PULLUP);
  pinMode(BTN_PREV, INPUT_PULLUP);
  pinMode(BTN_VOLUP, INPUT_PULLUP);
  pinMode(BTN_VOLDOWN, INPUT_PULLUP);

  // =======================
  // Initial Settings
  // =======================
  myDFPlayer.volume(volumeLevel);

  // Start playing first song
  myDFPlayer.play(1);

  Serial.println("Playing Track 1");
}

void loop() {
  // =======================
  // PLAY / PAUSE BUTTON
  // =======================
  if (digitalRead(BTN_PLAYPAUSE) == LOW) {
    if (isPaused) {
      myDFPlayer.start();
      Serial.println("Resume Music");
    } else {
      myDFPlayer.pause();
      Serial.println("Pause Music");
    }

    isPaused = !isPaused;

    delay(300);  // debounce
  }

  // =======================
  // NEXT TRACK BUTTON
  // =======================
  if (digitalRead(BTN_NEXT) == LOW) {
    myDFPlayer.next();

    Serial.println("Next Track");

    delay(300);  // debounce
  }

  // =======================
  // PREVIOUS TRACK BUTTON
  // =======================
  if (digitalRead(BTN_PREV) == LOW) {
    myDFPlayer.previous();

    Serial.println("Previous Track");

    delay(300);  // debounce
  }

  // =======================
  // VOLUME UP BUTTON
  // =======================
  if (digitalRead(BTN_VOLUP) == LOW) {
    if (volumeLevel < 30) {
      volumeLevel++;

      myDFPlayer.volume(volumeLevel);

      Serial.print("Volume Up: ");
      Serial.println(volumeLevel);
    }

    delay(200);  // debounce
  }

  // =======================
  // VOLUME DOWN BUTTON
  // =======================
  if (digitalRead(BTN_VOLDOWN) == LOW) {
    if (volumeLevel > 0) {
      volumeLevel--;

      myDFPlayer.volume(volumeLevel);

      Serial.print("Volume Down: ");
      Serial.println(volumeLevel);
    }

    delay(200);  // debounce
  }
}