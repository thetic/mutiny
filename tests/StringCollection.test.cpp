#include "mutiny/test/StringCollection.hpp"

#include "mutiny/test.hpp"

TEST_GROUP(StringCollection) {};

TEST(StringCollection, CollectionReadOutOfBoundsReturnsEmptyString)
{
  mu::tiny::test::StringCollection col;
  col.allocate(3);
  STRCMP_EQUAL("", col[3].c_str());
}

TEST(StringCollection, CollectionWritingToEmptyString)
{
  mu::tiny::test::StringCollection col;
  col.allocate(3);
  col[3] = mu::tiny::test::String("HAH");
  STRCMP_EQUAL("", col[3].c_str());
}
TEST(StringCollection, split)
{
  mu::tiny::test::String hi("hello\nworld\nhow\ndo\nyou\ndo\n\n");

  mu::tiny::test::StringCollection collection(hi, '\n');

  LONGS_EQUAL(7, collection.size());
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
  mu::tiny::test::String string("Bah Yah oops");
  mu::tiny::test::StringCollection collection(string, ' ');

  LONGS_EQUAL(3, collection.size());
  STRCMP_EQUAL("Bah ", collection[0].c_str());
  STRCMP_EQUAL("Yah ", collection[1].c_str());
  STRCMP_EQUAL("oops", collection[2].c_str());
}
