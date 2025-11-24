#ifndef _TUNES_H
#define _TUNES_H

#include "buzzer.h"

uint16_t _melodyJingleBells[] = {
  NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_G5, NOTE_C5, NOTE_D5,
  NOTE_E5,
  NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5,
  NOTE_F5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_D5, NOTE_D5, NOTE_E5,
  NOTE_D5, NOTE_G5
};

uint16_t _melodyCatinaBand[] = {
  // Cantina Band - Star wars 
  // Score available at https://musescore.com/user/6795541/scores/1606876
  // https://github.com/robsoncouto/arduino-songs/blob/master/cantinaband/cantinaband.ino
  NOTE_B4, NOTE_E5, NOTE_B4, NOTE_E5, 
  NOTE_B4, NOTE_E5, NOTE_B4, REST, NOTE_AS4, NOTE_B4,
  NOTE_B4, NOTE_AS4,NOTE_B4, NOTE_A4, REST, NOTE_GS4, NOTE_A4, NOTE_G4,
  NOTE_G4, NOTE_E4,
  NOTE_B4, NOTE_E5, NOTE_B4, NOTE_E5, 
  NOTE_B4, NOTE_E5, NOTE_B4, REST, NOTE_AS4,NOTE_B4,

  NOTE_A4, NOTE_A4, NOTE_GS4, NOTE_A4,
  NOTE_D5, NOTE_C5, NOTE_B4, NOTE_A4,
  NOTE_B4, NOTE_E5, NOTE_B4, NOTE_E5, 
  NOTE_B4, NOTE_E5, NOTE_B4, REST, NOTE_AS4,NOTE_B4,
  NOTE_D5, NOTE_D5, NOTE_B4, NOTE_A4,
  NOTE_G4, NOTE_E4,
  NOTE_E4, NOTE_G4,
  NOTE_B4, NOTE_D5,

  NOTE_F5, NOTE_E5, NOTE_AS4,NOTE_AS4, NOTE_B4, NOTE_G4
};

uint16_t _melodyImperialMarch[] = {
  // Dart Vader theme (Imperial March) - Star wars 
  // Score available at https://musescore.com/user/202909/scores/1141521
  // The tenor saxophone part was used
  // https://github.com/robsoncouto/arduino-songs/blob/master/imperialmarch/imperialmarch.ino
  
  // NOTE_A4, NOTE_A4, NOTE_A4, NOTE_A4, NOTE_A4, NOTE_A4, NOTE_F4, REST,
  NOTE_A4, NOTE_A4, NOTE_A4, NOTE_A4, NOTE_A4, NOTE_A4, NOTE_F4, REST,
  NOTE_A4, NOTE_A4, NOTE_A4, NOTE_F4, NOTE_C5,

  NOTE_A4, NOTE_F4, NOTE_C5, NOTE_A4,//4
  NOTE_E5, NOTE_E5, NOTE_E5, NOTE_F5, NOTE_C5,
  NOTE_A4, NOTE_F4, NOTE_C5, NOTE_A4,
  
  NOTE_A5, NOTE_A4, NOTE_A4, NOTE_A5, NOTE_GS5, NOTE_G5, //7 
  NOTE_DS5, NOTE_D5, NOTE_DS5, REST, NOTE_A4, NOTE_DS5, NOTE_D5, NOTE_CS5,

  NOTE_C5, NOTE_B4, NOTE_C5, REST, NOTE_F4, NOTE_GS4, NOTE_F4, NOTE_A4,//9
  NOTE_C5, NOTE_A4, NOTE_C5, NOTE_E5,

  NOTE_A5, NOTE_A4, NOTE_A4, NOTE_A5, NOTE_GS5, NOTE_G5, //7 
  NOTE_DS5, NOTE_D5, NOTE_DS5, REST, NOTE_A4, NOTE_DS5, NOTE_D5, NOTE_CS5,

  NOTE_C5, NOTE_B4, NOTE_C5, REST, NOTE_F4, NOTE_GS4, NOTE_F4, NOTE_A4,//9
  NOTE_A4, NOTE_F4, NOTE_C5, NOTE_A4,
};

// note durations: 4 = quarter note, 8 = eighth note, etc, also called tempo:
int16_t _melodyNoteDurationsJingleBells[] = {
  8, 8, 4,
  8, 8, 4,
  8, 8, 8, 8,
  2,
  8, 8, 8, 8,
  8, 8, 8, 16, 16,
  8, 8, 8, 8,
  4, 4
};

int16_t _melodyNoteDurationsCatinaBand[] = {
  // Cantina Band - Star wars 
  // Score available at https://musescore.com/user/6795541/scores/1606876
  // https://github.com/robsoncouto/arduino-songs/blob/master/cantinaband/cantinaband.ino
  -4, -4, -4, -4, 
  8, -4, 8, 8,  8, 8, 
  8, 8, 8, 8, 8, 8, 8, 8,
  4, -2, 
  -4, -4, -4, -4, 
  8, 4, 8, 8, 8, 8,

  -4, -4, 8, -4,
  8,  -4, -4, -4,
  -4, -4, -4, -4, 
  8,  -4, 8, 8, 8, 8,
  4, -4, 8, -4,
  -4, -2,
  2, 2,
  2, 2,

  -4, -4, 8, 8, 4, 4
};

int16_t _melodyNoteDurationsImperialMarch[] = {
  // Dart Vader theme (Imperial March) - Star wars 
  // Score available at https://musescore.com/user/202909/scores/1141521
  // The tenor saxophone part was used
  // https://github.com/robsoncouto/arduino-songs/blob/master/imperialmarch/imperialmarch.ino
  
  // -4, -4, 16, 16, 16, 16, 8, 8,
  -4, -4, 16, 16, 16, 16, 8, 8,
  4, 4, 4, -8, 16,

  4, -8, 16, 2,//4
  4, 4, 4, -8, 16,
  4, -8, 16, 2,
  
  4, -8, 16, 4, -8, 16, //7 
  16, 16, 8, 8, 8, 4, -8, 16,

  16, 16, 16, 8, 8, 4, -8, -16,//9
  4, -8, 16, 2,

  4, -8, 16, 4, -8, 16, //7 
  16, 16, 8, 8, 8, 4, -8, 16,

  16, 16, 16, 8, 8, 4, -8, -16,//9
  4, -8, 16, 2,
};

uint16_t _melodyTempoJingleBells = 140;
uint16_t _melodyTempoCatinaBand = 280;
uint16_t _melodyTempoImperialMarch = 120;

#endif