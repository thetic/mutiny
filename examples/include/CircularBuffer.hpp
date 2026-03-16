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

  void put(int);
  int get();
  bool is_empty();
  bool is_full();
  int capacity();
  int next(int i);
  void print(Printer*);

private:
  int index_{ 0 };
  int outdex_{ 0 };
  int* buffer_;
  int capacity_;
  static constexpr int default_capacity = 5;
  bool empty_{ true };
  bool full_{ false };

  CircularBuffer(const CircularBuffer&);
  CircularBuffer& operator=(const CircularBuffer&);
};

#endif // INCLUDED_CIRCULARBUFFER_HPP
