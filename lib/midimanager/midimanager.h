#ifndef MIDIMANAGER_H
#define MIDIMANAGER_H

#include "types.h"
#include "staticlinkedlist.h"

typedef uint8_t channel; // 0..<16
typedef uint8_t note; // 0..<128
typedef uint8_t velocity; // 0..<128

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
  MidiEvent* voices_[VSize];
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
    // // TODO: can a single one manage multiple channels? should be a matter of checking channel+note
    // if (c != channel_) {
    //   return; // ignore events for other channels
    // }
    // MidiEvent e = { .note = n, .velocity = v };
    // Node<MidiEvent>* cur = queue_.head_;
    // if (!e.isOn()) {
    //   // note off

    //   if (queue_.isEmpty()) {
    //     // we got a note off event for a note we weren't tracking.
    //     // normally this might be considered unexpected, but it can happen
    //     // if notes were pressed before we initialized, or if we overran
    //     // our queue size in pressed notes
    //     return;
    //   }

    //   while (true) {
    //     if (cur->data.matches(e)) {
    //       // we got a match, so we want to either replace this voice
    //       // with the next in the queue, or turn this voice off if
    //       // the queue is empty

    //       size_t voice = -1;
    //       for (size_t i = 0; i < VSize; i++) {
    //         if (voices_[i] == cur) {
    //           // there's a voice pointing to this one
    //           voice = i;
    //           break;
    //         }
    //       }

    //       queue_.remove(cur); // TODO: this is gross because it invalidates a pointer it knows we have
    //       if (voice != -1) {
    //         // we need to point this voice to the next available voice in the queue
    //         if (queue_.isEmpty()) {
    //           // there weren't any pending notes, so leave the voice off
    //           // voices_[voice] = nullptr;
    //           voices_[voice]->data.velocity = 0;
    //           return;
    //         }
    //         cur = queue_.head_;
    //         while (true) {
    //           if (cur->next == queue_.cur_) {
    //             // there weren't any pending notes, so leave the voice off
    //             // voices_[voice] = nullptr;
    //             voices_[voice]->data.velocity = 0;
    //             return;
    //           }
    //           // TODO: these nested loops really suck
    //           for (size_t v = 0; v < VSize; v++) {
    //             if (cur->next == voices_[v]) {
    //               // this is the next one in the queue
    //               voices_[voice] = cur;
    //               return;
    //             }
    //           }
    //         }
    //       } // else we let go of a note that was queued, so we can ignore it

    //       return;
    //     }
    //     if (cur == queue_.cur_) {
    //       // we got a note off event for a note we weren't tracking.
    //       // normally this might be considered unexpected, but it can happen
    //       // if notes were pressed before we initialized, or if we overran
    //       // our queue size in pressed notes
    //       return;
    //     }
    //     cur = cur->next;
    //   }
    // }

    // // note on
    // if (!queue_.isEmpty()) {
    //   do {
    //     if (cur->data.matches(e)) {
    //       // update velocity
    //       cur->data.velocity = v;
    //       return;
    //     }
    //     cur = cur->next;
    //   } while (cur != queue_.tail_);
    // }
    // // didn't already have the note, so add it
    // cur = queue_.push(e);
    // if (availableVoices_ > 0) {
    //   // we found an available voice
    //   voices_[VSize - availableVoices_] = cur;
    //   availableVoices_--;
    // } else {
    //   // we've exhausted the available voices, so steal the oldest voice
    //   // and add that one to the queue
    //   // TODO: not always the first voice, round-robin the note to steal
    //   voices_[0] = cur;
    // }
  }

  MidiEvent get(size_t voice)
  {
    if (voice > VSize) {
      return EMPTY;
    }
    if (voices_[voice] == nullptr) {
      return EMPTY;
    }
    return *voices_[voice];
  }
};

#endif // MIDIMANAGER_H
