#include "CppUTest/StringCollection.hpp"

#include "CppUTest/TestHarness.hpp"

TEST_GROUP(StringCollection) {};

TEST(StringCollection, CollectionReadOutOfBoundsReturnsEmptyString)
{
  cpputest::StringCollection col;
  col.allocate(3);
  STRCMP_EQUAL("", col[3].c_str());
}

TEST(StringCollection, CollectionWritingToEmptyString)
{
  cpputest::StringCollection col;
  col.allocate(3);
  col[3] = cpputest::String("HAH");
  STRCMP_EQUAL("", col[3].c_str());
}
TEST(StringCollection, split)
{
  cpputest::String hi("hello\nworld\nhow\ndo\nyou\ndo\n\n");

  cpputest::StringCollection collection(hi, '\n');

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
  cpputest::String string("Bah Yah oops");
  cpputest::StringCollection collection(string, ' ');

  LONGS_EQUAL(3, collection.size());
  STRCMP_EQUAL("Bah ", collection[0].c_str());
  STRCMP_EQUAL("Yah ", collection[1].c_str());
  STRCMP_EQUAL("oops", collection[2].c_str());
}
