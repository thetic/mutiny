#include "CppUTest/TestFailure.hpp"

#include "CppUTest/String.hpp"
#include "CppUTest/TestHarness.hpp"
#include "CppUTest/TestOutput.hpp"
#include "CppUTest/math.hpp"

#if CPPUTEST_USE_STD_CPP_LIB
#include <typeinfo>
#if defined(__GNUC__)
#include <memory>

#include <cxxabi.h>
#endif
#endif

namespace cpputest {
namespace {
size_t
getPrintableSize(String const& str)
{
  size_t str_size = str.size();
  size_t printable_str_size = str_size;

  for (size_t i = 0; i < str_size; i++) {
    char c = str.c_str()[i];
    if (isControlWithShortEscapeSequence(c)) {
      printable_str_size += 1;
    } else if (isControl(c)) {
      printable_str_size += 3;
    }
  }

  return printable_str_size;
}

String
Printable(String const& str)
{
  static const char* shortEscapeCodes[] = {
    "\\a", "\\b", "\\t", "\\n", "\\v", "\\f", "\\r"
  };

  String result;
  result.reserve(getPrintableSize(str) + 1);

  size_t str_size = str.size();
  size_t j = 0;
  for (size_t i = 0; i < str_size; i++) {
    char c = str.c_str()[i];
    if (isControlWithShortEscapeSequence(c)) {
      StrNCpy(&result.data()[j],
          shortEscapeCodes[static_cast<unsigned char>(c - '\a')],
          2);
      j += 2;
    } else if (isControl(c)) {
      String hexEscapeCode = StringFromFormat("\\x%02X ", c);
      StrNCpy(&result.data()[j], hexEscapeCode.c_str(), 4);
      j += 4;
    } else {
      result.data()[j] = c;
      j++;
    }
  }
  result.data()[j] = 0;

  return result;
}

String
PrintableStringFromOrNull(const char* expected)
{
  return (expected) ? Printable(StringFrom(expected)) : StringFrom("(null)");
}
}

TestFailure::TestFailure(TestShell* test,
    const char* fileName,
    size_t lineNumber,
    const String& theMessage)
  : testName_(test->getFormattedName())
  , testNameOnly_(test->getName())
  , fileName_(fileName)
  , lineNumber_(lineNumber)
  , testFileName_(test->getFile())
  , testLineNumber_(test->getLineNumber())
  , message_(theMessage)
{
}

TestFailure::TestFailure(TestShell* test, const String& theMessage)
  : testName_(test->getFormattedName())
  , testNameOnly_(test->getName())
  , fileName_(test->getFile())
  , lineNumber_(test->getLineNumber())
  , testFileName_(test->getFile())
  , testLineNumber_(test->getLineNumber())
  , message_(theMessage)
{
}

TestFailure::TestFailure(TestShell* test, const char* fileName, size_t lineNum)
  : testName_(test->getFormattedName())
  , testNameOnly_(test->getName())
  , fileName_(fileName)
  , lineNumber_(lineNum)
  , testFileName_(test->getFile())
  , testLineNumber_(test->getLineNumber())
  , message_("no message")
{
}

TestFailure::TestFailure(const TestFailure& f)
  : testName_(f.testName_)
  , testNameOnly_(f.testNameOnly_)
  , fileName_(f.fileName_)
  , lineNumber_(f.lineNumber_)
  , testFileName_(f.testFileName_)
  , testLineNumber_(f.testLineNumber_)
  , message_(f.message_)
{
}

TestFailure::TestFailure(TestFailure&& f) noexcept
  : testName_(static_cast<String&&>(f.testName_))
  , testNameOnly_(static_cast<String&&>(f.testNameOnly_))
  , fileName_(static_cast<String&&>(f.fileName_))
  , lineNumber_(f.lineNumber_)
  , testFileName_(static_cast<String&&>(f.testFileName_))
  , testLineNumber_(f.testLineNumber_)
  , message_(static_cast<String&&>(f.message_))
{
}

TestFailure::~TestFailure() {}

String
TestFailure::getFileName() const
{
  return fileName_;
}

String
TestFailure::getTestFileName() const
{
  return testFileName_;
}

String
TestFailure::getTestName() const
{
  return testName_;
}

String
TestFailure::getTestNameOnly() const
{
  return testNameOnly_;
}

size_t
TestFailure::getFailureLineNumber() const
{
  return lineNumber_;
}

size_t
TestFailure::getTestLineNumber() const
{
  return testLineNumber_;
}

String
TestFailure::getMessage() const
{
  return message_;
}

bool
TestFailure::isOutsideTestFile() const
{
  return testFileName_ != fileName_;
}

bool
TestFailure::isInHelperFunction() const
{
  return lineNumber_ < testLineNumber_;
}

String
TestFailure::createButWasString(const String& expected, const String& actual)
{
  return StringFromFormat(
      "expected <%s>\n\tbut was  <%s>", expected.c_str(), actual.c_str());
}

String
TestFailure::createDifferenceAtPosString(const String& actual,
    size_t offset,
    size_t reportedPosition)
{
  String result;
  const size_t extraCharactersWindow = 20;
  const size_t halfOfExtraCharactersWindow = extraCharactersWindow / 2;

  String paddingForPreventingOutOfBounds(" ", halfOfExtraCharactersWindow);
  String actualString = paddingForPreventingOutOfBounds + actual +
                        paddingForPreventingOutOfBounds;
  String differentString =
      StringFromFormat("difference starts at position %lu at: <",
          static_cast<unsigned long>(reportedPosition));

  result += "\n";
  result += StringFromFormat("\t%s%s>\n",
      differentString.c_str(),
      actualString.substr(offset, extraCharactersWindow).c_str());

  result += StringFromFormat("\t%s^",
      String(" ", (differentString.size() + halfOfExtraCharactersWindow))
          .c_str());
  return result;
}

String
TestFailure::createUserText(const String& text)
{
  String userMessage = "";
  if (!text.empty()) {
    // This is a kludge to turn off "Message: " for this case.
    // I don't think "Message: " adds anything, as you get to see the
    // message. I propose we remove "Message: " lead in
    if (!stringStartsWith(text, "LONGS_EQUAL"))
      userMessage += "Message: ";
    userMessage += text;
    userMessage += "\n\t";
  }
  return userMessage;
}

EqualsFailure::EqualsFailure(TestShell* test,
    const char* fileName,
    size_t lineNumber,
    const char* expected,
    const char* actual,
    const String& text)
  : TestFailure(test, fileName, lineNumber)
{
  message_ = createUserText(text);

  message_ +=
      createButWasString(StringFromOrNull(expected), StringFromOrNull(actual));
}

EqualsFailure::EqualsFailure(TestShell* test,
    const char* fileName,
    size_t lineNumber,
    const String& expected,
    const String& actual,
    const String& text)
  : TestFailure(test, fileName, lineNumber)
{
  message_ = createUserText(text);

  message_ += createButWasString(expected, actual);
}

DoublesEqualFailure::DoublesEqualFailure(TestShell* test,
    const char* fileName,
    size_t lineNumber,
    double expected,
    double actual,
    double threshold,
    const String& text)
  : TestFailure(test, fileName, lineNumber)
{
  message_ = createUserText(text);

  message_ +=
      createButWasString(StringFrom(expected, 7), StringFrom(actual, 7));
  message_ += " threshold used was <";
  message_ += StringFrom(threshold, 7);
  message_ += ">";

  if (cpputest::IsNan(expected) || cpputest::IsNan(actual) ||
      cpputest::IsNan(threshold))
    message_ += "\n\tCannot make comparisons with Nan";
}

CheckEqualFailure::CheckEqualFailure(TestShell* test,
    const char* fileName,
    size_t lineNumber,
    const String& expected,
    const String& actual,
    const String& text)
  : TestFailure(test, fileName, lineNumber)
{
  message_ = createUserText(text);

  String printableExpected = PrintableStringFromOrNull(expected.c_str());
  String printableActual = PrintableStringFromOrNull(actual.c_str());

  message_ += createButWasString(printableExpected, printableActual);

  size_t failStart;
  for (failStart = 0; actual[failStart] == expected[failStart]; failStart++)
    ;
  size_t failStartPrintable;
  for (failStartPrintable = 0; printableActual[failStartPrintable] ==
                               printableExpected[failStartPrintable];
      failStartPrintable++)
    ;
  message_ += createDifferenceAtPosString(
      printableActual, failStartPrintable, failStart);
}

ComparisonFailure::ComparisonFailure(TestShell* test,
    const char* fileName,
    size_t lineNumber,
    const String& checkString,
    const String& comparisonString,
    const String& text)
  : TestFailure(test, fileName, lineNumber)
{
  message_ = createUserText(text);
  message_ += checkString;
  message_ += "(";
  message_ += comparisonString;
  message_ += ") failed";
}

ContainsFailure::ContainsFailure(TestShell* test,
    const char* fileName,
    size_t lineNumber,
    const String& expected,
    const String& actual,
    const String& text)
  : TestFailure(test, fileName, lineNumber)
{
  message_ = createUserText(text);

  message_ += StringFromFormat(
      "actual <%s>\n\tdid not contain  <%s>", actual.c_str(), expected.c_str());
}

CheckFailure::CheckFailure(TestShell* test,
    const char* fileName,
    size_t lineNumber,
    const String& checkString,
    const String& conditionString,
    const String& text)
  : TestFailure(test, fileName, lineNumber)
{
  message_ = createUserText(text);

  message_ += checkString;
  message_ += "(";
  message_ += conditionString;
  message_ += ") failed";
}

FailFailure::FailFailure(TestShell* test,
    const char* fileName,
    size_t lineNumber,
    const String& message)
  : TestFailure(test, fileName, lineNumber)
{
  message_ = message;
}

LongsEqualFailure::LongsEqualFailure(TestShell* test,
    const char* fileName,
    size_t lineNumber,
    long expected,
    long actual,
    const String& text)
  : TestFailure(test, fileName, lineNumber)
{
  message_ = createUserText(text);

  String aDecimal = StringFrom(actual);
  String eDecimal = StringFrom(expected);

  padStringsToSameLength(aDecimal, eDecimal, ' ');

  String actualReported =
      aDecimal + " " + BracketsFormattedHexStringFrom(actual);
  String expectedReported =
      eDecimal + " " + BracketsFormattedHexStringFrom(expected);
  message_ += createButWasString(expectedReported, actualReported);
}

UnsignedLongsEqualFailure::UnsignedLongsEqualFailure(TestShell* test,
    const char* fileName,
    size_t lineNumber,
    unsigned long expected,
    unsigned long actual,
    const String& text)
  : TestFailure(test, fileName, lineNumber)
{
  message_ = createUserText(text);

  String aDecimal = StringFrom(actual);
  String eDecimal = StringFrom(expected);

  padStringsToSameLength(aDecimal, eDecimal, ' ');

  String actualReported =
      aDecimal + " " + BracketsFormattedHexStringFrom(actual);
  String expectedReported =
      eDecimal + " " + BracketsFormattedHexStringFrom(expected);

  message_ += createButWasString(expectedReported, actualReported);
}

LongLongsEqualFailure::LongLongsEqualFailure(TestShell* test,
    const char* fileName,
    size_t lineNumber,
    long long expected,
    long long actual,
    const String& text)
  : TestFailure(test, fileName, lineNumber)
{
  message_ = createUserText(text);

  String aDecimal = StringFrom(actual);
  String eDecimal = StringFrom(expected);

  padStringsToSameLength(aDecimal, eDecimal, ' ');

  String actualReported =
      aDecimal + " " + BracketsFormattedHexStringFrom(actual);
  String expectedReported =
      eDecimal + " " + BracketsFormattedHexStringFrom(expected);
  message_ += createButWasString(expectedReported, actualReported);
}

UnsignedLongLongsEqualFailure::UnsignedLongLongsEqualFailure(TestShell* test,
    const char* fileName,
    size_t lineNumber,
    unsigned long long expected,
    unsigned long long actual,
    const String& text)
  : TestFailure(test, fileName, lineNumber)
{
  message_ = createUserText(text);

  String aDecimal = StringFrom(actual);
  String eDecimal = StringFrom(expected);

  padStringsToSameLength(aDecimal, eDecimal, ' ');

  String actualReported =
      aDecimal + " " + BracketsFormattedHexStringFrom(actual);
  String expectedReported =
      eDecimal + " " + BracketsFormattedHexStringFrom(expected);
  message_ += createButWasString(expectedReported, actualReported);
}

SignedBytesEqualFailure::SignedBytesEqualFailure(TestShell* test,
    const char* fileName,
    size_t lineNumber,
    signed char expected,
    signed char actual,
    const String& text)
  : TestFailure(test, fileName, lineNumber)
{
  message_ = createUserText(text);

  String aDecimal = StringFrom(static_cast<int>(actual));
  String eDecimal = StringFrom(static_cast<int>(expected));

  padStringsToSameLength(aDecimal, eDecimal, ' ');

  String actualReported =
      aDecimal + " " + BracketsFormattedHexStringFrom(actual);
  String expectedReported =
      eDecimal + " " + BracketsFormattedHexStringFrom(expected);
  message_ += createButWasString(expectedReported, actualReported);
}

StringEqualFailure::StringEqualFailure(TestShell* test,
    const char* fileName,
    size_t lineNumber,
    const char* expected,
    const char* actual,
    const String& text)
  : TestFailure(test, fileName, lineNumber)
{
  message_ = createUserText(text);

  String printableExpected = PrintableStringFromOrNull(expected);
  String printableActual = PrintableStringFromOrNull(actual);

  message_ += createButWasString(printableExpected, printableActual);
  if ((expected) && (actual)) {
    size_t failStart;
    for (failStart = 0; actual[failStart] == expected[failStart]; failStart++)
      ;
    size_t failStartPrintable;
    for (failStartPrintable = 0; printableActual[failStartPrintable] ==
                                 printableExpected[failStartPrintable];
        failStartPrintable++)
      ;
    message_ += createDifferenceAtPosString(
        printableActual, failStartPrintable, failStart);
  }
}

StringEqualNoCaseFailure::StringEqualNoCaseFailure(TestShell* test,
    const char* fileName,
    size_t lineNumber,
    const char* expected,
    const char* actual,
    const String& text)
  : TestFailure(test, fileName, lineNumber)
{
  message_ = createUserText(text);

  String printableExpected = PrintableStringFromOrNull(expected);
  String printableActual = PrintableStringFromOrNull(actual);

  message_ += createButWasString(printableExpected, printableActual);
  if ((expected) && (actual)) {
    size_t failStart;
    for (failStart = 0;
        ToLower(actual[failStart]) == ToLower(expected[failStart]);
        failStart++)
      ;
    size_t failStartPrintable;
    for (failStartPrintable = 0; ToLower(printableActual[failStartPrintable]) ==
                                 ToLower(printableExpected[failStartPrintable]);
        failStartPrintable++)
      ;
    message_ += createDifferenceAtPosString(
        printableActual, failStartPrintable, failStart);
  }
}

BinaryEqualFailure::BinaryEqualFailure(TestShell* test,
    const char* fileName,
    size_t lineNumber,
    const unsigned char* expected,
    const unsigned char* actual,
    size_t size,
    const String& text)
  : TestFailure(test, fileName, lineNumber)
{
  message_ = createUserText(text);

  String actualHex = StringFromBinaryOrNull(actual, size);

  message_ +=
      createButWasString(StringFromBinaryOrNull(expected, size), actualHex);
  if ((expected) && (actual)) {
    size_t failStart;
    for (failStart = 0; actual[failStart] == expected[failStart]; failStart++)
      ;
    message_ +=
        createDifferenceAtPosString(actualHex, (failStart * 3 + 1), failStart);
  }
}

FeatureUnsupportedFailure::FeatureUnsupportedFailure(TestShell* test,
    const char* fileName,
    size_t lineNumber,
    const String& featureName,
    const String& text)
  : TestFailure(test, fileName, lineNumber)
{
  message_ = createUserText(text);

  message_ += StringFromFormat(
      "The feature \"%s\" is not supported in this environment or with the "
      "feature set selected when building the library.",
      featureName.c_str());
}

#if CPPUTEST_HAVE_EXCEPTIONS
UnexpectedExceptionFailure::UnexpectedExceptionFailure(TestShell* test)
  : TestFailure(test, "Unexpected exception of unknown type was thrown.")
{
}

#if CPPUTEST_USE_STD_CPP_LIB
#if CPPUTEST_HAVE_RTTI
namespace {
String
getExceptionTypeName(const std::exception& e)
{
  const char* name = typeid(e).name();
#if defined(__GNUC__)
  int status = -1;

  std::unique_ptr<char, void (*)(void*)> demangledName(
      abi::__cxa_demangle(name, nullptr, nullptr, &status), std::free);

  return (status == 0) ? demangledName.get() : name;
#else
  return name;
#endif
}
} // namespace
#endif // CPPUTEST_HAVE_RTTI

UnexpectedExceptionFailure::UnexpectedExceptionFailure(TestShell* test,
    const std::exception& e)
  : TestFailure(test,
#if CPPUTEST_HAVE_RTTI
        StringFromFormat("Unexpected exception of type '%s' was thrown: %s",
            getExceptionTypeName(e).c_str(),
            e.what())
#else
        "Unexpected exception of unknown type was thrown."
#endif
    )
{
  (void)e;
}
#endif // CPPUTEST_USE_STD_CPP_LIB
#endif // CPPUTEST_HAVE_EXCEPTIONS

} // namespace cpputest
