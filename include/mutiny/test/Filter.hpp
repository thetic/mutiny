#ifndef INCLUDED_MUTINY_TEST_FILTER_HPP
#define INCLUDED_MUTINY_TEST_FILTER_HPP

#include "mutiny/String.hpp"
#include "mutiny/export.h"

namespace mu {
namespace tiny {
namespace test {

class MUTINY_EXPORT Filter
{
public:
  Filter();
  Filter(const char* filter);
  Filter(const String& filter);

  Filter* add(Filter* filter);
  Filter* get_next() const;

  bool match(const String& name) const;

  void strict_matching();
  void invert_matching();

  bool operator==(const Filter& filter) const;
  bool operator!=(const Filter& filter) const;

  String as_string() const;

private:
  String filter_;
  bool strict_matching_;
  bool invert_matching_;
  Filter* next_;
};

} // namespace test

MUTINY_EXPORT String string_from(const test::Filter& filter);

} // namespace tiny
} // namespace mu

#endif
