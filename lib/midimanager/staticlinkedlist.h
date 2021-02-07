#ifndef STATICLINKEDLIST_H
#define STATICLINKEDLIST_H

#include <assert.h>
#include "types.h"

#define SLL_ASSERT_SANITY_CHECKS() \
    assert((size_==0) == (cur_==nullptr)); \
    assert(head_->up == nullptr); \
    assert(tail_->down == nullptr);

// StaticLinkedList is a linked list implementation which
// uses no dynamic allocations. It's max size is determined
// at compile time by TSize. If it's max size is exceeded, it
// will forget elements at the end of the list.
// It has 2 push implementations, allowing it to be used
// as a stack or as a queue (or a combination).
// It also has the ability to remove elements from arbitrary
// positions in the list, using a function pointer to decide
// which elements to remove. All operations are constant time
// except for the arbitrary removal, which is linear.
// This class is *not* thread-safe.
// TSize must to be at least 2. TODO: is this true? can we fix this?
template <typename T, size_t TSize> class StaticLinkedList
{
  struct Node
  {
    T data;
    Node* up; // down the stack
    Node* down; // up the stack
  };

private:
  Node nodes_[TSize];
  Node* head_; // the max slot
  Node* tail_; // the min slot
  Node* cur_; // the top of the stack, from tail
  size_t size_;

public:
  StaticLinkedList()
  {
    tail_ = nodes_;
    head_ = nodes_ + TSize - 1;
    cur_ = nullptr;
    size_ = 0;
    for (size_t i = 1; i < TSize-1; i++) {
      nodes_[i].up = nodes_ + i + 1;
      nodes_[i].down = nodes_ + i - 1;
    }
    tail_->down = nullptr;
    tail_->up = nodes_ + 1;
    head_->down = nodes_ + TSize - 2;
    head_->up = nullptr;
  }

  ~StaticLinkedList() {}

  bool isEmpty()
  {
    SLL_ASSERT_SANITY_CHECKS();
    return size_ == 0;
  }

  size_t size()
  {
    SLL_ASSERT_SANITY_CHECKS();
    return size_;
  }

  void clear()
  {
    size_ = 0;
    cur_ = nullptr;
    SLL_ASSERT_SANITY_CHECKS();
  }

  // pushStack adds an item to the top, i.e. the next item to be popped.
  // Use this if you wish to use the list as a stack. If the stack is full,
  // it will forget the oldest item in the stack. O(1).
  void pushStack(const T item)
  {
    if (cur_ == nullptr) {
      tail_->data = item;
      cur_ = tail_;
      size_ = 1;
      SLL_ASSERT_SANITY_CHECKS();
      return;
    }
    if (cur_->up == nullptr) {
      // we're full, so steal from the bottom
      freeToHead(tail_);
      size_--;
    }
    // now there's definitely space
    cur_ = cur_->up;
    cur_->data = item;
    size_++;
    SLL_ASSERT_SANITY_CHECKS();
    return;
  }

  // pushQueue adds the item to the bottom, i.e. the last
  // item to be popped. Use this if you which to use the list
  // as a queue. Steals from the top if the list is full. O(1).
  void pushQueue(const T item)
  {
    if (isEmpty()) {
      pushStack(item);
      return;
    }
    if (cur_ == head_) {
      size_--;
      cur_ = head_->down;
    }
    freeToTail(head_);
    tail_->data = item;
    size_++;
    SLL_ASSERT_SANITY_CHECKS();
    return;
  }

  // look at the next item without chaning the list.
  // returns null if the list is empty.
  T* peek() {
    if (isEmpty()) {
      return nullptr;
    }
    return &cur_->data;
  }

  T* pop()
  {
    if (isEmpty()) {
      return nullptr;
    }
    T* result = &cur_->data;
    cur_ = cur_->down; // this will be nullptr if we're at tail, which is expected
    size_--;
    SLL_ASSERT_SANITY_CHECKS();
    return result;
  }

  // removeIf removes elements from anywhere in the list selected by callback.
  // callback is given the value and the index of the element (from the top).
  // Returns the number if items removed. O(n)
  size_t removeIf(bool (*callback)(const T*, size_t))
  {
    size_t removed = 0;
    Node* item = cur_;
    size_t i = 0;
    while (item != nullptr)
    {
      if (callback(item->data, i)) {
        if (item == cur_) {
          pop(); // easy case
        } else {
          freeToHead(item);
          size_--;
        }
        removed++;
      }
      item = item->down;
      i++;
    }
    SLL_ASSERT_SANITY_CHECKS();
    return removed;
  }

private:

  void freeToTail(Node* item)
  {
    if (item->up == nullptr) {
      head_ = head_->down;
      head_->up = nullptr;
    } else {
      item->down->up = item->up;
    }
    tail_->down = item;
    item->up = tail_;
    item->down = nullptr;
    tail_ = item;
    SLL_ASSERT_SANITY_CHECKS();
  }

  void freeToHead(Node* item)
  {
    // remove this one from the chain
    if (item->down == nullptr) {
      tail_ = tail_->up;
      tail_->down = nullptr;
    } else {
      item->down->up = item->up;
    }
    // put this one at the head
    head_->up = item;
    item->down = head_;
    item->up = nullptr;
    head_ = item;
    SLL_ASSERT_SANITY_CHECKS();
  }
};

#endif // STATICLINKEDLIST_H
