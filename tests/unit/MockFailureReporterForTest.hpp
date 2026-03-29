#ifndef INCLUDED_MUTINY_MOCKFAILUREREPORTERFORTEST_HPP
#define INCLUDED_MUTINY_MOCKFAILUREREPORTERFORTEST_HPP

#include "MockFailureReporter.hpp"

#include "mutiny/mock/CheckedExpectedCall.hpp"
#include "mutiny/mock/ExpectedCallsList.hpp"

class ExpectedCallsListForTest : public mu::tiny::mock::ExpectedCallsList
{
public:
  ~ExpectedCallsListForTest() override;
  mu::tiny::mock::CheckedExpectedCall* add_function(
      const mu::tiny::String& name
  );
  mu::tiny::mock::CheckedExpectedCall* add_function(
      unsigned int num_calls,
      const mu::tiny::String& name
  );
  mu::tiny::mock::CheckedExpectedCall* add_function_ordered(
      const mu::tiny::String& name,
      unsigned int order
  );
};

#endif
