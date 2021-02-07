#include <midimanager.h>
#include <stdio.h>
#include <assert.h>

// MidiManager<16, 4> manager = MidiManager<16, 4>(0);

void doMain() {
  StaticLinkedList<size_t, 3> list;
  for (size_t i = 1; i <= 10; i++) {
    list.pushQueue(i);
  }
  while (!list.isEmpty()) {
    printf("%lu\n", *list.pop());
  }

  // MidiManager<16, 1> m = MidiManager<16, 1>(0);
  // m.handle(0, 50, 127);
  // printf("%d %d\n", m.get(0).note, m.get(0).velocity);
  // m.handle(0, 45, 96);
  // printf("%d %d\n", m.get(0).note, m.get(0).velocity);
  // m.handle(0, 50, 63);
  // printf("%d %d\n", m.get(0).note, m.get(0).velocity);
  // m.handle(0, 45, 0);
  // printf("%d %d\n", m.get(0).note, m.get(0).velocity);
  // m.handle(0, 45, 96);
  // printf("%d %d\n", m.get(0).note, m.get(0).velocity);
  // m.handle(0, 50, 0);
  // printf("%d %d\n", m.get(0).note, m.get(0).velocity);
  // m.handle(0, 45, 0);
}

#ifdef ARDUINO
#include <Arduino.h>
void setup() {
  doMain();
}

void loop() {}

#else // ARDUINO

int main(int argc, char **argv) {
    doMain();
    return 0;
}

#endif // ARDUINO
