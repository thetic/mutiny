#ifndef D_CommandLineArguments_H
#define D_CommandLineArguments_H

#include "CppUTest/String.hpp"
#include "CppUTest/TestFilter.hpp"
#include "CppUTest/TestOutput.hpp"

namespace cpputest {

class TestPlugin;

class CommandLineArguments
{
public:
  explicit CommandLineArguments(int ac, const char* const* av);
  virtual ~CommandLineArguments();

  bool parse(TestPlugin* plugin);
  bool needHelp() const;
  bool isVerbose() const;
  bool isVeryVerbose() const;
  bool isColor() const;
  bool isListingTestGroupNames() const;
  bool isListingTestGroupAndCaseNames() const;
  bool isListingTestLocations() const;
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
  enum OutputType
  {
    OUTPUT_ECLIPSE,
    OUTPUT_JUNIT,
    OUTPUT_TEAMCITY
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

  String getParameterField(int ac,
      const char* const* av,
      int& i,
      const String& parameterName);
  void setRepeatCount(int ac, const char* const* av, int& index);
  bool setShuffle(int ac, const char* const* av, int& index);
  void addGroupFilter(int ac, const char* const* av, int& index);
  bool addGroupDotNameFilter(int ac,
      const char* const* av,
      int& index,
      const String& parameterName,
      bool strict,
      bool exclude);
  void addStrictGroupFilter(int ac, const char* const* av, int& index);
  void addExcludeGroupFilter(int ac, const char* const* av, int& index);
  void addExcludeStrictGroupFilter(int ac, const char* const* av, int& index);
  void addNameFilter(int ac, const char* const* av, int& index);
  void addStrictNameFilter(int ac, const char* const* av, int& index);
  void addExcludeNameFilter(int ac, const char* const* av, int& index);
  void addExcludeStrictNameFilter(int ac, const char* const* av, int& index);
  void addTestToRunBasedOnVerboseOutput(int ac,
      const char* const* av,
      int& index,
      const char* parameterName);
  bool setOutputType(int ac, const char* const* av, int& index);
  void setPackageName(int ac, const char* const* av, int& index);

  CommandLineArguments(const CommandLineArguments&);
  CommandLineArguments& operator=(const CommandLineArguments&);
};

} // namespace cpputest

#endif
