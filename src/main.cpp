// Copyright 2021 Charles Julian Knight
#include <midimanager.h>
#include <stdio.h>
#include <assert.h>

MidiManager<16, 2> m;

void doMain() {
  printf("size: %lu %lu %lu\n", sizeof(MidiManager<16, 2>), sizeof(size_t*), sizeof(size_t));
  m.handle(0, 50, 127);
  printf("%d, %d :: %d, %d\n", m.get(0).note, m.get(0).velocity, m.get(1).note, m.get(1).velocity);
  m.handle(0, 45, 96);
  printf("%d, %d :: %d, %d\n", m.get(0).note, m.get(0).velocity, m.get(1).note, m.get(1).velocity);
  m.handle(0, 50, 63);
  printf("%d, %d :: %d, %d\n", m.get(0).note, m.get(0).velocity, m.get(1).note, m.get(1).velocity);
  m.handle(0, 45, 0);
  printf("%d, %d :: %d, %d\n", m.get(0).note, m.get(0).velocity, m.get(1).note, m.get(1).velocity);
  m.handle(0, 45, 96);
  printf("%d, %d :: %d, %d\n", m.get(0).note, m.get(0).velocity, m.get(1).note, m.get(1).velocity);
  m.handle(0, 50, 0);
  printf("%d, %d :: %d, %d\n", m.get(0).note, m.get(0).velocity, m.get(1).note, m.get(1).velocity);
  m.handle(0, 45, 0);
  printf("%d, %d :: %d, %d\n", m.get(0).note, m.get(0).velocity, m.get(1).note, m.get(1).velocity);
}

#ifdef ARDUINO
#include <Arduino.h>
void setup() {
  doMain();
}
void loop() {}
#else  // ARDUINO
int main(int argc, char **argv) {
    doMain();
    return 0;
}
#endif  // ARDUINO
