/*





void Buzzer2_code( void * parameter) {
  while (true) {
    Serial.print("buzzer2 running on core ");
    Serial.println(xPortGetCoreID());

    //-------------------------------
    int length = 9; // the number of notes
    char notes[] = "        Cg"; // a space represents a rest
    int beats[] = { 6, 6, 6, 6, 3, 3, 3, 6, 3, 3};
    for (int i = 0; i < length; i++) {
      if (notes[i] == ' ') {
        delay(beats[i] * tempo2); // rest
      } else {
        playNote_2(notes[i], beats[i] * tempo2);
      }

      // pause between notes
      delay(tempo2 / 2);
    }
    //-----------------------------------------

    int length2 = 5; // the number of notes
    char notes2[] = "c  C"; // a space represents a rest
    int beats2[] = {6, 6, 6, 6};
    while (true) {
      for (int i = 0; i < length2; i++) {
        if (notes2[i] == ' ') {
          delay(beats2[i] * tempo2); // rest
        } else {
          playNote_2(notes2[i], beats2[i] * tempo2);
        }

        // pause between notes
        delay(tempo2 / 2);
      }
    }


  }

}


void playTone_2(int tone, int duration) {
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(buzzer2_pin, HIGH);
    delayMicroseconds(tone);
    digitalWrite(buzzer2_pin, LOW);
    delayMicroseconds(tone);
  }
}

void playNote_2(char note, int duration) {
  char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
  //int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 };
  int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 };

  // play the tone corresponding to the note name
  for (int i = 0; i < 8; i++) {
    if (names[i] == note) {
      playTone_2(tones[i], duration);
    }
  }
}


*/
