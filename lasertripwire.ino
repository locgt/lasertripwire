//Laser tripwire sketch - @vmfoo

//playMelody function and pitches.h borrowed from arduino tone tutorial and adapted
//http://arduino.cc/en/Tutorial/Tone 

#include "pitches.h"  //include the frequency definitions of the pitches

#define PIEZO_PIN 3
#define ARMED_PIN 2
#define RECEIVER_PIN A7
#define THRESHOLD 80 
#define LED_PIN 13

// notes in the melody:
int melody[] =
{NOTE_G3, NOTE_G3, NOTE_G3, NOTE_DS3, NOTE_AS3, NOTE_G3, NOTE_DS3, NOTE_AS3, NOTE_G3, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_DS4, NOTE_AS3, NOTE_FS3, NOTE_DS3, NOTE_AS3, NOTE_G3};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = { 4, 4, 4, 6,12,4,6,12,2, 4, 4, 4, 6,12,4,6,12,2 };

void setup() {
  Serial.begin(19200); //for debug
  pinMode(PIEZO_PIN, OUTPUT);  //setup the pin to make noise on
  pinMode(LED_PIN, OUTPUT);  //setup the pin to light when set
  pinMode(ARMED_PIN, INPUT); //we are going to look for this to go high
}

void loop() {
  // read the input on RECEIVER_PIN
  int sensorValue = analogRead(RECEIVER_PIN);
  // print out the value you read if debugging
  //Serial.println(sensorValue);
  if( sensorValue < THRESHOLD ) {
    digitalWrite(LED_PIN, HIGH);  //Turn it on to show that the trap is set
  } else {
    digitalWrite(LED_PIN, LOW);  //Turn it off 
  }
  if (digitalRead(ARMED_PIN) && (sensorValue > THRESHOLD) ) { //make noise if the trap is armed and the laser is off
    Serial.println("INTRUDER ALERT!!");
    playMelody();  //this funciton blocks until melody is over
  }
}



void playMelody() {
// iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 18; thisNote++) {

    // to calculate the note duration, take one second 
    // divided by the note type.
    // e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    // I bumped 1000 up to 1500 to slow the tempo. raise it more to slow it down
    int noteDuration = 1500/noteDurations[thisNote];
    tone(PIEZO_PIN, melody[thisNote],noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(PIEZO_PIN);
  }
}

