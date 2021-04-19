#include "Pitches.h" 




void setup() {
  //fuck
}


void loop() {

int melody[] = {NOTE_C5, NOTE_C5, NOTE_C5, NOTE_C5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_D5, NOTE_D5, NOTE_D5,NOTE_D5, NOTE_G5, NOTE_G5, NOTE_G5,
NOTE_G5, NOTE_A5, NOTE_A5, NOTE_A5,NOTE_A5, NOTE_A5, NOTE_A5, NOTE_A5,NOTE_A5, NOTE_A5,NOTE_A5, NOTE_A5,NOTE_A5, NOTE_D5, NOTE_C5, NOTE_B4, NOTE_G4};

//note durations. 4=quarter note / 8=eighth note
int noteDurations[] = {4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,  4, 4, 4, 4, 4, 4, 4, 4};

  for (int thisNote = 0; thisNote < 36; thisNote++) {

    int noteDuration = 800 / noteDurations [thisNote];
    tone(8, melody [thisNote], noteDuration);

    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);

    //stop the tone playing
    noTone(8);
  }

//-----------------------------------------------------------------------------------------------------------------------------
  int melody2[] = {NOTE_A4, 0, NOTE_A4, NOTE_E5, NOTE_D5, 0, NOTE_C5, 0, NOTE_B4, 0, NOTE_B4,NOTE_B4, NOTE_D5, 0, NOTE_C5,
NOTE_B4, NOTE_A4, 0, NOTE_A4, NOTE_C6, NOTE_B5, NOTE_C6, NOTE_B5, NOTE_C6, NOTE_A4, 0, NOTE_A4, NOTE_C6, NOTE_B5, NOTE_C6, NOTE_B5, NOTE_C6};

//note durations. 4=quarter note / 8=eighth note
int noteDuration2[] = {4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4};

  for (int thisNote = 0; thisNote < 32; thisNote++) {

    int noteDuration = 800 / noteDuration2 [thisNote];
    tone(8, melody2 [thisNote], noteDuration);

    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);

    //stop the tone playing
    noTone(8);
  }

}
