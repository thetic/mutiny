///////////////////////////////////////////////////////////////////////////////
//
// Failure is a class which holds information for a specific
// test failure. It can be overriden for more complex failure messages
//
///////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_CPPMU_TESTFAILURE_HPP
#define INCLUDED_CPPMU_TESTFAILURE_HPP

#include "CppMu/String.hpp"

#if CPPMU_USE_STD_CPP_LIB
#include <stdexcept>
#endif

#include "CppMu/features.hpp"

namespace cppmu {

class TestShell;
class TestOutput;

class TestFailure
{

public:
  TestFailure(TestShell*,
      const char* file_name,
      size_t line_number,
      const String& the_message);
  TestFailure(TestShell*, const String& the_message);
  TestFailure(TestShell*, const char* file_name, size_t line_number);
  TestFailure(const TestFailure&) = default;
  TestFailure(TestFailure&&) noexcept;
  virtual ~TestFailure() = default;

  virtual String get_file_name() const;
  virtual String get_test_name() const;
  virtual String get_test_name_only() const;
  virtual size_t get_failure_line_number() const;
  virtual String get_message() const;
  virtual String get_test_file_name() const;
  virtual size_t get_test_line_number() const;
  virtual bool is_error() const { return false; }
  bool is_outside_test_file() const;
  bool is_in_helper_function() const;

protected:
  String create_but_was_string(const String& expected, const String& actual);
  String create_difference_at_pos_string(const String& actual,
      size_t offset,
      size_t reported_position);
  String create_user_text(const String& text);

  String test_name_;
  String test_name_only_;
  String file_name_;
  size_t line_number_;
  String test_file_name_;
  size_t test_line_number_;
  String message_;

  TestFailure& operator=(const TestFailure&);
};

class EqualsFailure : public TestFailure
{
public:
  EqualsFailure(TestShell*,
      const char* file_name,
      size_t line_number,
      const char* expected,
      const char* actual,
      const String& text);
  EqualsFailure(TestShell*,
      const char* file_name,
      size_t line_number,
      const String& expected,
      const String& actual,
      const String& text);
};

class DoublesEqualFailure : public TestFailure
{
public:
  DoublesEqualFailure(TestShell*,
      const char* file_name,
      size_t line_number,
      double expected,
      double actual,
      double threshold,
      const String& text);
};

class CheckEqualFailure : public TestFailure
{
public:
  CheckEqualFailure(TestShell* test,
      const char* file_name,
      size_t line_number,
      const String& expected,
      const String& actual,
      const String& text);
};

class ComparisonFailure : public TestFailure
{
public:
  ComparisonFailure(TestShell* test,
      const char* file_name,
      size_t line_number,
      const String& check_string,
      const String& comparison_string,
      const String& text);
};

class ContainsFailure : public TestFailure
{
public:
  ContainsFailure(TestShell*,
      const char* file_name,
      size_t line_number,
      const String& expected,
      const String& actual,
      const String& text);
};

class CheckFailure : public TestFailure
{
public:
  CheckFailure(TestShell* test,
      const char* file_name,
      size_t line_number,
      const String& check_string,
      const String& condition_string,
      const String& text_string = "");
};

class FailFailure : public TestFailure
{
public:
  FailFailure(TestShell* test,
      const char* file_name,
      size_t line_number,
      const String& message);
};

class LongsEqualFailure : public TestFailure
{
public:
  LongsEqualFailure(TestShell* test,
      const char* file_name,
      size_t line_number,
      long expected,
      long actual,
      const String& text);
};

class UnsignedLongsEqualFailure : public TestFailure
{
public:
  UnsignedLongsEqualFailure(TestShell* test,
      const char* file_name,
      size_t line_number,
      unsigned long expected,
      unsigned long actual,
      const String& text);
};

class LongLongsEqualFailure : public TestFailure
{
public:
  LongLongsEqualFailure(TestShell* test,
      const char* file_name,
      size_t line_number,
      long long expected,
      long long actual,
      const String& text);
};

class UnsignedLongLongsEqualFailure : public TestFailure
{
public:
  UnsignedLongLongsEqualFailure(TestShell* test,
      const char* file_name,
      size_t line_number,
      unsigned long long expected,
      unsigned long long actual,
      const String& text);
};

class SignedBytesEqualFailure : public TestFailure
{
public:
  SignedBytesEqualFailure(TestShell* test,
      const char* file_name,
      size_t line_number,
      signed char expected,
      signed char actual,
      const String& text);
};

class StringEqualFailure : public TestFailure
{
public:
  StringEqualFailure(TestShell* test,
      const char* file_name,
      size_t line_number,
      const char* expected,
      const char* actual,
      const String& text);
};

class StringEqualNoCaseFailure : public TestFailure
{
public:
  StringEqualNoCaseFailure(TestShell* test,
      const char* file_name,
      size_t line_number,
      const char* expected,
      const char* actual,
      const String& text);
};

class BinaryEqualFailure : public TestFailure
{
public:
  BinaryEqualFailure(TestShell* test,
      const char* file_name,
      size_t line_number,
      const unsigned char* expected,
      const unsigned char* actual,
      size_t size,
      const String& text);
};

class FeatureUnsupportedFailure : public TestFailure
{
public:
  FeatureUnsupportedFailure(TestShell* test,
      const char* file_name,
      size_t line_number,
      const String& feature_name,
      const String& text);
};

#if CPPMU_HAVE_EXCEPTIONS
class UnexpectedExceptionFailure : public TestFailure
{
public:
  bool is_error() const override { return true; }
  UnexpectedExceptionFailure(TestShell* test);
#if CPPMU_USE_STD_CPP_LIB
  UnexpectedExceptionFailure(TestShell* test, const std::exception& e);
#endif
};
#endif

} // namespace cppmu

#endif
