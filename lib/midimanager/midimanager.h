#ifndef MIDIMANAGER_H
#define MIDIMANAGER_H

#include <assert.h>

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

template <typename T, size_t TSize> class StaticLinkedList
{
public:
  typedef T ValueType;
  typedef Node<T> NodeType;

  NodeType nodes_[TSize];
  NodeType* head_; // the first element
  NodeType* tail_; // the last element
  NodeType* cur_; // the most resent element with data

  StaticLinkedList()
  {
    head_ = nodes_;
    tail_ = nodes_ + TSize - 1;
    cur_ = nullptr;
    for (size_t i = 0; i < TSize-1; i++) {
      nodes_[i].next = nodes_ + i + 1;
    }
    tail_->next = nullptr;
  }

  ~StaticLinkedList() {}

  bool isEmpty()
  {
    return cur_ == nullptr;
  }

  void clear()
  {
    cur_ = nullptr;
  }

  NodeType* push(const ValueType item)
  {
    if (cur_ == nullptr) {
      head_->data = item;
      cur_ = head_;
      return cur_;
    }
    if (cur_->next == nullptr) {
      // put head at the end
      NodeType* tmp = head_;
      head_ = tmp->next;
      tail_->next = tmp;
      tmp->next = nullptr;
      tail_ = tmp;
    }
    cur_ = cur_->next;
    cur_->data = item;
    return cur_;
  }

  void remove(NodeType* item)
  {
    // TODO: a bit gross, as we are trusting that we're provided a legal pointer
    // TODO: we end up looping through the list twice here, as the caller also looped
    if (isEmpty()) {
      return; // user error, can't remove if empty
    }
    NodeType* prev = nullptr;
    NodeType* next = head_;
    while (next != item) {
      if (next == cur_) {
        // user error, you gave us a pointer outside the filled space
        return;
      }
      if (next == nullptr) {
        // user error, you gave us a pointer which doesn't actually exist in the queue
        return;
      }
      prev = next;
      next = prev->next;
    }
    // now next==item and prev is the one before (could be nullptr if head_)
    // next could also be tail_
    if (prev == nullptr) {
      // remove first element
      head_ = next->next;
      tail_->next = next;
      tail_ = next;
      next->next = nullptr;
    } else if (next == cur_) {
      // remove last element
      // this is the base case for linked lists, where we just pop cur_ back one
    } else {
      // remove from inside the used space
      prev->next = next->next;
      tail_->next = next;
      tail_ = next;
      next->next = nullptr;
    }
    if (next == cur_) {
      cur_ = prev; // this will be nullptr if we're at head_, which is expected
    }
  }
};

struct MidiEvent
{
  note note;
  velocity velocity;

  bool matches(const MidiEvent& other)
  {
    return other.note == note;
  }

  bool isOn()
  {
    return velocity > 0;
  }
};

const MidiEvent EMPTY = {0,0};

template <size_t QSize, size_t VSize> class MidiManager
{
private:
  StaticLinkedList<MidiEvent, QSize> queue_;
  Node<MidiEvent>* voices_[VSize];
  uint8_t channel_;
  size_t availableVoices_;

public:
  MidiManager(uint8_t channel)
  {
    channel_ = channel;
    availableVoices_ = VSize;
  }

  void handle(channel c, note n, velocity v)
  {
    // TODO: can a single one manage multiple channels? should be a matter of checking channel+note
    if (c != channel_) {
      return; // ignore events for other channels
    }
    MidiEvent e = { .note = n, .velocity = v };
    Node<MidiEvent>* cur = queue_.head_;
    if (!e.isOn()) {
      // note off

      if (queue_.isEmpty()) {
        // strange, why did we get a note off event for a note we didn't have?
        return;
      }
      
      do {
        if (cur->data.matches(e)) {
          // we got a match, so we want to either replace this voice
          // with the next in the queue, or turn this voice off if
          // the queue is empty
          

          return;
        }
        cur = cur->next;
      } while (cur != queue_.tail_);

    // otherwise we let go of a note that's in the queue, so
    // remove that note from the queue

      return;
    }

    // note on
    if (!queue_.isEmpty()) {
      do {
        if (cur->data.matches(e)) {
          // update velocity
          cur->data.velocity = v;
          return;
        }
        cur = cur->next;
      } while (cur != queue_.tail_);
    }
    // didn't already have the note, so add it
    cur = queue_.push(e);
    if (availableVoices_ > 0) {
      // we found an available voice
      voices_[VSize - availableVoices_] = cur;
      availableVoices_--;
    } else {
      // we've exhausted the available voices, so steal the oldest voice
      // and add that one to the queue
      // TODO: not always the first voice, round-robin the note to steal
      voices_[0] = cur;
    }
  }

  MidiEvent get(size_t voice)
  {
    if (voice > VSize) {
      return EMPTY;
    }
    if (voices_[voice] == nullptr) {
      return EMPTY;
    }
    return voices_[voice]->data;
  }
};

#endif // MIDIMANAGER_H
