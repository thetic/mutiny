#include "CircularBuffer.hpp"

#include "Printer.hpp"

#include <stddef.h>

CircularBuffer::CircularBuffer(int capacity)
  : capacity_(capacity)

{
  buffer_ = new int[static_cast<size_t>(this->capacity_)];
}

CircularBuffer::~CircularBuffer()
{
  delete[] buffer_;
}

bool CircularBuffer::is_empty()
{
  return empty_;
}

bool CircularBuffer::is_full()
{
  return full_;
}

void CircularBuffer::put(int i)
{
  empty_ = false;
  buffer_[index_] = i;
  index_ = next(index_);
  if (full_) {
    outdex_ = next(outdex_);
  } else if (index_ == outdex_) {
    full_ = true;
  }
}

int CircularBuffer::get()
{
  int result = -1;
  full_ = false;

  if (!empty_) {
    result = buffer_[outdex_];
    outdex_ = next(outdex_);
    if (outdex_ == index_) {
      empty_ = true;
    }
  }
  return result;
}

int CircularBuffer::capacity()
{
  return capacity_;
}

int CircularBuffer::next(int i)
{
  if (++i >= capacity_) {
    i = 0;
  }
  return i;
}

void CircularBuffer::print(Printer* p)
{
  p->print("Circular buffer content:\n<");

  int print_index = outdex_;
  int count = index_ - outdex_;

  if (!empty_ && (index_ <= outdex_)) {
    count = capacity_ - (outdex_ - index_);
  }

  for (int i = 0; i < count; i++) {
    p->print(buffer_[print_index]);
    print_index = next(print_index);
    if (i + 1 != count) {
      p->print(", ");
    }
  }
  p->print(">\n");
}
