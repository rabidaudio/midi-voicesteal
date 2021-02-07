#include <midimanager.h>
#include <unity.h>


void test_static_linked_list_stack_push() {
  StaticLinkedList<size_t, 3> list;
  TEST_ASSERT_TRUE(list.isEmpty());
  for (size_t i = 1; i <= 10; i++) {
    list.pushStack(i);
    TEST_ASSERT_EQUAL(i, *list.peek());
    TEST_ASSERT_FALSE(list.isEmpty());
    TEST_ASSERT_EQUAL(i, *list.pop());
    list.pushStack(i);
    TEST_ASSERT_FALSE(list.isEmpty());
  }
  TEST_ASSERT_EQUAL(3, list.size());
  TEST_ASSERT_EQUAL(10, *list.pop());
  TEST_ASSERT_EQUAL(9, *list.pop());
  TEST_ASSERT_EQUAL(8, *list.pop());
  TEST_ASSERT_TRUE(list.isEmpty());
  TEST_ASSERT_NULL(list.pop());
}

void test_static_linked_list_queue_push() {
  StaticLinkedList<size_t, 3> list;
  TEST_ASSERT_TRUE(list.isEmpty());
  for (size_t i = 1; i <= 10; i++) {
    list.pushQueue(i);
    TEST_ASSERT_FALSE(list.isEmpty());
  }
  TEST_ASSERT_EQUAL(3, list.size());
  TEST_ASSERT_EQUAL(8, *list.pop());
  TEST_ASSERT_EQUAL(9, *list.pop());
  TEST_ASSERT_EQUAL(10, *list.pop());
}

void test_static_linked_list_remove() {
  StaticLinkedList<size_t, 3> list;
  list.pushStack(1);
  list.pushStack(2);
  list.pushStack(3);
  TEST_ASSERT_EQUAL(3, list.size());
  list.removeAt(0);
  TEST_ASSERT_EQUAL(2, list.size());
  TEST_ASSERT_EQUAL(2, *list.peek());

  list.pushStack(3);
  list.removeAt(1);
  TEST_ASSERT_EQUAL(2, list.size());
  TEST_ASSERT_EQUAL(3, *list.peek());
  TEST_ASSERT_EQUAL(3, list[0]);
  TEST_ASSERT_EQUAL(1, list[1]);
  // Node<size_t>* n;
  // // remove element when empty
  // list.remove(list.head_);
  // TEST_ASSERT_TRUE(list.isEmpty());
  // TEST_ASSERT_NULL(list.tail_->next);

  // // remove nonsense item
  // list.remove(nullptr);
  // TEST_ASSERT_TRUE(list.isEmpty());
  // TEST_ASSERT_NULL(list.tail_->next);

  // // remove element when one item
  // n = list.push(1);
  // list.remove(n);
  // TEST_ASSERT_TRUE(list.isEmpty());
  // TEST_ASSERT_NULL(list.tail_->next);

  // // remove first item when multiple items
  // list.clear();
  // n = list.push(1);
  // list.push(2);
  // list.remove(n);
  // TEST_ASSERT_FALSE(list.isEmpty());
  // TEST_ASSERT_EQUAL(2, list.cur_->data);
  // TEST_ASSERT_EQUAL(2, list.head_->data);
  // TEST_ASSERT_NULL(list.tail_->next);

  // // remove item from the middle
  // list.clear();
  // list.push(1);
  // n = list.push(2);
  // list.push(3);
  // list.remove(n);
  // TEST_ASSERT_FALSE(list.isEmpty());
  // TEST_ASSERT_EQUAL(3, list.cur_->data);
  // TEST_ASSERT_EQUAL(1, list.head_->data);
  // TEST_ASSERT_EQUAL(3, list.head_->next->data);
  // TEST_ASSERT_NULL(list.tail_->next);

  // // remove cursor with multiple items
  // list.clear();
  // list.push(1);
  // list.push(2);
  // n = list.push(3);
  // list.remove(n);
  // TEST_ASSERT_FALSE(list.isEmpty());
  // TEST_ASSERT_EQUAL(2, list.cur_->data);
  // TEST_ASSERT_EQUAL(1, list.head_->data);
  // TEST_ASSERT_EQUAL(2, list.head_->next->data);

  // // remove head when cursor is tail
  // list.clear();
  // n = list.push(1);
  // list.push(2);
  // list.push(3);
  // list.remove(n);
  // TEST_ASSERT_FALSE(list.isEmpty());
  // TEST_ASSERT_EQUAL(3, list.cur_->data);
  // TEST_ASSERT_EQUAL(2, list.head_->data);

  // // TODO: test a lot of ads and removes for segfaults 
}

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

void test_one_voice_stack() {
  MidiManager<16, 1> m = MidiManager<16, 1>(0);
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

void process() {
    UNITY_BEGIN();
    RUN_TEST(test_static_linked_list_stack_push);
    RUN_TEST(test_static_linked_list_queue_push);
    RUN_TEST(test_static_linked_list_remove);
    // RUN_TEST(test_ignored_channels);
    // RUN_TEST(test_one_voice);
    // RUN_TEST(test_one_voice_stack);
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
