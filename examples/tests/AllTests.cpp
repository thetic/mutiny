#include "CppUTest/Extensions/IEEE754ExceptionsPlugin.hpp"
#include "CppUTest/Extensions/MockSupportPlugin.hpp"

#include "CppUTest/CommandLineTestRunner.hpp"
#include "CppUTest/TestPlugin.hpp"
#include "CppUTest/TestRegistry.hpp"

class MyDummyComparator : public cpputest::extensions::MockNamedValueComparator
{
public:
  virtual bool isEqual(const void* object1, const void* object2) override
  {
    return object1 == object2;
  }

  virtual cpputest::String valueToString(const void* object) override
  {
    return cpputest::StringFrom(object);
  }
};

int
main(int ac, char** av)
{
  MyDummyComparator dummyComparator;
  cpputest::extensions::MockSupportPlugin mockPlugin;
  cpputest::extensions::IEEE754ExceptionsPlugin ieee754Plugin;

  mockPlugin.installComparator("MyDummyType", dummyComparator);
  cpputest::TestRegistry::getCurrentRegistry()->installPlugin(&mockPlugin);
  cpputest::TestRegistry::getCurrentRegistry()->installPlugin(&ieee754Plugin);
  return cpputest::CommandLineTestRunner::RunAllTests(ac, av);
}

#include "AllTests.h"
