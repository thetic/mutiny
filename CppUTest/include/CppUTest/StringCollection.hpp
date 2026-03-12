#ifndef INCLUDED_CPPUTEST_STRINGCOLLECTION_HPP
#define INCLUDED_CPPUTEST_STRINGCOLLECTION_HPP

#include "CppUTest/String.hpp"

namespace cpputest {

class StringCollection
{
public:
  StringCollection(String const& str, char delimiter);
  StringCollection() = default;
  ~StringCollection();


  void splitString(String const& str, char delimiter);
  void allocate(size_t size);

  size_t size() const;
  String& operator[](size_t index);

private:
  String* collection_{ nullptr };
  String empty_;
  size_t size_{ 0 };

  void operator=(StringCollection&);
  StringCollection(StringCollection&);
};

}

#endif // INCLUDED_CPPUTEST_STRINGCOLLECTION_HPP
