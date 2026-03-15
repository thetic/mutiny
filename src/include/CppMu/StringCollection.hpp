#ifndef INCLUDED_CPPMU_STRINGCOLLECTION_HPP
#define INCLUDED_CPPMU_STRINGCOLLECTION_HPP

#include "CppMu/String.hpp"

namespace cppmu {

class StringCollection
{
public:
  StringCollection(String const& str, char delimiter);
  StringCollection() = default;
  ~StringCollection();

  void split_string(String const& str, char delimiter);
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

#endif // INCLUDED_CPPMU_STRINGCOLLECTION_HPP
