#ifndef INCLUDED_CPPUTEST_COMMANDLINEARGUMENTS_HPP
#define INCLUDED_CPPUTEST_COMMANDLINEARGUMENTS_HPP

#include "CppUTest/String.hpp"
#include "CppUTest/TestFilter.hpp"
#include "CppUTest/TestOutput.hpp"

namespace cpputest {

class TestPlugin;

class CommandLineArguments
{
public:
  explicit CommandLineArguments(int argc, const char* const* argv);
  virtual ~CommandLineArguments();

  bool parse(TestPlugin* plugin);
  bool needHelp() const;
  bool isVerbose() const;
  bool isVeryVerbose() const;
  bool isColor() const;
  bool isListingTestGroupNames() const;
  bool isListingTestGroupAndCaseNames() const;
  bool isListingTestLocations() const;
  bool isListingOrderedTestLocations() const;
  bool isRunIgnored() const;
  size_t getRepeatCount() const;
  bool isShuffling() const;
  bool isReversing() const;
  bool isCrashingOnFail() const;
  bool isRethrowingExceptions() const;
  size_t getShuffleSeed() const;
  const TestFilter* getGroupFilters() const;
  const TestFilter* getNameFilters() const;
  bool isJUnitOutput() const;
  bool isEclipseOutput() const;
  bool isTeamCityOutput() const;
  const String& getPackageName() const;
  const char* usage() const;
  const char* help() const;

private:
  enum class OutputType
  {
    ECLIPSE,
    JUNIT,
    TEAMCITY
  };

  int ac_;
  const char* const* av_;

  bool needHelp_;
  bool verbose_;
  bool veryVerbose_;
  bool color_;
  bool listTestGroupNames_;
  bool listTestGroupAndCaseNames_;
  bool listTestLocations_;
  bool listOrderedTestLocations_;
  bool runIgnored_;
  bool reversing_;
  bool crashOnFail_;
  bool rethrowExceptions_;
  bool shuffling_;
  bool shufflingPreSeeded_;
  size_t repeat_;
  size_t shuffleSeed_;
  TestFilter* groupFilters_;
  TestFilter* nameFilters_;
  OutputType outputType_;
  String packageName_;

  String getParameterField(int argc,
      const char* const* argv,
      int& i,
      const String& parameterName);
  void setRepeatCount(int argc, const char* const* argv, int& index);
  bool setShuffle(int argc, const char* const* argv, int& index);
  void addGroupFilter(int argc, const char* const* argv, int& index);
  bool addGroupDotNameFilter(int argc,
      const char* const* argv,
      int& index,
      const String& parameterName,
      bool strict,
      bool exclude);
  void addStrictGroupFilter(int argc, const char* const* argv, int& index);
  void addExcludeGroupFilter(int argc, const char* const* argv, int& index);
  void addExcludeStrictGroupFilter(int argc,
      const char* const* argv,
      int& index);
  void addNameFilter(int argc, const char* const* argv, int& index);
  void addStrictNameFilter(int argc, const char* const* argv, int& index);
  void addExcludeNameFilter(int argc, const char* const* argv, int& index);
  void addExcludeStrictNameFilter(int argc,
      const char* const* argv,
      int& index);
  void addTestToRunBasedOnVerboseOutput(int argc,
      const char* const* argv,
      int& index,
      const char* parameterName);
  bool setOutputType(int argc, const char* const* argv, int& index);
  void setPackageName(int argc, const char* const* argv, int& index);

  CommandLineArguments(const CommandLineArguments&);
  CommandLineArguments& operator=(const CommandLineArguments&);
};

} // namespace cpputest

#endif
