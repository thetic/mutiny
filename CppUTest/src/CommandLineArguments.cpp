#include "CppUTest/CommandLineArguments.hpp"

#include "CppUTest/PlatformSpecificFunctions.hpp"
#include "CppUTest/StringCollection.hpp"
#include "CppUTest/TestHarness.hpp"

namespace cpputest {

CommandLineArguments::CommandLineArguments(int argc, const char* const* argv)
  : ac_(argc)
  , av_(argv)
  , needHelp_(false)
  , verbose_(false)
  , veryVerbose_(false)
  , color_(false)
  , listTestGroupNames_(false)
  , listTestGroupAndCaseNames_(false)
  , listTestLocations_(false)
  , listOrderedTestLocations_(false)
  , runIgnored_(false)
  , reversing_(false)
  , crashOnFail_(false)
  , rethrowExceptions_(true)
  , shuffling_(false)
  , shufflingPreSeeded_(false)
  , repeat_(1)
  , shuffleSeed_(0)
  , groupFilters_(nullptr)
  , nameFilters_(nullptr)
  , outputType_(OutputType::ECLIPSE)
{
}

CommandLineArguments::~CommandLineArguments()
{
  while (groupFilters_) {
    TestFilter* current = groupFilters_;
    groupFilters_ = groupFilters_->getNext();
    delete current;
  }
  while (nameFilters_) {
    TestFilter* current = nameFilters_;
    nameFilters_ = nameFilters_->getNext();
    delete current;
  }
}

bool
CommandLineArguments::parse(TestPlugin* plugin)
{
  bool correctParameters = true;
  for (int i = 1; i < ac_; i++) {
    String argument = av_[i];

    if (argument == "-h") {
      needHelp_ = true;
      correctParameters = false;
    } else if (argument == "-v")
      verbose_ = true;
    else if (argument == "-vv")
      veryVerbose_ = true;
    else if (argument == "-c")
      color_ = true;
    else if (argument == "-b")
      reversing_ = true;
    else if (argument == "-lg")
      listTestGroupNames_ = true;
    else if (argument == "-ln")
      listTestGroupAndCaseNames_ = true;
    else if (argument == "-llo")
      listOrderedTestLocations_ = true;
    else if (argument == "-ll")
      listTestLocations_ = true;
    else if (argument == "-ri")
      runIgnored_ = true;
    else if (argument == "-f")
      crashOnFail_ = true;
    else if ((argument == "-e") || (argument == "-ci"))
      rethrowExceptions_ = false;
    else if (stringStartsWith(argument, "-r"))
      setRepeatCount(ac_, av_, i);
    else if (stringStartsWith(argument, "-g"))
      addGroupFilter(ac_, av_, i);
    else if (stringStartsWith(argument, "-t"))
      correctParameters =
          addGroupDotNameFilter(ac_, av_, i, "-t", false, false);
    else if (stringStartsWith(argument, "-st"))
      correctParameters =
          addGroupDotNameFilter(ac_, av_, i, "-st", true, false);
    else if (stringStartsWith(argument, "-xt"))
      correctParameters =
          addGroupDotNameFilter(ac_, av_, i, "-xt", false, true);
    else if (stringStartsWith(argument, "-xst"))
      correctParameters =
          addGroupDotNameFilter(ac_, av_, i, "-xst", true, true);
    else if (stringStartsWith(argument, "-sg"))
      addStrictGroupFilter(ac_, av_, i);
    else if (stringStartsWith(argument, "-xg"))
      addExcludeGroupFilter(ac_, av_, i);
    else if (stringStartsWith(argument, "-xsg"))
      addExcludeStrictGroupFilter(ac_, av_, i);
    else if (stringStartsWith(argument, "-n"))
      addNameFilter(ac_, av_, i);
    else if (stringStartsWith(argument, "-sn"))
      addStrictNameFilter(ac_, av_, i);
    else if (stringStartsWith(argument, "-xn"))
      addExcludeNameFilter(ac_, av_, i);
    else if (stringStartsWith(argument, "-xsn"))
      addExcludeStrictNameFilter(ac_, av_, i);
    else if (stringStartsWith(argument, "-s"))
      correctParameters = setShuffle(ac_, av_, i);
    else if (stringStartsWith(argument, "TEST("))
      addTestToRunBasedOnVerboseOutput(ac_, av_, i, "TEST(");
    else if (stringStartsWith(argument, "IGNORE_TEST("))
      addTestToRunBasedOnVerboseOutput(ac_, av_, i, "IGNORE_TEST(");
    else if (stringStartsWith(argument, "-o"))
      correctParameters = setOutputType(ac_, av_, i);
    else if (stringStartsWith(argument, "-p"))
      correctParameters = plugin->parseAllArguments(ac_, av_, i);
    else if (stringStartsWith(argument, "-k"))
      setPackageName(ac_, av_, i);
    else
      correctParameters = false;

    if (correctParameters == false) {
      return false;
    }
  }
  return true;
}

const char*
CommandLineArguments::usage() const
{
  return "use -h for more extensive help\n"
         "usage [-h] [-v] [-vv] [-c] [-lg] [-ln] [-ll] [-llo] [-ri] [-r[<#>]] "
         "[-f] [-e] [-ci]\n"
         "      [-g|sg|xg|xsg <groupName>]... [-n|sn|xn|xsn <testName>]... "
         "[-t|st|xt|xst <groupName>.<testName>]...\n"
         "      [-b] [-s [<seed>]] [\"[IGNORE_]TEST(<groupName>, "
         "<testName>)\"]...\n"
         "      [-o{normal|eclipse|junit|teamcity}] [-k <packageName>]\n";
}

const char*
CommandLineArguments::help() const
{
  return "Thanks for using CppUTest.\n"
         "\n"
         "Options that do not run tests but query:\n"
         "  -h                - this wonderful help screen. Joy!\n"
         "  -lg               - print a list of group names, separated by "
         "spaces\n"
         "  -ln               - print a list of test names in the form of "
         "group.name, separated by spaces\n"
         "  -ll               - print a list of test names in the form of "
         "group.name.test_file_path.line\n"
         "  -llo              - print a list of ordered test names in the form "
         "of group.name.test_file_path.line\n"
         "\n"
         "Options that change the output format:\n"
         "  -c                - colorize output, print green if OK, or red if "
         "failed\n"
         "  -v                - verbose, print each test name as it runs\n"
         "  -vv               - very verbose, print internal information "
         "during test run\n"
         "\n"
         "Options that change the output location:\n"
         "  -onormal          - no output to files\n"
         "  -oeclipse         - equivalent to -onormal\n"
         "  -oteamcity        - output to xml files (as the name suggests, for "
         "TeamCity)\n"
         "  -ojunit           - output to JUnit ant plugin style xml files "
         "(for CI systems)\n"
         "  -k <packageName>  - add a package name in JUnit output (for "
         "classification in CI systems)\n"
         "\n"
         "\n"
         "Options that control which tests are run:\n"
         "  -g <group>        - only run tests whose group contains <group>\n"
         "  -n <name>         - only run tests whose name contains <name>\n"
         "  -t <group>.<name> - only run tests whose group and name contain "
         "<group> and <name>\n"
         "  -sg <group>       - only run tests whose group exactly matches "
         "<group>\n"
         "  -sn <name>        - only run tests whose name exactly matches "
         "<name>\n"
         "  -st <grp>.<name>  - only run tests whose group and name exactly "
         "match <grp> and <name>\n"
         "  -xg <group>       - exclude tests whose group contains <group>\n"
         "  -xn <name>        - exclude tests whose name contains <name>\n"
         "  -xt <grp>.<name>  - exclude tests whose group and name contain "
         "<grp> and <name>\n"
         "  -xsg <group>      - exclude tests whose group exactly matches "
         "<group>\n"
         "  -xsn <name>       - exclude tests whose name exactly matches "
         "<name>\n"
         "  -xst <grp>.<name> - exclude tests whose group and name exactly "
         "match <grp> and <name>\n"
         "  \"[IGNORE_]TEST(<group>, <name>)\"\n"
         "                    - only run tests whose group and name exactly "
         "match <group> and <name>\n"
         "                      (this can be used to copy-paste output from "
         "the -v option on the command line)\n"
         "\n"
         "Options that control how the tests are run:\n"
         "  -b                - run the tests backwards, reversing the normal "
         "way\n"
         "  -s [<seed>]       - shuffle tests randomly (randomization seed is "
         "optional, must be greater than 0)\n"
         "  -r[<#>]           - repeat the tests <#> times (or twice if <#> is "
         "not specified)\n"
         "  -ri               - run ignored tests as if they are not ignored\n"
         "  -f                - Cause the tests to crash on failure (to allow "
         "the test to be debugged if necessary)\n"
         "  -e                - do not rethrow unexpected exceptions on "
         "failure\n"
         "  -ci               - continuous integration mode (equivalent to "
         "-e)\n";
}

bool
CommandLineArguments::needHelp() const
{
  return needHelp_;
}

bool
CommandLineArguments::isVerbose() const
{
  return verbose_;
}

bool
CommandLineArguments::isVeryVerbose() const
{
  return veryVerbose_;
}

bool
CommandLineArguments::isColor() const
{
  return color_;
}

bool
CommandLineArguments::isListingTestGroupNames() const
{
  return listTestGroupNames_;
}

bool
CommandLineArguments::isListingTestGroupAndCaseNames() const
{
  return listTestGroupAndCaseNames_;
}

bool
CommandLineArguments::isListingTestLocations() const
{
  return listTestLocations_;
}

bool
CommandLineArguments::isListingOrderedTestLocations() const
{
  return listOrderedTestLocations_;
}

bool
CommandLineArguments::isRunIgnored() const
{
  return runIgnored_;
}

size_t
CommandLineArguments::getRepeatCount() const
{
  return repeat_;
}

bool
CommandLineArguments::isReversing() const
{
  return reversing_;
}

bool
CommandLineArguments::isCrashingOnFail() const
{
  return crashOnFail_;
}

bool
CommandLineArguments::isRethrowingExceptions() const
{
  return rethrowExceptions_;
}

bool
CommandLineArguments::isShuffling() const
{
  return shuffling_;
}

size_t
CommandLineArguments::getShuffleSeed() const
{
  return shuffleSeed_;
}

const TestFilter*
CommandLineArguments::getGroupFilters() const
{
  return groupFilters_;
}

const TestFilter*
CommandLineArguments::getNameFilters() const
{
  return nameFilters_;
}

void
CommandLineArguments::setRepeatCount(int argc, const char* const* argv, int& i)
{
  repeat_ = 0;

  String repeatParameter(argv[i]);
  if (repeatParameter.size() > 2)
    repeat_ = static_cast<size_t>(AtoI(argv[i] + 2));
  else if (i + 1 < argc) {
    repeat_ = static_cast<size_t>(AtoI(argv[i + 1]));
    if (repeat_ != 0)
      i++;
  }

  if (0 == repeat_)
    repeat_ = 2;
}

bool
CommandLineArguments::setShuffle(int argc, const char* const* argv, int& i)
{
  shuffling_ = true;
  shuffleSeed_ = static_cast<unsigned int>(GetPlatformSpecificTimeInMillis());
  if (shuffleSeed_ == 0)
    shuffleSeed_++;

  String shuffleParameter = argv[i];
  if (shuffleParameter.size() > 2) {
    shufflingPreSeeded_ = true;
    shuffleSeed_ = AtoU(argv[i] + 2);
  } else if (i + 1 < argc) {
    unsigned int parsedParameter = AtoU(argv[i + 1]);
    if (parsedParameter != 0) {
      shufflingPreSeeded_ = true;
      shuffleSeed_ = parsedParameter;
      i++;
    }
  }
  return (shuffleSeed_ != 0);
}

String
CommandLineArguments::getParameterField(int argc,
    const char* const* argv,
    int& i,
    const String& parameterName)
{
  size_t parameterLength = parameterName.size();
  String parameter(argv[i]);
  if (parameter.size() > parameterLength)
    return argv[i] + parameterLength;
  else if (i + 1 < argc)
    return argv[++i];
  return "";
}

void
CommandLineArguments::addGroupFilter(int argc, const char* const* argv, int& i)
{
  auto* groupFilter = new TestFilter(getParameterField(argc, argv, i, "-g"));
  groupFilters_ = groupFilter->add(groupFilters_);
}

bool
CommandLineArguments::addGroupDotNameFilter(int argc,
    const char* const* argv,
    int& i,
    const String& parameterName,
    bool strict,
    bool exclude)
{
  String groupDotName = getParameterField(argc, argv, i, parameterName);
  StringCollection collection(groupDotName, '.');

  if (collection.size() != 2)
    return false;

  auto* groupFilter =
      new TestFilter(collection[0].substr(0, collection[0].size() - 1));
  auto* nameFilter = new TestFilter(collection[1]);
  if (strict) {
    groupFilter->strictMatching();
    nameFilter->strictMatching();
  }
  if (exclude) {
    groupFilter->invertMatching();
    nameFilter->invertMatching();
  }
  groupFilters_ = groupFilter->add(groupFilters_);
  nameFilters_ = nameFilter->add(nameFilters_);
  return true;
}

void
CommandLineArguments::addStrictGroupFilter(int argc,
    const char* const* argv,
    int& i)
{
  auto* groupFilter = new TestFilter(getParameterField(argc, argv, i, "-sg"));
  groupFilter->strictMatching();
  groupFilters_ = groupFilter->add(groupFilters_);
}

void
CommandLineArguments::addExcludeGroupFilter(int argc,
    const char* const* argv,
    int& i)
{
  auto* groupFilter = new TestFilter(getParameterField(argc, argv, i, "-xg"));
  groupFilter->invertMatching();
  groupFilters_ = groupFilter->add(groupFilters_);
}

void
CommandLineArguments::addExcludeStrictGroupFilter(int argc,
    const char* const* argv,
    int& i)
{
  auto* groupFilter = new TestFilter(getParameterField(argc, argv, i, "-xsg"));
  groupFilter->strictMatching();
  groupFilter->invertMatching();
  groupFilters_ = groupFilter->add(groupFilters_);
}

void
CommandLineArguments::addNameFilter(int argc, const char* const* argv, int& i)
{
  auto* nameFilter = new TestFilter(getParameterField(argc, argv, i, "-n"));
  nameFilters_ = nameFilter->add(nameFilters_);
}

void
CommandLineArguments::addStrictNameFilter(int argc,
    const char* const* argv,
    int& index)
{
  auto* nameFilter =
      new TestFilter(getParameterField(argc, argv, index, "-sn"));
  nameFilter->strictMatching();
  nameFilters_ = nameFilter->add(nameFilters_);
}

void
CommandLineArguments::addExcludeNameFilter(int argc,
    const char* const* argv,
    int& index)
{
  auto* nameFilter =
      new TestFilter(getParameterField(argc, argv, index, "-xn"));
  nameFilter->invertMatching();
  nameFilters_ = nameFilter->add(nameFilters_);
}

void
CommandLineArguments::addExcludeStrictNameFilter(int argc,
    const char* const* argv,
    int& index)
{
  auto* nameFilter =
      new TestFilter(getParameterField(argc, argv, index, "-xsn"));
  nameFilter->invertMatching();
  nameFilter->strictMatching();
  nameFilters_ = nameFilter->add(nameFilters_);
}

void
CommandLineArguments::addTestToRunBasedOnVerboseOutput(int argc,
    const char* const* argv,
    int& index,
    const char* parameterName)
{
  String wholename = getParameterField(argc, argv, index, parameterName);
  String testname = wholename.subStringFromTill(',', ')');
  testname = testname.substr(2);
  auto* namefilter = new TestFilter(testname);
  auto* groupfilter =
      new TestFilter(wholename.subStringFromTill(wholename[0], ','));
  namefilter->strictMatching();
  groupfilter->strictMatching();
  groupFilters_ = groupfilter->add(groupFilters_);
  nameFilters_ = namefilter->add(nameFilters_);
}

void
CommandLineArguments::setPackageName(int argc, const char* const* argv, int& i)
{
  String packageName = getParameterField(argc, argv, i, "-k");
  if (packageName.size() == 0)
    return;

  packageName_ = packageName;
}

bool
CommandLineArguments::setOutputType(int argc, const char* const* argv, int& i)
{
  String outputType = getParameterField(argc, argv, i, "-o");
  if (outputType.size() == 0)
    return false;

  if (outputType == "normal" || outputType == "eclipse") {
    outputType_ = OutputType::ECLIPSE;
    return true;
  }
  if (outputType == "junit") {
    outputType_ = OutputType::JUNIT;
    return true;
  }
  if (outputType == "teamcity") {
    outputType_ = OutputType::TEAMCITY;
    return true;
  }

  return false;
}

bool
CommandLineArguments::isEclipseOutput() const
{
  return outputType_ == OutputType::ECLIPSE;
}

bool
CommandLineArguments::isJUnitOutput() const
{
  return outputType_ == OutputType::JUNIT;
}

bool
CommandLineArguments::isTeamCityOutput() const
{
  return outputType_ == OutputType::TEAMCITY;
}

const String&
CommandLineArguments::getPackageName() const
{
  return packageName_;
}

} // namespace cpputest
