#include "mu/tiny/StringView.hpp"

namespace mu {
namespace tiny {

StringView::StringView(const char* str)
  : data_(str)
  , size_(strlen(str))
{
}

StringView::StringView(const char* str, size_t len)
  : data_(str)
  , size_(len)
{
}

StringView::StringView(const String& str)
  : data_(str.c_str())
  , size_(str.size())
{
}

bool operator==(StringView left, StringView right)
{
  return left.size() == right.size() &&
         strncmp(left.data(), right.data(), left.size()) == 0;
}

bool operator!=(StringView left, StringView right)
{
  return !(left == right);
}

} // namespace tiny
} // namespace mu
