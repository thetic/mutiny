#include "CppUTest/TestFilter.hpp"

namespace cpputest {

TestFilter::TestFilter()
  : strictMatching_(false)
  , invertMatching_(false)
  , next_(nullptr)
{
}

TestFilter::TestFilter(const String& filter)
  : strictMatching_(false)
  , invertMatching_(false)
  , next_(nullptr)
{
  filter_ = filter;
}

TestFilter::TestFilter(const char* filter)
  : strictMatching_(false)
  , invertMatching_(false)
  , next_(nullptr)
{
  filter_ = filter;
}

TestFilter*
TestFilter::add(TestFilter* filter)
{
  next_ = filter;
  return this;
}

TestFilter*
TestFilter::getNext() const
{
  return next_;
}

void
TestFilter::strictMatching()
{
  strictMatching_ = true;
}

void
TestFilter::invertMatching()
{
  invertMatching_ = true;
}

bool
TestFilter::match(const String& name) const
{
  bool matches = false;

  if (strictMatching_)
    matches = name == filter_;
  else
    matches = name.contains(filter_);

  return invertMatching_ ? !matches : matches;
}

bool
TestFilter::operator==(const TestFilter& filter) const
{
  return (filter_ == filter.filter_ &&
          strictMatching_ == filter.strictMatching_ &&
          invertMatching_ == filter.invertMatching_);
}

bool
TestFilter::operator!=(const TestFilter& filter) const
{
  return !(filter == *this);
}

String
TestFilter::asString() const
{
  String textFilter = StringFromFormat("TestFilter: \"%s\"", filter_.c_str());
  if (strictMatching_ && invertMatching_)
    textFilter += " with strict, invert matching";
  else if (strictMatching_)
    textFilter += " with strict matching";
  else if (invertMatching_)
    textFilter += " with invert matching";

  return textFilter;
}

String
StringFrom(const TestFilter& filter)
{
  return filter.asString();
}

} // namespace cpputest
