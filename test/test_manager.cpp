#include <midimanager.h>
#include <unity.h>

void test_ignored_channels() {
  MidiManager<16, 1> m = MidiManager<16, 1>(0);
  m.handle(1, 50, 127);
  MidiEvent s0 = m.get(0);
  TEST_ASSERT_EQUAL_MESSAGE(0, s0.velocity, "since the channels don't match, we expect it to be off");
}

void test_one_voice() {
  MidiManager<16, 1> m = MidiManager<16, 1>(0);
  // note on
  m.handle(0, 50, 127);
  TEST_ASSERT_EQUAL(50, m.get(0).note);
  TEST_ASSERT_EQUAL(127, m.get(0).velocity);
  // change velocity
  m.handle(0, 50, 63);
  TEST_ASSERT_EQUAL(50, m.get(0).note);
  TEST_ASSERT_EQUAL(63, m.get(0).velocity);
  // note off
  m.handle(0, 50, 0);
  TEST_ASSERT_EQUAL(50, m.get(0).note);
  TEST_ASSERT_EQUAL(0, m.get(0).velocity);
}

void process() {
    UNITY_BEGIN();
    RUN_TEST(test_ignored_channels);
    RUN_TEST(test_one_voice);
    UNITY_END();
}

#ifdef ARDUINO

#include <Arduino.h>
void setup() {
    // NOTE!!! Wait for >2 secs
    // if board doesn't support software reset via Serial.DTR/RTS
    delay(2000);

    process();
}

void loop() {
    digitalWrite(13, HIGH);
    delay(100);
    digitalWrite(13, LOW);
    delay(500);
}

#else // ARDUINO

int main(int argc, char **argv) {
    process();
    return 0;
}

#endif // ARDUINO
