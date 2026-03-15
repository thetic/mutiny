#include "CppUTest/Extensions/IEEE754ExceptionsPlugin.hpp"
#include "CppUTest/Extensions/MockSupportPlugin.hpp"

#include "CppUTest/CommandLineTestRunner.hpp"
#include "CppUTest/TestPlugin.hpp"
#include "CppUTest/TestRegistry.hpp"

class MyDummyComparator : public cpputest::extensions::MockNamedValueComparator
{
public:
  virtual bool is_equal(const void* object1, const void* object2) override
  {
    return object1 == object2;
  }

  virtual cpputest::String value_to_string(const void* object) override
  {
    return cpputest::string_from(object);
  }
};

int
main(int argc, char** argv)
{
  MyDummyComparator dummy_comparator;
  cpputest::extensions::MockSupportPlugin mock_plugin;
  cpputest::extensions::IEEE754ExceptionsPlugin ieee754_plugin;

  mock_plugin.install_comparator("MyDummyType", dummy_comparator);
  cpputest::TestRegistry::get_current_registry()->install_plugin(&mock_plugin);
  cpputest::TestRegistry::get_current_registry()->install_plugin(
      &ieee754_plugin);
  return cpputest::CommandLineTestRunner::run_all_tests(argc, argv);
}
