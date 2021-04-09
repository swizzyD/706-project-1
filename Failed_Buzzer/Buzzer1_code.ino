/*





void Buzzer1_code( void * parameter) {
  while (true) {
    Serial.print("buzzer1 running on core ");
    Serial.println(xPortGetCoreID());

    //-----------------------------------------
    int length = 9; // the number of notes
    char notes[] = "cccceeeeddddggggaaaaaaaaaaaadcBG"; // a space represents a rest
    int beats[] = { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};
    for (int i = 0; i < length; i++) {
      if (notes[i] == ' ') {
        delay(beats[i] * tempo); // rest
      } else {
        playNote_1(notes[i], beats[i] * tempo);
      }

      // pause between notes
      delay(tempo / 2);
    }
    //-----------------------------------------
    int length2 = 5; // the number of notes
    char notes2[] = "efaee"; // a space represents a rest
    int beats2[] = { 2, 2, 2, 3, 3};
    while (true) {
      for (int i = 0; i < length2; i++) {
        if (notes[i] == ' ') {
          delay(beats[i] * tempo); // rest
        } else {
          playNote_1(notes2[i], beats2[i] * tempo);
        }

        // pause between notes
        delay(tempo / 2);
      }
    }

  }
}


void playTone_1(int tone, int duration) {
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(buzzer1_pin, HIGH);
    delayMicroseconds(tone);
    digitalWrite(buzzer1_pin, LOW);
    delayMicroseconds(tone);
  }
}

void playNote_1(char note, int duration) {
  char names[] = { 'G', 'A', 'B', 'c', 'd', 'e', 'f', 'g', 'a', 'b', '/' };
  int tones[] = {3000, 2500 ,2100 , 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 };

  // play the tone corresponding to the note name
  for (int i = 0; i < 8; i++) {
    if (names[i] == note) {
      playTone_1(tones[i], duration);
    }
  }
}


*/
