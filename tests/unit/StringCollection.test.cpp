#include "mutiny/StringCollection.hpp"

#include "mutiny/test.hpp"

TEST_GROUP(StringCollection) {};

TEST(StringCollection, CollectionReadOutOfBoundsReturnsEmptyString)
{
  mu::tiny::StringCollection col;
  col.allocate(3);
  STRCMP_EQUAL("", col[3].c_str());
}

TEST(StringCollection, CollectionWritingToEmptyString)
{
  mu::tiny::StringCollection col;
  col.allocate(3);
  col[3] = mu::tiny::String("HAH");
  STRCMP_EQUAL("", col[3].c_str());
}
TEST(StringCollection, split)
{
  mu::tiny::String hi("hello\nworld\nhow\ndo\nyou\ndo\n\n");

  mu::tiny::StringCollection collection(hi, '\n');

  CHECK_EQUAL(size_t{ 7 }, collection.size());
  STRCMP_EQUAL("hello\n", collection[0].c_str());
  STRCMP_EQUAL("world\n", collection[1].c_str());
  STRCMP_EQUAL("how\n", collection[2].c_str());
  STRCMP_EQUAL("do\n", collection[3].c_str());
  STRCMP_EQUAL("you\n", collection[4].c_str());
  STRCMP_EQUAL("do\n", collection[5].c_str());
  STRCMP_EQUAL("\n", collection[6].c_str());
}

TEST(StringCollection, splitNoTokenOnTheEnd)
{
  mu::tiny::String string("Bah Yah oops");
  mu::tiny::StringCollection collection(string, ' ');

  CHECK_EQUAL(size_t{ 3 }, collection.size());
  STRCMP_EQUAL("Bah ", collection[0].c_str());
  STRCMP_EQUAL("Yah ", collection[1].c_str());
  STRCMP_EQUAL("oops", collection[2].c_str());
}
