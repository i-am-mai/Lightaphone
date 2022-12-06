#include "Pitches.h"

// Number of keys on the keyboard
const int numKeys = 5;
// The buzzer's pin
const int buzzer = 8;
// Pins for each led
const int leds[] = {7, 6, 5, 4, 3};
// Pins for each button
const int buttons[] = {13, 12, 11, 10, 9};
// Notes to play for each key
const int notes[] = {NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5};
// Length of pattern to produce in game
const int patternLength = 8;
// Array to store pattern
int pattern[patternLength];

void setup()
{
  for (int i = 0; i < numKeys; i++) {
    pinMode(leds[i], OUTPUT);
    pinMode(buttons[i], INPUT);
  }
  Serial.begin(9600);

  // A5 is unused, so this randomizes the random number generator.
  randomSeed(analogRead(5));
}

void loop()
{
  resetBoard();
  delay(1000);
  game();

// Use free play mode to test whatever buttons you want.
// freePlay();
}

// Allows the user to play any key.
void freePlay()
{
  bool buttonPlayed = false;
  for (int i = 0; i < numKeys; i++) {
    if (digitalRead(buttons[i]) == HIGH) {
      Serial.println(i);
      buttonPlayed = true;
      tone(buzzer, notes[i]);
      digitalWrite(leds[i], HIGH);
      break;
    }
  }
  if (!buttonPlayed)
    noTone(buzzer);
    for (int i = 0; i < numKeys; i++) {
      digitalWrite(leds[i], LOW);
    }
}

// Generates a random pattern of notes
void generatePattern() {
  pattern[0] = random(numKeys);
  for (int i = 1; i < patternLength; i++) {
    int temp = random(numKeys);
    
    // Ensures that each note will be different from the previous.
    while (temp == pattern[i-1]) {
      temp = random(numKeys);
    }
    pattern[i] = temp;
  }
}

// Plays n notes of the ptatern on the keyboard
void playPattern(int n) {
  for (int i = 0; i < n; i++) {
    tone(buzzer, notes[pattern[i]]);
    digitalWrite(leds[pattern[i]], HIGH);
    delay(500);
    digitalWrite(leds[pattern[i]], LOW);
    noTone(buzzer);
  }
}

// Gets input from the player and checks if it matches the pattern.
bool getPlayerInput(int n) {
  int i = 0;              // Keeps track of the index of the pattern the player should play.
  int prevKey = -1;       // Index of previous key played by the player.
  int currKey = -1;       // Index of key the player is currently playing.
  bool isCorrect = true;  // Represents whether the player is playing the right notes.

  while (i < n) {
    currKey = -1;
    bool notePlayed = false;
    for (int j = 0; j < numKeys; j++) {
      if (digitalRead(buttons[j]) == HIGH) {
        currKey = j;
        notePlayed = true;
        tone(buzzer, notes[j]);
        digitalWrite(leds[currKey], HIGH);
      }
    }
    if (!notePlayed) {
      resetBoard();
      continue;
    }
    
    // Ignores inputs that are the same as the previous key or where no button is pressed.
    if (currKey != -1 && currKey != prevKey) {
      // Checks if the note played is correct.
      if (currKey != pattern[i]) {
        isCorrect = false;
        break;
      }
      i++;
    }
    prevKey = currKey;
  }

  // Continues playing the final note so it doesn't end abruptly.
  while (digitalRead(buttons[currKey]) == HIGH) {
    tone(buzzer, notes[currKey]);
    digitalWrite(leds[currKey], HIGH);
  }
  noTone(buzzer);
  digitalWrite(leds[currKey], LOW);
  
  return isCorrect;
}

//// Plays the note corresponding to the button at pin keyVal
//void playNote(int keyVal) {
//  if (keyVal == 0) {
//    noTone(buzzer);
//    for (int i = 0; i < numKeys; i++) {
//      digitalWrite(leds[i], LOW);
//    }
//  }
//  else {
//    for (int i = 0; i < numKeys; i++) {
//      if (keyVal == buttons[i]) {
//        tone(buzzer, notes[i]);
//        digitalWrite(leds[i], HIGH);
//        delay(200);
//        noTone(buzzer);
//        digitalWrite(leds[i], LOW);
//      }
//    }
//  }
//}

// Resets all lights and the buzzer.
void resetBoard() {
  noTone(buzzer);
  for (int i = 0; i < numKeys; i++) {
    digitalWrite(leds[i], LOW);
  }
}

// Plays a key for some specified duration.
void playTone(int key, int duration) {
  tone(buzzer, key);
  delay(duration);
  noTone(buzzer);
}

// Plays the Super Mario Bros. level clear theme.
void gameWin() {
  for (int i = 0; i < numKeys; i++) {
    digitalWrite(leds[i], HIGH);
  }
  playTone(NOTE_G4, 139);
  playTone(NOTE_C5, 139);
  playTone(NOTE_E5, 139);
  playTone(NOTE_G4, 139);
  playTone(NOTE_C5, 139);
  playTone(NOTE_E5, 139);
  playTone(NOTE_G5, 417);
  playTone(NOTE_E5, 417);
  playTone(NOTE_GS4, 139);
  playTone(NOTE_CS5, 139);
  playTone(NOTE_F5, 139);
  playTone(NOTE_GS4, 139);
  playTone(NOTE_CS5, 139);
  playTone(NOTE_F5, 139);
  playTone(NOTE_GS5, 417);
  playTone(NOTE_F5, 417);
  playTone(NOTE_AS4, 139);
  playTone(NOTE_D5, 139);
  playTone(NOTE_F5, 139);
  playTone(NOTE_AS4, 139);
  playTone(NOTE_D5, 139);
  playTone(NOTE_F5, 139);
  playTone(NOTE_AS5, 417);
  playTone(NOTE_AS5, 313);
  playTone(NOTE_AS5, 104);
  playTone(NOTE_C6, 834);
}

// Plays the Super Mario Bros. level fail theme.
void gameLose() {
  playTone(NOTE_C5, 313);
  delay(130);
  playTone(NOTE_G4, 313);
  delay(130);
  playTone(NOTE_E4, 220);
  delay(130);
  playTone(NOTE_A4, 180);
  playTone(NOTE_B4, 180);
  playTone(NOTE_A4, 180);
  playTone(NOTE_GS4, 190);
  playTone(NOTE_AS4, 200);
  playTone(NOTE_GS4, 210);
  playTone(NOTE_G4, 150);
  playTone(NOTE_F4, 150);
  playTone(NOTE_G4, 1200);
}

// Controls the logic of the game.
void game() {
  generatePattern();
  bool gameLost = false;
  int n = 1;
  while (!gameLost && n <= patternLength) {
    playPattern(n);
    if (getPlayerInput(n) == false) {
      gameLost = true;
      resetBoard();
      delay(500);
      break;
    }
    resetBoard();
    delay(1000);
    n++;
  }
  if (gameLost) {
    gameLose();
  }
  else {
    gameWin();
  }
}
