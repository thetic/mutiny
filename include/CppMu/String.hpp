///////////////////////////////////////////////////////////////////////////////
//
// SIMPLESTRING.H
//
// One of the design goals of CppUnitLite is to compilation with very old C++
// compilers.  For that reason, the simple string class that provides
// only the operations needed in CppUnitLite.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_CPPMU_STRING_HPP
#define INCLUDED_CPPMU_STRING_HPP

#include <stdarg.h>
#include <stddef.h>

#if CPPMU_USE_STD_CPP_LIB
#include <string>
#endif

#ifndef __has_attribute
#define CPPMU_HAS_ATTRIBUTE(x) 0
#else
#define CPPMU_HAS_ATTRIBUTE(x) __has_attribute(x)
#endif

#if defined(__MINGW32__)
#define CPPMU_CHECK_FORMAT_TYPE __MINGW_PRINTF_FORMAT
#else
#define CPPMU_CHECK_FORMAT_TYPE printf
#endif

namespace cppmu {

class String
{
  friend bool operator==(const String& left, const String& right);
  friend bool operator!=(const String& left, const String& right);

public:
  String(const char* value = "");
  String(size_t count, char ch);
  String(const String& other);
  String(String&& other) noexcept;
  ~String();

  String& operator=(const String& other);
  String& operator=(String&& other) noexcept;
  String operator+(const String&) const;
  String& operator+=(const String&);
  String& operator+=(const char*);

  static const size_t npos = static_cast<size_t>(-1);

  char& operator[](size_t pos) { return data()[pos]; }
  const char& operator[](size_t pos) const { return c_str()[pos]; }

  size_t find(char ch, size_t pos = 0) const;
  size_t find(const char* s, size_t pos = 0) const;

  String substr(size_t begin_pos) const;
  String substr(size_t begin_pos, size_t amount) const;

  const char* c_str() const;
  const char* data() const;
  char* data();
  size_t size() const;
  size_t length() const { return size(); }
  size_t capacity() const { return buffer_size_; }
  bool empty() const;
  void clear();
  void reserve(size_t new_capacity);

  friend bool operator<(const String& left, const String& right);

private:
  void deallocate_internal_buffer();
  void set_internal_buffer_as_empty_string();
  void set_internal_buffer_to(char* buffer, size_t buffer_size);
  void copy_buffer_to_new_internal_buffer(const char* other_buffer);
  void copy_buffer_to_new_internal_buffer(const char* other_buffer,
      size_t buffer_size);
  void copy_buffer_to_new_internal_buffer(const String& other_buffer);

  char* buffer_;
  size_t buffer_size_;

  char* get_empty_string() const;
};

bool
string_contains(const String& str, const String& substr);
bool
string_starts_with(const String& str, const String& prefix);
bool
string_ends_with(const String& str, const String& suffix);

void
string_replace(String& str, char from, char to);
void
string_replace(String& str, const char* from, const char* to);

bool
is_control(char ch);
bool
is_control_with_short_escape_sequence(char ch);
char*
str_n_cpy(char* s1, const char* s2, size_t n);
const char*
str_str(const char* s1, const char* s2);

void
pad_strings_to_same_length(String& str1, String& str2, char ch);
int
ato_i(const char* str);
unsigned
ato_u(const char* str);
int
str_cmp(const char* s1, const char* s2);
int
str_n_cmp(const char* s1, const char* s2, size_t n);
char
to_lower(char ch);
int
mem_cmp(const void* s1, const void* s2, size_t n);

String
string_from(bool value);

String
string_from(const void* value);

String
string_from(void (*value)());

String
string_from(char value);

String
string_from(const char* value);

String
string_from_or_null(const char* value);

String
string_from(int value);

String
string_from(unsigned int value);

String
string_from(long value);

String
string_from(unsigned long value);

String
string_from(long long value);

String
string_from(unsigned long long value);

String
hex_string_from(unsigned int value);

String
hex_string_from(int value);

String
hex_string_from(signed char value);

String
hex_string_from(long value);

String
hex_string_from(unsigned long value);

String
hex_string_from(long long value);

String
hex_string_from(unsigned long long value);

String
hex_string_from(const void* value);

String
hex_string_from(void (*value)());

String
string_from(double value, int precision = 6);

String
string_from(const String& other);

#if CPPMU_HAS_ATTRIBUTE(format)
#define CPPMU_CHECK_FORMAT(type, format_parameter, other_parameters)           \
  __attribute__((format(type, format_parameter, other_parameters)))
#else
#define CPPMU_CHECK_FORMAT(type,                                               \
    format_parameter,                                                          \
    other_parameters) /* type, format_parameter, other_parameters */
#endif
String
string_from_format(const char* format, ...)
    CPPMU_CHECK_FORMAT(CPPMU_CHECK_FORMAT_TYPE, 1, 2);

String
v_string_from_format(const char* format, va_list args);

String
string_from_binary(const unsigned char* value, size_t size);

String
string_from_binary_or_null(const unsigned char* value, size_t size);

String
string_from_binary_with_size(const unsigned char* value, size_t size);

String
string_from_binary_with_size_or_null(const unsigned char* value, size_t size);

String
string_from_ordinal_number(unsigned int number);

String
brackets_formatted_hex_string_from(int value);

String
brackets_formatted_hex_string_from(unsigned int value);

String
brackets_formatted_hex_string_from(long value);

String
brackets_formatted_hex_string_from(unsigned long value);

String
brackets_formatted_hex_string_from(long long value);

String
brackets_formatted_hex_string_from(unsigned long long value);

String
brackets_formatted_hex_string_from(signed char value);

String
brackets_formatted_hex_string(String hex_string);

#if CPPMU_USE_STD_CPP_LIB
String
string_from(const std::nullptr_t value);

String
string_from(const std::string& other);
#endif

} // namespace cppmu

#undef CPPMU_HAS_ATTRIBUTE
#undef CPPMU_CHECK_FORMAT_TYPE

#endif
