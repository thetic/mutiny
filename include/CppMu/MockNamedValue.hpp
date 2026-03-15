#ifndef INCLUDED_CPPMU_MOCKNAMEDVALUE_HPP
#define INCLUDED_CPPMU_MOCKNAMEDVALUE_HPP

#include "CppMu/MockNamedValueComparator.hpp"
#include "CppMu/MockNamedValueCopier.hpp"

namespace cppmu {

/*
 * MockNamedValue is the generic value class used. It encapsulates basic types
 * and can use them "as if one" Also it enables other types by putting object
 * pointers. They can be compared with comparators.
 *
 * Basically this class ties together a Name, a Value, a Type, and a Comparator
 */

class MockNamedValueComparatorsAndCopiersRepository;
class MockNamedValue
{
public:
  MockNamedValue(const cppmu::String& name);
  MockNamedValue(const MockNamedValue&) = default;
  MockNamedValue(MockNamedValue&&) noexcept;
  virtual ~MockNamedValue();

  virtual void set_value(bool value);
  virtual void set_value(int value);
  virtual void set_value(unsigned int value);
  virtual void set_value(long int value);
  virtual void set_value(unsigned long int value);
  virtual void set_value(long long value);
  virtual void set_value(unsigned long long value);
  virtual void set_value(double value);
  virtual void set_value(double value, double tolerance);
  virtual void set_value(void* value);
  virtual void set_value(const void* value);
  virtual void set_value(void (*value)());
  virtual void set_value(const char* value);
  virtual void set_memory_buffer(const unsigned char* value, size_t size);
  virtual void set_const_object_pointer(const cppmu::String& type,
      const void* object_ptr);
  virtual void set_object_pointer(const cppmu::String& type, void* object_ptr);
  virtual void set_size(size_t size);

  virtual void set_name(const char* name);

  virtual bool equals(const MockNamedValue& p) const;
  virtual bool compatible_for_copying(const MockNamedValue& p) const;

  virtual cppmu::String to_string() const;

  virtual cppmu::String get_name() const;
  virtual cppmu::String get_type() const;

  virtual bool get_bool_value() const;
  virtual int get_int_value() const;
  virtual unsigned int get_unsigned_int_value() const;
  virtual long int get_long_int_value() const;
  virtual unsigned long int get_unsigned_long_int_value() const;
  virtual long long get_long_long_int_value() const;
  virtual unsigned long long get_unsigned_long_long_int_value() const;
  virtual double get_double_value() const;
  virtual double get_double_tolerance() const;
  virtual const char* get_string_value() const;
  virtual void* get_pointer_value() const;
  virtual const void* get_const_pointer_value() const;
  virtual void (*get_function_pointer_value() const)();
  virtual const unsigned char* get_memory_buffer() const;
  virtual const void* get_const_object_pointer() const;
  virtual void* get_object_pointer() const;
  virtual bool is_const_object() const;
  virtual size_t get_size() const;

  virtual MockNamedValueComparator* get_comparator() const;
  virtual MockNamedValueCopier* get_copier() const;

  static void set_default_comparators_and_copiers_repository(
      MockNamedValueComparatorsAndCopiersRepository* repository);
  static MockNamedValueComparatorsAndCopiersRepository*
  get_default_comparators_and_copiers_repository();

  static const double default_double_tolerance;

private:
  cppmu::String name_;
  cppmu::String type_;
  bool is_const_object_;
  union
  {
    bool bool_value;
    int int_value;
    unsigned int unsigned_int_value;
    long int long_int_value;
    unsigned long int unsigned_long_int_value;
    long long long_long_int_value;
    unsigned long long unsigned_long_long_int_value;
    struct
    {
      double value;
      double tolerance;
    } double_value;
    const char* string_value;
    void* pointer_value;
    const void* const_pointer_value;
    void (*function_pointer_value)();
    const unsigned char* memory_buffer_value;
    const void* const_object_pointer_value;
    void* object_pointer_value;
    const void* output_pointer_value;
  } value_;
  size_t size_;
  MockNamedValueComparator* comparator_;
  MockNamedValueCopier* copier_;
  static MockNamedValueComparatorsAndCopiersRepository* default_repository_;
};

} // namespace cppmu

#endif
