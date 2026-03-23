#ifndef INCLUDED_MUTINY_TEST_FILTER_HPP
#define INCLUDED_MUTINY_TEST_FILTER_HPP

#include "mutiny/test/String.hpp"

namespace mu {
namespace tiny {
namespace test {

class Filter
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

String string_from(const Filter& filter);

}
}
} // namespace mu::tiny::test

#endif
