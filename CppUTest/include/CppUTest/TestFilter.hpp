#ifndef TESTFILTER_H_
#define TESTFILTER_H_

#include "CppUTest/String.hpp"

namespace cpputest {

class TestFilter
{
public:
  TestFilter();
  TestFilter(const char* filter);
  TestFilter(const String& filter);

  TestFilter* add(TestFilter* filter);
  TestFilter* getNext() const;

  bool match(const String& name) const;

  void strictMatching();
  void invertMatching();

  bool operator==(const TestFilter& filter) const;
  bool operator!=(const TestFilter& filter) const;

  String asString() const;

private:
  String filter_;
  bool strictMatching_;
  bool invertMatching_;
  TestFilter* next_;
};

String
StringFrom(const TestFilter& filter);

} // namespace cpputest

#endif
