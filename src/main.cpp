#include <midimanager.h>
#include <stdio.h>
#include <assert.h>

void doMain() {
  StaticLinkedList<size_t, 3> list;
  list.pushStack(1);
  list.pushStack(2);
  list.pushStack(3);
  list.removeAt(0);
  list.pushStack(3);
  list.removeAt(1);
  printf("%lu %lu\n", list[0], list[1]);

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
