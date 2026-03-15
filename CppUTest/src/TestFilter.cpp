#include "CppUTest/TestFilter.hpp"

namespace cpputest {

TestFilter::TestFilter()
  : strict_matching_(false)
  , invert_matching_(false)
  , next_(nullptr)
{
}

TestFilter::TestFilter(const String& filter)
  : strict_matching_(false)
  , invert_matching_(false)
  , next_(nullptr)
{
  filter_ = filter;
}

TestFilter::TestFilter(const char* filter)
  : strict_matching_(false)
  , invert_matching_(false)
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
TestFilter::get_next() const
{
  return next_;
}

void
TestFilter::strict_matching()
{
  strict_matching_ = true;
}

void
TestFilter::invert_matching()
{
  invert_matching_ = true;
}

bool
TestFilter::match(const String& name) const
{
  bool matches = false;

  if (strict_matching_)
    matches = name == filter_;
  else
    matches = string_contains(name, filter_);

  return invert_matching_ ? !matches : matches;
}

bool
TestFilter::operator==(const TestFilter& filter) const
{
  return (filter_ == filter.filter_ &&
          strict_matching_ == filter.strict_matching_ &&
          invert_matching_ == filter.invert_matching_);
}

bool
TestFilter::operator!=(const TestFilter& filter) const
{
  return !(filter == *this);
}

String
TestFilter::as_string() const
{
  String text_filter =
      string_from_format("TestFilter: \"%s\"", filter_.c_str());
  if (strict_matching_ && invert_matching_)
    text_filter += " with strict, invert matching";
  else if (strict_matching_)
    text_filter += " with strict matching";
  else if (invert_matching_)
    text_filter += " with invert matching";

  return text_filter;
}

String
string_from(const TestFilter& filter)
{
  return filter.as_string();
}

} // namespace cpputest
