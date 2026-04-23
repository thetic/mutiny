#ifndef INCLUDED_CIRCULARBUFFER_HPP
#define INCLUDED_CIRCULARBUFFER_HPP

#include <stddef.h>

class Printer;

class CircularBuffer
{
public:
  explicit CircularBuffer(size_t capacity = default_capacity);
  virtual ~CircularBuffer();

  CircularBuffer(const CircularBuffer&) = delete;
  CircularBuffer& operator=(const CircularBuffer&) = delete;

  void put(int i);
  int get();
  bool is_empty() const;
  bool is_full() const;
  size_t capacity() const;
  size_t next(size_t i) const;
  void print(Printer* p);

private:
  size_t index_{ 0 };
  size_t outdex_{ 0 };
  int* buffer_;
  size_t capacity_;
  static constexpr size_t default_capacity = 5;
  bool empty_{ true };
  bool full_{ false };
};

#endif // INCLUDED_CIRCULARBUFFER_HPP
