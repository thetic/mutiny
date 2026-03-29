///////////////////////////////////////////////////////////////////////////////
//
// Failure is a class which holds information for a specific
// test failure. It can be overriden for more complex failure messages
//
///////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_MUTINY_TEST_FAILURE_HPP
#define INCLUDED_MUTINY_TEST_FAILURE_HPP

#include "mutiny/String.hpp"
#include "mutiny/export.h"
#include "mutiny/features.hpp"

#if MUTINY_USE_STD_CPP_LIB
#include <stdexcept>
#endif

namespace mu {
namespace tiny {
namespace test {

class Shell;
class Output;

class MUTINY_EXPORT Failure
{

public:
  Failure(
      Shell*,
      const char* file_name,
      size_t line_number,
      const String& the_message
  );
  Failure(Shell*, const String& the_message);
  Failure(Shell*, const char* file_name, size_t line_number);
  Failure(const Failure&) = default;
  Failure(Failure&&) noexcept;
  virtual ~Failure() = default;

  virtual const String& get_file_name() const;
  virtual const String& get_test_name() const;
  virtual const String& get_test_name_only() const;
  virtual size_t get_failure_line_number() const;
  virtual const String& get_message() const;
  virtual const String& get_test_file_name() const;
  virtual size_t get_test_line_number() const;
  virtual bool is_error() const { return false; }
  bool is_outside_test_file() const;
  bool is_in_helper_function() const;

protected:
  String create_but_was_string(const String& expected, const String& actual);
  String create_difference_at_pos_string(
      const String& actual,
      size_t offset,
      size_t reported_position
  );
  String create_user_text(const String& text);

  String test_name_;
  String test_name_only_;
  String file_name_;
  size_t line_number_;
  String test_file_name_;
  size_t test_line_number_;
  String message_;

  Failure& operator=(const Failure&);
};

class MUTINY_EXPORT EqualsFailure : public Failure
{
public:
  EqualsFailure(
      Shell*,
      const char* file_name,
      size_t line_number,
      const char* expected,
      const char* actual,
      const String& text
  );
  EqualsFailure(
      Shell*,
      const char* file_name,
      size_t line_number,
      const String& expected,
      const String& actual,
      const String& text
  );
};

class MUTINY_EXPORT DoublesEqualFailure : public Failure
{
public:
  DoublesEqualFailure(
      Shell*,
      const char* file_name,
      size_t line_number,
      double expected,
      double actual,
      double threshold,
      const String& text
  );
};

class MUTINY_EXPORT CheckEqualFailure : public Failure
{
public:
  CheckEqualFailure(
      Shell* test,
      const char* file_name,
      size_t line_number,
      const String& expected,
      const String& actual,
      const String& text
  );
};

class MUTINY_EXPORT ComparisonFailure : public Failure
{
public:
  ComparisonFailure(
      Shell* test,
      const char* file_name,
      size_t line_number,
      const String& check_string,
      const String& comparison_string,
      const String& text
  );
};

class MUTINY_EXPORT ContainsFailure : public Failure
{
public:
  ContainsFailure(
      Shell*,
      const char* file_name,
      size_t line_number,
      const String& expected,
      const String& actual,
      const String& text
  );
};

class MUTINY_EXPORT CheckFailure : public Failure
{
public:
  CheckFailure(
      Shell* test,
      const char* file_name,
      size_t line_number,
      const String& check_string,
      const String& condition_string,
      const String& text_string = ""
  );
};

class MUTINY_EXPORT FailFailure : public Failure
{
public:
  FailFailure(
      Shell* test,
      const char* file_name,
      size_t line_number,
      const String& message
  );
};

class MUTINY_EXPORT LongsEqualFailure : public Failure
{
public:
  LongsEqualFailure(
      Shell* test,
      const char* file_name,
      size_t line_number,
      long expected,
      long actual,
      const String& text
  );
};

class MUTINY_EXPORT UnsignedLongsEqualFailure : public Failure
{
public:
  UnsignedLongsEqualFailure(
      Shell* test,
      const char* file_name,
      size_t line_number,
      unsigned long expected,
      unsigned long actual,
      const String& text
  );
};

class MUTINY_EXPORT LongLongsEqualFailure : public Failure
{
public:
  LongLongsEqualFailure(
      Shell* test,
      const char* file_name,
      size_t line_number,
      long long expected,
      long long actual,
      const String& text
  );
};

class MUTINY_EXPORT UnsignedLongLongsEqualFailure : public Failure
{
public:
  UnsignedLongLongsEqualFailure(
      Shell* test,
      const char* file_name,
      size_t line_number,
      unsigned long long expected,
      unsigned long long actual,
      const String& text
  );
};

class MUTINY_EXPORT SignedBytesEqualFailure : public Failure
{
public:
  SignedBytesEqualFailure(
      Shell* test,
      const char* file_name,
      size_t line_number,
      signed char expected,
      signed char actual,
      const String& text
  );
};

class MUTINY_EXPORT StringEqualFailure : public Failure
{
public:
  StringEqualFailure(
      Shell* test,
      const char* file_name,
      size_t line_number,
      const char* expected,
      const char* actual,
      const String& text
  );
};

class MUTINY_EXPORT StringEqualNoCaseFailure : public Failure
{
public:
  StringEqualNoCaseFailure(
      Shell* test,
      const char* file_name,
      size_t line_number,
      const char* expected,
      const char* actual,
      const String& text
  );
};

class MUTINY_EXPORT BinaryEqualFailure : public Failure
{
public:
  BinaryEqualFailure(
      Shell* test,
      const char* file_name,
      size_t line_number,
      const unsigned char* expected,
      const unsigned char* actual,
      size_t size,
      const String& text
  );
};

class MUTINY_EXPORT FeatureUnsupportedFailure : public Failure
{
public:
  FeatureUnsupportedFailure(
      Shell* test,
      const char* file_name,
      size_t line_number,
      const String& feature_name,
      const String& text
  );
};

#if MUTINY_HAVE_EXCEPTIONS
class MUTINY_EXPORT UnexpectedExceptionFailure : public Failure
{
public:
  bool is_error() const override { return true; }
  UnexpectedExceptionFailure(Shell* test);
#if MUTINY_USE_STD_CPP_LIB
  UnexpectedExceptionFailure(Shell* test, const std::exception& e);
#endif
};
#endif

} // namespace test
} // namespace tiny
} // namespace mu

#endif
