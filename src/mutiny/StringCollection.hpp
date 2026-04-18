#ifndef INCLUDED_MUTINY_TEST_STRINGCOLLECTION_HPP
#define INCLUDED_MUTINY_TEST_STRINGCOLLECTION_HPP

#include "mutiny/String.hpp"

namespace mu {
namespace tiny {

class StringCollection
{
public:
  StringCollection(String const& str, char delimiter);
  StringCollection() = default;
  ~StringCollection();
  void operator=(StringCollection&) = delete;
  StringCollection(StringCollection&) = delete;

  void split_string(String const& str, char delimiter);
  void allocate(size_t size);

  size_t size() const;
  String& operator[](size_t index);

private:
  String* collection_{ nullptr };
  size_t size_{ 0 };
};

} // namespace tiny
} // namespace mu

#endif // INCLUDED_MUTINY_TEST_STRINGCOLLECTION_HPP
