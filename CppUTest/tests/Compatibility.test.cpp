
#include "CppUTest/TestHarness.hpp"

#if CPPUTEST_USE_STD_CPP_LIB

#include <memory>

TEST_GROUP(Compatibility)
{};

#if defined(__cplusplus) && __cplusplus >= 201402L

TEST(Compatibility, UniquePtrConversationToBool)
{
  auto const a_null = std::unique_ptr<int>(nullptr);
  CHECK_FALSE(a_null);

  auto const not_null = std::make_unique<int>(1);
  CHECK_TRUE(not_null);
}

#endif

#endif
