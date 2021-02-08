// Copyright 2021 Charles Julian Knight
#include <midimanager.h>

MidiManager<16, 4> m;

#ifdef ARDUINO
#include <Arduino.h>
void setup() {
  Serial.begin(9600);
  Serial.println(sizeof(MidiManager<16, 4>), DEC);
}

void loop() {}
#else  // ARDUINO
#include <stdio.h>
#include <assert.h>
int main(int argc, char **argv) {
    return 0;
}
#endif  // ARDUINO
