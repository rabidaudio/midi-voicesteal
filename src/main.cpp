#include <midimanager.h>
#include <stdio.h>
#include <assert.h>

MidiManager<16, 4> manager = MidiManager<16, 4>(0);

void doMain() {
  manager.handle(1, 50, 127);
  MidiEvent s0 = manager.get(0);
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
