#ifndef INCLUDED_MUTINY_TESTFILTER_HPP
#define INCLUDED_MUTINY_TESTFILTER_HPP

#include "mutiny/test/String.hpp"

namespace mu {
namespace tiny {
namespace test {

class TestFilter
{
public:
  TestFilter();
  TestFilter(const char* filter);
  TestFilter(const String& filter);

  TestFilter* add(TestFilter* filter);
  TestFilter* get_next() const;

  bool match(const String& name) const;

  void strict_matching();
  void invert_matching();

  bool operator==(const TestFilter& filter) const;
  bool operator!=(const TestFilter& filter) const;

  String as_string() const;

private:
  String filter_;
  bool strict_matching_;
  bool invert_matching_;
  TestFilter* next_;
};

String string_from(const TestFilter& filter);

}
}
} // namespace mu::tiny::test

#endif
