// Copyright 2021 Charles Julian Knight
#ifndef LIB_MIDIMANAGER_MIDIMANAGER_H_
#define LIB_MIDIMANAGER_MIDIMANAGER_H_

#include "./types.h"
#include "./staticlinkedlist.h"

typedef uint8_t Channel;   // 0..<16
typedef uint8_t Note;      // 0..<128
typedef uint8_t Velocity;  // 0..<128

struct MidiEvent {
  // channel channel;
  Note note;
  Velocity velocity;

  bool matches(const MidiEvent& other) {
    return other.note == note /* && other.channel == channel */;
  }

  bool isOn() {
    return velocity > 0;
  }
};

// A MidiManager tracks the state of pressed keys through
// MIDI messages, and assigns keys to a given number of voices.
// New keys will "steal" voices from earlier keys if no more
// voices are available, but pressed keys are remembered such
// that releasing a key assigned to a voice will assign that
// voice to the most-recently-pressed waiting key. It tracks
// changes in velocity for different keys.
// VSize is the number of voices supported, while MSize is
// the maximum number of pressed keys to track. Reducing MSize
// will reduce the memory footprint, but if MSize is exceeded
// then the oldest pressed keys will be forgotten.
// Unused voices are assigned in order (lowest to highest),
// if all voices are in use the oldest voice is stolen for
// new keys.
template <size_t MSize, size_t VSize> class MidiManager {
 private:
  typedef size_t Voice;
  Channel channel_;  // TODO(cjk): figure out multi-channel support
  MidiEvent voices_[VSize];
  StaticLinkedList<Voice, VSize> unassigned_voices_;
  StaticLinkedList<Voice, VSize> assigned_voices_;
  StaticLinkedList<MidiEvent, MSize> pending_notes_;

 public:
  MidiManager() {
    channel_ = 0;
    for (Voice v = 0; v < VSize; v++) {
      voices_[v].velocity = 0;
      voices_[v].note = 0;
      unassigned_voices_.pushQueue(v);
    }
  }

  void handle(Channel c, Note n, Velocity v) {
    if (c != channel_) {
      return;  // ignore events for other channels
    }
    MidiEvent e = { n, v };
    if (e.isOn()) {
      // update the velocity if it's already playing
      Iterator<Voice> i = assigned_voices_.iterator();
      while (i.hasNext()) {
        Voice& v = i.next();
        if (voices_[v].matches(e)) {
          voices_[v].velocity = e.velocity;
          return;
        }
      }
      // update the velocity if it's already tracked
      Iterator<MidiEvent> ni = pending_notes_.iterator();
      while (ni.hasNext()) {
        MidiEvent& n = ni.next();
        if (n.matches(e)) {
          n.velocity = e.velocity;
          return;
        }
      }
      // must be a new note
      Voice v;
      if (unassigned_voices_.isEmpty()) {
        v = assigned_voices_.pop();  // steal oldest voice
        // save that voice's current note for later
        pending_notes_.pushStack(voices_[v]);
      } else {
        v = unassigned_voices_.pop();
      }
      assigned_voices_.pushQueue(v);
      voices_[v] = e;
      return;
    }
    // else note off

    // if it's a queued note simply forget about it
    Iterator<MidiEvent> ni = pending_notes_.iterator();
    size_t i = 0;
    while (ni.hasNext()) {
      MidiEvent& n = ni.next();
      if (n.matches(e)) {
        n.velocity = 0;
        pending_notes_.removeAt(i);
        return;
      }
      i++;
    }

    Iterator<Voice> vi = assigned_voices_.iterator();
    i = 0;
    while (vi.hasNext()) {
      Voice v = vi.next();
      if (!voices_[v].matches(e)) {
        i++;
        continue;
      }
      // pressed note is now off
      assigned_voices_.removeAt(i);
      if (pending_notes_.isEmpty()) {
        voices_[v].velocity = 0;
        unassigned_voices_.pushQueue(v);
      } else {
        MidiEvent pending = pending_notes_.pop();
        voices_[v] = pending;
        assigned_voices_.pushQueue(v);
      }
      return;
    }
    // otherwise we got a note off for a note we weren't tracking.
    // this can happen if notes were pressed before we initialized,
    // or if we overran the max size of pending notes
  }

  MidiEvent get(Voice voice) {
    return voices_[voice];
  }
};

#endif  // LIB_MIDIMANAGER_MIDIMANAGER_H_
