// Copyright 2021 Charles Julian Knight
#include <midimanager.h>
#include <unity.h>

void test_static_linked_list_stack_push() {
  StaticLinkedList<size_t, 3> list;
  TEST_ASSERT_TRUE(list.isEmpty());
  for (size_t i = 1; i <= 10; i++) {
    list.pushStack(i);
    TEST_ASSERT_EQUAL(i, list.peek());
    TEST_ASSERT_FALSE(list.isEmpty());
    TEST_ASSERT_EQUAL(i, list.pop());
    list.pushStack(i);
    TEST_ASSERT_FALSE(list.isEmpty());
  }
  TEST_ASSERT_EQUAL(3, list.size());
  TEST_ASSERT_EQUAL(10, list.pop());
  TEST_ASSERT_EQUAL(9, list.pop());
  TEST_ASSERT_EQUAL(8, list.pop());
  TEST_ASSERT_TRUE(list.isEmpty());
}

void test_static_linked_list_queue_push() {
  StaticLinkedList<size_t, 3> list;
  TEST_ASSERT_TRUE(list.isEmpty());
  for (size_t i = 1; i <= 10; i++) {
    list.pushQueue(i);
    TEST_ASSERT_FALSE(list.isEmpty());
  }
  TEST_ASSERT_EQUAL(3, list.size());
  TEST_ASSERT_EQUAL(8, list.pop());
  TEST_ASSERT_EQUAL(9, list.pop());
  TEST_ASSERT_EQUAL(10, list.pop());
}

void test_static_linked_list_remove() {
  StaticLinkedList<size_t, 3> list;
  list.pushStack(1);
  list.pushStack(2);
  list.pushStack(3);
  TEST_ASSERT_EQUAL(3, list.size());

  list.removeAt(0);
  TEST_ASSERT_EQUAL(2, list.size());
  TEST_ASSERT_EQUAL(2, list.peek());

  list.pushStack(3);
  list.removeAt(1);
  TEST_ASSERT_EQUAL(2, list.size());
  TEST_ASSERT_EQUAL(3, list.peek());
  TEST_ASSERT_EQUAL(3, list[0]);
  TEST_ASSERT_EQUAL(1, list[1]);

  list.removeAt(1);
  TEST_ASSERT_EQUAL(1, list.size());
  TEST_ASSERT_EQUAL(3, list.peek());
  TEST_ASSERT_EQUAL(3, list[0]);
  list.removeAt(0);
  TEST_ASSERT_EQUAL(0, list.size());
}

void test_static_linked_list_aggressive() {
  StaticLinkedList<size_t, 7> list;
  for (size_t i = 0; i < 255; i++) {
    if (i % 2 == 0) {
      list.pushStack(i);
    } else {
      list.pushQueue(i);
    }
    if (i % 3 == 0) {
      list.removeAt(i % list.size());
    }
  }
}

void test_static_linked_list_mutate_elements() {
  StaticLinkedList<size_t, 3> list;
  list.pushStack(1);
  list.pushStack(2);

  list[0] = 5;
  TEST_ASSERT_EQUAL(5, list.peek());
  TEST_ASSERT_EQUAL(5, list.pop());
  TEST_ASSERT_EQUAL(1, list.pop());
}

void test_ignored_channels() {
  MidiManager<16, 1> m;
  m.handle(1, 50, 127);
  MidiEvent s0 = m.get(0);
  TEST_ASSERT_EQUAL(0, s0.velocity);
}

