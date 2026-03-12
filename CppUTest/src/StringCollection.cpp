#include "CppUTest/StringCollection.hpp"

namespace cpputest {
namespace {
size_t
countSubstr(const String& string, const String& substr)
{
  size_t num = 0;
  const char* str = string.c_str();
  const char* strpart = nullptr;
  if (*str) {
    strpart = StrStr(str, substr.c_str());
  }
  while (*str && strpart) {
    str = strpart;
    str++;
    num++;
    strpart = StrStr(str, substr.c_str());
  }
  return num;
}

}

StringCollection::StringCollection(String const& str, char delimiter)
{
  splitString(str, delimiter);
}

StringCollection::~StringCollection()
{
  delete[] collection_;
}

void
StringCollection::splitString(String const& string, char d)
{
  auto delimiter = StringFrom(d);

  size_t num = countSubstr(string, delimiter);
  size_t extraEndToken = (stringEndsWith(string, delimiter)) ? 0 : 1U;
  allocate(num + extraEndToken);

  const char* str = string.c_str();
  const char* prev;
  for (size_t i = 0; i < num; ++i) {
    prev = str;
    str = StrStr(str, delimiter.c_str()) + 1;
    (*this)[i] = String(prev).substr(0, size_t(str - prev));
  }
  if (extraEndToken) {
    (*this)[num] = str;
  }
}

void
StringCollection::allocate(size_t _size)
{
  delete[] collection_;

  size_ = _size;
  collection_ = new String[size_];
}

size_t
StringCollection::size() const
{
  return size_;
}

String&
StringCollection::operator[](size_t index)
{
  if (index >= size_) {
    empty_ = "";
    return empty_;
  }

  return collection_[index];
}

}
