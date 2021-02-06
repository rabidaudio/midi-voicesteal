#include <midimanager.h>
#include <stdio.h>
#include <assert.h>

// MidiManager<16, 4> manager = MidiManager<16, 4>(0);

void doMain() {
  StaticLinkedList<size_t, 3> list;
  Node<size_t>* n;
  // Node<size_t>* n0 = list.push(1);
  // Node<size_t>* n1 = list.push(2);
  // Node<size_t>* n2 = list.push(3);
  // Node<size_t>* n3 = list.push(4);

  n = list.push(1);
  list.remove(n);
  list.clear();
  n = list.push(1);
  list.push(2);
  list.remove(n);

  list.clear();
  list.push(1);
  n = list.push(2);
  list.push(3);
  list.remove(n);
  printf("%i %lu %lu %lu", list.isEmpty(), list.cur_->data, list.head_->data, list.head_->next->data);

  // MidiManager<16, 1> m = MidiManager<16, 1>(0);
  // m.handle(0, 50, 127);
  // m.handle(0, 50, 63);
  // m.handle(0, 50, 0);
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
