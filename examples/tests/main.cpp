#include "IEEE754ExceptionsPlugin.hpp"
#include "TeamCityOutputPlugin.hpp"

#include "mutiny/mock/SupportPlugin.hpp"

#include "mutiny/test/CommandLineRunner.hpp"
#include "mutiny/test/Registry.hpp"

class MyDummyComparator : public mu::tiny::mock::NamedValueComparator
{
public:
  bool is_equal(const void* object1, const void* object2) override
  {
    return object1 == object2;
  }

  mu::tiny::String value_to_string(const void* object) override
  {
    return mu::tiny::string_from(object);
  }
};

int main(int argc, char** argv)
{
  mu::tiny::mock::SupportPlugin mock_plugin;
  MyDummyComparator dummy_comparator;
  mock_plugin.install_comparator("MyDummyType", dummy_comparator);
  mu::tiny::test::Registry::get_current_registry()->install_plugin(
      &mock_plugin
  );

  IEEE754ExceptionsPlugin ieee754_plugin;
  mu::tiny::test::Registry::get_current_registry()->install_plugin(
      &ieee754_plugin
  );

  TeamCityOutputPlugin tc_plugin;
  mu::tiny::test::Registry::get_current_registry()->install_plugin(&tc_plugin);

  return mu::tiny::test::CommandLineRunner::run_all_tests(argc, argv);
}
