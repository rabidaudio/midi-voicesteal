#ifndef MIDIMANAGER_H
#define MIDIMANAGER_H

#ifdef ARDUINO

#include <Arduino.h>

#else // ARDUINO

#include <stddef.h>
#include <stdint.h>

#endif // ARDUINO

template <typename T> struct Node
{
  T data;
  Node<T>* next;
};

typedef uint8_t channel; // 0..<16
typedef uint8_t note; // 0..<128
typedef uint8_t velocity; // 0..<128

template <typename T, size_t TSize> class FixedLinkedList
{
public:
  typedef T ValueType;
  typedef Node<T> NodeType;

  NodeType nodes_[TSize];
  NodeType* head_;
  NodeType* tail_;

  FixedLinkedList()
  {
    head_ = nodes_;
    tail_ = head_;
  }

  ~FixedLinkedList() {}

  void fill(const T item)
  {
    for (size_t i = 0; i < TSize; i++) {
      nodes_[i].data = item;
    }
  }

  void push(const T item)
  { 
    if (tail_->next == NULL) {
      // replace the first item
    } else {
      tail_->next->data = item;
      tail_ = tail_->next;
    }
  }

  NodeType* removeAt(size_t index)
  {
    NodeType* prev = NULL;
    NodeType* cur = head_;
    for (size_t i = 0; i < index+1; i++) {
      prev = cur;
      cur = prev->next;
    }
    if (prev == NULL) {
      head_ = cur->next;
    } else {
      prev->next = cur->next;
    }
    cur = tail_;
    while (cur->next != NULL) {
      cur = cur->next;
    }
    cur->next = prev;
    return prev;
  }
};

struct MidiEvent
{
  note note;
  velocity velocity;
};

template <size_t QSize, size_t VSize> class MidiManager
{
private:
  FixedLinkedList<MidiEvent, 16> queue_;
  Node<MidiEvent>* states_[VSize];
  uint8_t channel_;

public:
  MidiManager(uint8_t channel)
  {
    channel_ = channel;
    // zero out the structure
    MidiEvent empty = {channel_, 0, 0};
    queue_.fill(empty);
    for (size_t i = 0; i < VSize; i++) {
      states_[i] = queue_.head_ + i;
    }
  }

  void handle(channel c, note n, velocity v)
  {
    if (c != channel_) {
      return; // ignore events for other channels
    }
  }

  MidiEvent get(size_t voice)
  {
    return states_[voice]->data;
  }
};

#endif // MIDIMANAGER_H
