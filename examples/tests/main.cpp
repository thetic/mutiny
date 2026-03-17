#include "IEEE754ExceptionsPlugin.hpp"

#include "CppMu/CommandLineTestRunner.hpp"
#include "CppMu/MockSupportPlugin.hpp"
#include "CppMu/TestPlugin.hpp"
#include "CppMu/TestRegistry.hpp"

class MyDummyComparator : public cppmu::MockNamedValueComparator
{
public:
  bool is_equal(const void* object1, const void* object2) override
  {
    return object1 == object2;
  }

  cppmu::String value_to_string(const void* object) override
  {
    return cppmu::string_from(object);
  }
};

int main(int argc, char** argv)
{
  MyDummyComparator dummy_comparator;
  cppmu::MockSupportPlugin mock_plugin;
  IEEE754ExceptionsPlugin ieee754_plugin;

  mock_plugin.install_comparator("MyDummyType", dummy_comparator);
  cppmu::TestRegistry::get_current_registry()->install_plugin(&mock_plugin);
  cppmu::TestRegistry::get_current_registry()->install_plugin(&ieee754_plugin);
  return cppmu::CommandLineTestRunner::run_all_tests(argc, argv);
}
