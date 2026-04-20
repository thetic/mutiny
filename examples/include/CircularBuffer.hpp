#ifndef INCLUDED_CIRCULARBUFFER_HPP
#define INCLUDED_CIRCULARBUFFER_HPP

///////////////////////////////////////////////////////////////////////////////
//
//  CircularBuffer.h
//
//  CircularBuffer is responsible for ...
//
///////////////////////////////////////////////////////////////////////////////
class Printer;

class CircularBuffer
{
public:
  explicit CircularBuffer(int capacity = default_capacity);
  virtual ~CircularBuffer();

  CircularBuffer(const CircularBuffer&) = delete;
  CircularBuffer& operator=(const CircularBuffer&) = delete;

  void put(int i);
  int get();
  bool is_empty() const;
  bool is_full() const;
  int capacity() const;
  int next(int i) const;
  void print(Printer* p);

private:
  int index_{ 0 };
  int outdex_{ 0 };
  int* buffer_;
  int capacity_;
  static constexpr int default_capacity = 5;
  bool empty_{ true };
  bool full_{ false };
};

#endif // INCLUDED_CIRCULARBUFFER_HPP
