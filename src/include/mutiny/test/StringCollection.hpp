#ifndef INCLUDED_MUTINY_STRINGCOLLECTION_HPP
#define INCLUDED_MUTINY_STRINGCOLLECTION_HPP

#include "mutiny/test/String.hpp"

namespace mu {
namespace tiny {
namespace test {

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
  String empty_;
  size_t size_{ 0 };
};

}
}
} // namespace mu::tiny::test

#endif // INCLUDED_MUTINY_STRINGCOLLECTION_HPP