void test_one_voice() {
  MidiManager<16, 1> m;
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

void test_one_voice_stack() {
  MidiManager<16, 1> m;
  m.handle(0, 50, 127);
  TEST_ASSERT_EQUAL(50, m.get(0).note);
  TEST_ASSERT_EQUAL(127, m.get(0).velocity);
  m.handle(0, 45, 96);
  TEST_ASSERT_EQUAL(45, m.get(0).note);
  TEST_ASSERT_EQUAL(96, m.get(0).velocity);
  m.handle(0, 50, 63);
  TEST_ASSERT_EQUAL(45, m.get(0).note);
  TEST_ASSERT_EQUAL(96, m.get(0).velocity);
  m.handle(0, 45, 0);
  TEST_ASSERT_EQUAL(50, m.get(0).note);
  TEST_ASSERT_EQUAL(63, m.get(0).velocity);
  m.handle(0, 45, 96);
  TEST_ASSERT_EQUAL(45, m.get(0).note);
  TEST_ASSERT_EQUAL(96, m.get(0).velocity);
  m.handle(0, 50, 0);
  TEST_ASSERT_EQUAL(45, m.get(0).note);
  TEST_ASSERT_EQUAL(96, m.get(0).velocity);
  m.handle(0, 45, 0);
  TEST_ASSERT_EQUAL(45, m.get(0).note);
  TEST_ASSERT_EQUAL(0, m.get(0).velocity);
}

void test_multiple_voices() {
  MidiManager<4, 3> m;
  TEST_ASSERT_EQUAL(0, m.get(0).note);
  TEST_ASSERT_EQUAL(0, m.get(0).velocity);
  TEST_ASSERT_EQUAL(0, m.get(1).note);
  TEST_ASSERT_EQUAL(0, m.get(1).velocity);
  TEST_ASSERT_EQUAL(0, m.get(2).note);
  TEST_ASSERT_EQUAL(0, m.get(2).velocity);
  m.handle(0, 50, 127);
  TEST_ASSERT_EQUAL(50, m.get(0).note);
  TEST_ASSERT_EQUAL(127, m.get(0).velocity);
  TEST_ASSERT_EQUAL(0, m.get(1).note);
  TEST_ASSERT_EQUAL(0, m.get(1).velocity);
  TEST_ASSERT_EQUAL(0, m.get(2).note);
  TEST_ASSERT_EQUAL(0, m.get(2).velocity);
  m.handle(0, 51, 127);
  TEST_ASSERT_EQUAL(50, m.get(0).note);
  TEST_ASSERT_EQUAL(127, m.get(0).velocity);
  TEST_ASSERT_EQUAL(51, m.get(1).note);
  TEST_ASSERT_EQUAL(127, m.get(1).velocity);
  TEST_ASSERT_EQUAL(0, m.get(2).note);
  TEST_ASSERT_EQUAL(0, m.get(2).velocity);
  m.handle(0, 52, 127);
  TEST_ASSERT_EQUAL(50, m.get(0).note);
  TEST_ASSERT_EQUAL(127, m.get(0).velocity);
  TEST_ASSERT_EQUAL(51, m.get(1).note);
  TEST_ASSERT_EQUAL(127, m.get(1).velocity);
  TEST_ASSERT_EQUAL(52, m.get(2).note);
  TEST_ASSERT_EQUAL(127, m.get(2).velocity);
  m.handle(0, 53, 127);  // steal oldest note
  TEST_ASSERT_EQUAL(53, m.get(0).note);
  TEST_ASSERT_EQUAL(127, m.get(0).velocity);
  TEST_ASSERT_EQUAL(51, m.get(1).note);
  TEST_ASSERT_EQUAL(127, m.get(1).velocity);
  TEST_ASSERT_EQUAL(52, m.get(2).note);
  TEST_ASSERT_EQUAL(127, m.get(2).velocity);
  m.handle(0, 54, 127);  // steal oldest note
  TEST_ASSERT_EQUAL(53, m.get(0).note);
  TEST_ASSERT_EQUAL(127, m.get(0).velocity);
  TEST_ASSERT_EQUAL(54, m.get(1).note);
  TEST_ASSERT_EQUAL(127, m.get(1).velocity);
  TEST_ASSERT_EQUAL(52, m.get(2).note);
  TEST_ASSERT_EQUAL(127, m.get(2).velocity);
  m.handle(0, 50, 0);  // let go of pending note
  TEST_ASSERT_EQUAL(53, m.get(0).note);
  TEST_ASSERT_EQUAL(127, m.get(0).velocity);
  TEST_ASSERT_EQUAL(54, m.get(1).note);
  TEST_ASSERT_EQUAL(127, m.get(1).velocity);
  TEST_ASSERT_EQUAL(52, m.get(2).note);
  TEST_ASSERT_EQUAL(127, m.get(2).velocity);
  m.handle(0, 54, 0);  // let go of recent note
  TEST_ASSERT_EQUAL(53, m.get(0).note);
  TEST_ASSERT_EQUAL(127, m.get(0).velocity);
  TEST_ASSERT_EQUAL(51, m.get(1).note);
  TEST_ASSERT_EQUAL(127, m.get(1).velocity);
  TEST_ASSERT_EQUAL(52, m.get(2).note);
  TEST_ASSERT_EQUAL(127, m.get(2).velocity);
  m.handle(0, 55, 127);  // steal oldest voice
  TEST_ASSERT_EQUAL(53, m.get(0).note);
  TEST_ASSERT_EQUAL(127, m.get(0).velocity);
  TEST_ASSERT_EQUAL(51, m.get(1).note);
  TEST_ASSERT_EQUAL(127, m.get(1).velocity);
  TEST_ASSERT_EQUAL(55, m.get(2).note);
  TEST_ASSERT_EQUAL(127, m.get(2).velocity);
  m.handle(0, 53, 0);
  TEST_ASSERT_EQUAL(52, m.get(0).note);
  TEST_ASSERT_EQUAL(127, m.get(0).velocity);
  TEST_ASSERT_EQUAL(51, m.get(1).note);
  TEST_ASSERT_EQUAL(127, m.get(1).velocity);
  TEST_ASSERT_EQUAL(55, m.get(2).note);
  TEST_ASSERT_EQUAL(127, m.get(2).velocity);
  m.handle(0, 51, 0);
  TEST_ASSERT_EQUAL(52, m.get(0).note);
  TEST_ASSERT_EQUAL(127, m.get(0).velocity);
  TEST_ASSERT_EQUAL(51, m.get(1).note);
  TEST_ASSERT_EQUAL(0, m.get(1).velocity);
  TEST_ASSERT_EQUAL(55, m.get(2).note);
  TEST_ASSERT_EQUAL(127, m.get(2).velocity);
  m.handle(0, 55, 0);
  TEST_ASSERT_EQUAL(52, m.get(0).note);
  TEST_ASSERT_EQUAL(127, m.get(0).velocity);
  TEST_ASSERT_EQUAL(51, m.get(1).note);
  TEST_ASSERT_EQUAL(0, m.get(1).velocity);
  TEST_ASSERT_EQUAL(55, m.get(2).note);
  TEST_ASSERT_EQUAL(0, m.get(2).velocity);
  m.handle(0, 52, 0);
  TEST_ASSERT_EQUAL(52, m.get(0).note);
  TEST_ASSERT_EQUAL(0, m.get(0).velocity);
  TEST_ASSERT_EQUAL(51, m.get(1).note);
  TEST_ASSERT_EQUAL(0, m.get(1).velocity);
  TEST_ASSERT_EQUAL(55, m.get(2).note);
  TEST_ASSERT_EQUAL(0, m.get(2).velocity);
  m.handle(0, 56, 63);
  TEST_ASSERT_EQUAL(52, m.get(0).note);
  TEST_ASSERT_EQUAL(0, m.get(0).velocity);
  TEST_ASSERT_EQUAL(56, m.get(1).note);
  TEST_ASSERT_EQUAL(63, m.get(1).velocity);
  TEST_ASSERT_EQUAL(55, m.get(2).note);
  TEST_ASSERT_EQUAL(0, m.get(2).velocity);
  m.handle(0, 57, 63);
  TEST_ASSERT_EQUAL(52, m.get(0).note);
  TEST_ASSERT_EQUAL(0, m.get(0).velocity);
  TEST_ASSERT_EQUAL(56, m.get(1).note);
  TEST_ASSERT_EQUAL(63, m.get(1).velocity);
  TEST_ASSERT_EQUAL(57, m.get(2).note);
  TEST_ASSERT_EQUAL(63, m.get(2).velocity);
  m.handle(0, 58, 63);
  TEST_ASSERT_EQUAL(58, m.get(0).note);
  TEST_ASSERT_EQUAL(63, m.get(0).velocity);
  TEST_ASSERT_EQUAL(56, m.get(1).note);
  TEST_ASSERT_EQUAL(63, m.get(1).velocity);
  TEST_ASSERT_EQUAL(57, m.get(2).note);
  TEST_ASSERT_EQUAL(63, m.get(2).velocity);
}

void process() {
  UNITY_BEGIN();
  RUN_TEST(test_static_linked_list_stack_push);
  RUN_TEST(test_static_linked_list_queue_push);
  RUN_TEST(test_static_linked_list_remove);
  RUN_TEST(test_static_linked_list_mutate_elements);
  RUN_TEST(test_static_linked_list_aggressive);
  RUN_TEST(test_ignored_channels);
  RUN_TEST(test_one_voice);
  RUN_TEST(test_one_voice_stack);
  RUN_TEST(test_multiple_voices);
  UNITY_END();
}

#ifdef ARDUINO

#include <Arduino.h>
void setup() {
  digitalWrite(13, HIGH);
  delay(2000);
  process();
  digitalWrite(13, LOW);
}

void loop() {
  digitalWrite(13, HIGH);
  delay(100);
  digitalWrite(13, LOW);
  delay(500);
}

#else  // ARDUINO

int main(int argc, char **argv) {
  process();
  return 0;
}

#endif  // ARDUINO
