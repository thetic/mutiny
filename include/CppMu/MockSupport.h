#ifndef INCLUDED_CPPMU_MOCKSUPPORT_H
#define INCLUDED_CPPMU_MOCKSUPPORT_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stddef.h>

  enum MockValueType
  {
    mockvaluetype_bool,
    mockvaluetype_unsigned_integer,
    mockvaluetype_integer,
    mockvaluetype_long_integer,
    mockvaluetype_unsigned_long_integer,
    mockvaluetype_long_long_integer,
    mockvaluetype_unsigned_long_long_integer,
    mockvaluetype_double,
    mockvaluetype_string,
    mockvaluetype_pointer,
    mockvaluetype_const_pointer,
    mockvaluetype_functionpointer,
    mockvaluetype_memorybuffer,
    mockvaluetype_object,
    mockvaluetype_const_object
  };

  struct MockValue
  {
    enum MockValueType type;
    union
    {
      int bool_value;
      int int_value;
      unsigned int unsigned_int_value;
      long int long_int_value;
      unsigned long int unsigned_long_int_value;
      long long long_long_int_value;
      unsigned long long unsigned_long_long_int_value;
      double double_value;
      const char* string_value;
      void* pointer_value;
      const void* const_pointer_value;
      void (*function_pointer_value)(void);
      const unsigned char* memory_buffer_value;
      void* object_value;
      const void* const_object_value;
    } value;
  };

  struct MockActualCall
  {
    struct MockActualCall* (*with_bool_parameters)(const char* name, int value);
    struct MockActualCall* (*with_int_parameters)(const char* name, int value);
    struct MockActualCall* (*with_unsigned_int_parameters)(
        const char* name,
        unsigned int value
    );
    struct MockActualCall* (*with_long_int_parameters)(
        const char* name,
        long int value
    );
    struct MockActualCall* (*with_unsigned_long_int_parameters)(
        const char* name,
        unsigned long int value
    );
    struct MockActualCall* (*with_long_long_int_parameters)(
        const char* name,
        long long value
    );
    struct MockActualCall* (*with_unsigned_long_long_int_parameters)(
        const char* name,
        unsigned long long value
    );
    struct MockActualCall* (*with_double_parameters)(
        const char* name,
        double value
    );
    struct MockActualCall* (*with_string_parameters)(
        const char* name,
        const char* value
    );
    struct MockActualCall* (*with_pointer_parameters)(
        const char* name,
        void* value
    );
    struct MockActualCall* (*with_const_pointer_parameters)(
        const char* name,
        const void* value
    );
    struct MockActualCall* (*with_function_pointer_parameters)(
        const char* name,
        void (*value)(void)
    );
    struct MockActualCall* (*with_memory_buffer_parameter)(
        const char* name,
        const unsigned char* value,
        size_t size
    );
    struct MockActualCall* (*with_parameter_of_type)(
        const char* type,
        const char* name,
        const void* value
    );
    struct MockActualCall* (*with_output_parameter)(
        const char* name,
        void* value
    );
    struct MockActualCall* (*with_output_parameter_of_type)(
        const char* type,
        const char* name,
        void* value
    );
    int (*has_return_value)(void);
    struct MockValue (*return_value)(void);
    int (*bool_return_value)(void);
    int (*return_bool_value_or_default)(int default_value);
    int (*int_return_value)(void);
    int (*return_int_value_or_default)(int default_value);
    unsigned int (*unsigned_int_return_value)(void);
    unsigned int (*return_unsigned_int_value_or_default)(
        unsigned int default_value
    );
    long int (*long_int_return_value)(void);
    long int (*return_long_int_value_or_default)(long int default_value);
    unsigned long int (*unsigned_long_int_return_value)(void);
    unsigned long int (*return_unsigned_long_int_value_or_default)(
        unsigned long int default_value
    );
    long long (*long_long_int_return_value)(void);
    long long (*return_long_long_int_value_or_default)(long long default_value);
    unsigned long long (*unsigned_long_long_int_return_value)(void);
    unsigned long long (*return_unsigned_long_long_int_value_or_default)(
        unsigned long long default_value
    );
    const char* (*string_return_value)(void);
    const char* (*return_string_value_or_default)(const char* default_value);
    double (*double_return_value)(void);
    double (*return_double_value_or_default)(double default_value);
    void* (*pointer_return_value)(void);
    void* (*return_pointer_value_or_default)(void* default_value);
    const void* (*const_pointer_return_value)(void);
    const void* (*return_const_pointer_value_or_default)(
        const void* default_value
    );
    void (*(*function_pointer_return_value)(void))(void);
    void (*(*return_function_pointer_value_or_default)(void (*default_value)(
        void
    )))(void);
  };

  struct MockExpectedCall
  {
    struct MockExpectedCall* (*with_bool_parameters)(
        const char* name,
        int value
    );
    struct MockExpectedCall* (*with_int_parameters)(
        const char* name,
        int value
    );
    struct MockExpectedCall* (*with_unsigned_int_parameters)(
        const char* name,
        unsigned int value
    );
    struct MockExpectedCall* (*with_long_int_parameters)(
        const char* name,
        long int value
    );
    struct MockExpectedCall* (*with_unsigned_long_int_parameters)(
        const char* name,
        unsigned long int value
    );
    struct MockExpectedCall* (*with_long_long_int_parameters)(
        const char* name,
        long long value
    );
    struct MockExpectedCall* (*with_unsigned_long_long_int_parameters)(
        const char* name,
        unsigned long long value
    );
    struct MockExpectedCall* (*with_double_parameters)(
        const char* name,
        double value
    );
    struct MockExpectedCall* (*with_double_parameters_and_tolerance)(
        const char* name,
        double value,
        double tolerance
    );
    struct MockExpectedCall* (*with_string_parameters)(
        const char* name,
        const char* value
    );
    struct MockExpectedCall* (*with_pointer_parameters)(
        const char* name,
        void* value
    );
    struct MockExpectedCall* (*with_const_pointer_parameters)(
        const char* name,
        const void* value
    );
    struct MockExpectedCall* (*with_function_pointer_parameters)(
        const char* name,
        void (*value)(void)
    );
    struct MockExpectedCall* (*with_memory_buffer_parameter)(
        const char* name,
        const unsigned char* value,
        size_t size
    );
    struct MockExpectedCall* (*with_parameter_of_type)(
        const char* type,
        const char* name,
        const void* value
    );
    struct MockExpectedCall* (*with_output_parameter_returning)(
        const char* name,
        const void* value,
        size_t size
    );
    struct MockExpectedCall* (*with_output_parameter_of_type_returning)(
        const char* type,
        const char* name,
        const void* value
    );
    struct MockExpectedCall* (*with_unmodified_output_parameter)(
        const char* name
    );
    struct MockExpectedCall* (*ignore_other_parameters)(void);

    struct MockExpectedCall* (*and_return_bool_value)(int value);
    struct MockExpectedCall* (*and_return_unsigned_int_value)(
        unsigned int value
    );
    struct MockExpectedCall* (*and_return_int_value)(int value);
    struct MockExpectedCall* (*and_return_long_int_value)(long int value);
    struct MockExpectedCall* (*and_return_unsigned_long_int_value)(
        unsigned long int value
    );
    struct MockExpectedCall* (*and_return_long_long_int_value)(long long value);
    struct MockExpectedCall* (*and_return_unsigned_long_long_int_value)(
        unsigned long long value
    );
    struct MockExpectedCall* (*and_return_double_value)(double value);
    struct MockExpectedCall* (*and_return_string_value)(const char* value);
    struct MockExpectedCall* (*and_return_pointer_value)(void* value);
    struct MockExpectedCall* (*and_return_const_pointer_value)(
        const void* value
    );
    struct MockExpectedCall* (*and_return_function_pointer_value)(
        void (*value)(void)
    );
  };

  typedef int (*MockTypeEqualFunction)(
      const void* object1,
      const void* object2
  );
  typedef const char* (*MockTypeValueToStringFunction)(const void* object1);
  typedef void (*MockTypeCopyFunction)(void* dst, const void* src);

  struct MockSupport
  {
    void (*strict_order)(void);
    struct MockExpectedCall* (*expect_one_call)(const char* name);
    void (*expect_no_call)(const char* name);
    struct MockExpectedCall* (*expect_n_calls)(
        unsigned int number,
        const char* name
    );
    struct MockActualCall* (*actual_call)(const char* name);
    int (*has_return_value)(void);
    struct MockValue (*return_value)(void);
    int (*bool_return_value)(void);
    int (*return_bool_value_or_default)(int default_value);
    int (*int_return_value)(void);
    int (*return_int_value_or_default)(int default_value);
    unsigned int (*unsigned_int_return_value)(void);
    unsigned int (*return_unsigned_int_value_or_default)(
        unsigned int default_value
    );
    long int (*long_int_return_value)(void);
    long int (*return_long_int_value_or_default)(long int default_value);
    unsigned long int (*unsigned_long_int_return_value)(void);
    unsigned long int (*return_unsigned_long_int_value_or_default)(
        unsigned long int default_value
    );
    long long (*long_long_int_return_value)(void);
    long long (*return_long_long_int_value_or_default)(long long default_value);
    unsigned long long (*unsigned_long_long_int_return_value)(void);
    unsigned long long (*return_unsigned_long_long_int_value_or_default)(
        unsigned long long default_value
    );
    const char* (*string_return_value)(void);
    const char* (*return_string_value_or_default)(const char* default_value);
    double (*double_return_value)(void);
    double (*return_double_value_or_default)(double default_value);
    void* (*pointer_return_value)(void);
    void* (*return_pointer_value_or_default)(void* default_value);
    const void* (*const_pointer_return_value)(void);
    const void* (*return_const_pointer_value_or_default)(
        const void* default_value
    );
    void (*(*function_pointer_return_value)(void))(void);
    void (*(*return_function_pointer_value_or_default)(void (*default_value)(
        void
    )))(void);

    void (*set_bool_data)(const char* name, int value);
    void (*set_int_data)(const char* name, int value);
    void (*set_unsigned_int_data)(const char* name, unsigned int value);
    void (*set_long_int_data)(const char* name, long int value);
    void (*set_unsigned_long_int_data)(
        const char* name,
        unsigned long int value
    );
    void (*set_string_data)(const char* name, const char* value);
    void (*set_double_data)(const char* name, double value);
    void (*set_pointer_data)(const char* name, void* value);
    void (*set_const_pointer_data)(const char* name, const void* value);
    void (*set_function_pointer_data)(const char* name, void (*value)(void));
    void (*set_data_object)(const char* name, const char* type, void* value);
    void (*set_data_const_object)(
        const char* name,
        const char* type,
        const void* value
    );
    struct MockValue (*get_data)(const char* name);

    void (*disable)(void);
    void (*enable)(void);
    void (*ignore_other_calls)(void);

    void (*check_expectations)(void);
    int (*expected_calls_left)(void);

    void (*clear)(void);
    void (*crash_on_failure)(unsigned should_crash);

    void (*install_comparator)(
        const char* type_name,
        MockTypeEqualFunction is_equal,
        MockTypeValueToStringFunction value_to_string
    );
    void (*install_copier)(const char* type_name, MockTypeCopyFunction copier);
    void (*remove_all_comparators_and_copiers)(void);
  };

  struct MockSupport* mock(void);
  struct MockSupport* mock_scope(const char* scope);

#ifdef __cplusplus
}
#endif

#endif
