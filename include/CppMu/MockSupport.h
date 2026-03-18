#ifndef INCLUDED_CPPMU_MOCKSUPPORT_H
#define INCLUDED_CPPMU_MOCKSUPPORT_H

#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

  enum CppMuMockValueType
  {
    cppmu_type_bool,
    cppmu_type_unsigned_integer,
    cppmu_type_integer,
    cppmu_type_long_integer,
    cppmu_type_unsigned_long_integer,
    cppmu_type_long_long_integer,
    cppmu_type_unsigned_long_long_integer,
    cppmu_type_double,
    cppmu_type_string,
    cppmu_type_pointer,
    cppmu_type_const_pointer,
    cppmu_type_functionpointer,
    cppmu_type_memorybuffer,
    cppmu_type_object,
    cppmu_type_const_object
  };

  struct CppMuMockValue
  {
    enum CppMuMockValueType type;
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

  struct CppMuMockActualCall
  {
    struct CppMuMockActualCall* (*with_bool_parameters)(
        const char* name,
        int value
    );
    struct CppMuMockActualCall* (*with_int_parameters)(
        const char* name,
        int value
    );
    struct CppMuMockActualCall* (*with_unsigned_int_parameters)(
        const char* name,
        unsigned int value
    );
    struct CppMuMockActualCall* (*with_long_int_parameters)(
        const char* name,
        long int value
    );
    struct CppMuMockActualCall* (*with_unsigned_long_int_parameters)(
        const char* name,
        unsigned long int value
    );
    struct CppMuMockActualCall* (*with_long_long_int_parameters)(
        const char* name,
        long long value
    );
    struct CppMuMockActualCall* (*with_unsigned_long_long_int_parameters)(
        const char* name,
        unsigned long long value
    );
    struct CppMuMockActualCall* (*with_double_parameters)(
        const char* name,
        double value
    );
    struct CppMuMockActualCall* (*with_string_parameters)(
        const char* name,
        const char* value
    );
    struct CppMuMockActualCall* (*with_pointer_parameters)(
        const char* name,
        void* value
    );
    struct CppMuMockActualCall* (*with_const_pointer_parameters)(
        const char* name,
        const void* value
    );
    struct CppMuMockActualCall* (*with_function_pointer_parameters)(
        const char* name,
        void (*value)(void)
    );
    struct CppMuMockActualCall* (*with_memory_buffer_parameter)(
        const char* name,
        const unsigned char* value,
        size_t size
    );
    struct CppMuMockActualCall* (*with_parameter_of_type)(
        const char* type,
        const char* name,
        const void* value
    );
    struct CppMuMockActualCall* (*with_output_parameter)(
        const char* name,
        void* value
    );
    struct CppMuMockActualCall* (*with_output_parameter_of_type)(
        const char* type,
        const char* name,
        void* value
    );
    int (*has_return_value)(void);
    struct CppMuMockValue (*return_value)(void);
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

  struct CppMuMockExpectedCall
  {
    struct CppMuMockExpectedCall* (*with_bool_parameters)(
        const char* name,
        int value
    );
    struct CppMuMockExpectedCall* (*with_int_parameters)(
        const char* name,
        int value
    );
    struct CppMuMockExpectedCall* (*with_unsigned_int_parameters)(
        const char* name,
        unsigned int value
    );
    struct CppMuMockExpectedCall* (*with_long_int_parameters)(
        const char* name,
        long int value
    );
    struct CppMuMockExpectedCall* (*with_unsigned_long_int_parameters)(
        const char* name,
        unsigned long int value
    );
    struct CppMuMockExpectedCall* (*with_long_long_int_parameters)(
        const char* name,
        long long value
    );
    struct CppMuMockExpectedCall* (*with_unsigned_long_long_int_parameters)(
        const char* name,
        unsigned long long value
    );
    struct CppMuMockExpectedCall* (*with_double_parameters)(
        const char* name,
        double value
    );
    struct CppMuMockExpectedCall* (*with_double_parameters_and_tolerance)(
        const char* name,
        double value,
        double tolerance
    );
    struct CppMuMockExpectedCall* (*with_string_parameters)(
        const char* name,
        const char* value
    );
    struct CppMuMockExpectedCall* (*with_pointer_parameters)(
        const char* name,
        void* value
    );
    struct CppMuMockExpectedCall* (*with_const_pointer_parameters)(
        const char* name,
        const void* value
    );
    struct CppMuMockExpectedCall* (*with_function_pointer_parameters)(
        const char* name,
        void (*value)(void)
    );
    struct CppMuMockExpectedCall* (*with_memory_buffer_parameter)(
        const char* name,
        const unsigned char* value,
        size_t size
    );
    struct CppMuMockExpectedCall* (*with_parameter_of_type)(
        const char* type,
        const char* name,
        const void* value
    );
    struct CppMuMockExpectedCall* (*with_output_parameter_returning)(
        const char* name,
        const void* value,
        size_t size
    );
    struct CppMuMockExpectedCall* (*with_output_parameter_of_type_returning)(
        const char* type,
        const char* name,
        const void* value
    );
    struct CppMuMockExpectedCall* (*with_unmodified_output_parameter)(
        const char* name
    );
    struct CppMuMockExpectedCall* (*ignore_other_parameters)(void);

    struct CppMuMockExpectedCall* (*and_return_bool_value)(int value);
    struct CppMuMockExpectedCall* (*and_return_unsigned_int_value)(
        unsigned int value
    );
    struct CppMuMockExpectedCall* (*and_return_int_value)(int value);
    struct CppMuMockExpectedCall* (*and_return_long_int_value)(long int value);
    struct CppMuMockExpectedCall* (*and_return_unsigned_long_int_value)(
        unsigned long int value
    );
    struct CppMuMockExpectedCall* (*and_return_long_long_int_value)(
        long long value
    );
    struct CppMuMockExpectedCall* (*and_return_unsigned_long_long_int_value)(
        unsigned long long value
    );
    struct CppMuMockExpectedCall* (*and_return_double_value)(double value);
    struct CppMuMockExpectedCall* (*and_return_string_value)(const char* value);
    struct CppMuMockExpectedCall* (*and_return_pointer_value)(void* value);
    struct CppMuMockExpectedCall* (*and_return_const_pointer_value)(
        const void* value
    );
    struct CppMuMockExpectedCall* (*and_return_function_pointer_value)(
        void (*value)(void)
    );
  };

  typedef int (*CppMuMockTypeEqualFunction)(
      const void* object1,
      const void* object2
  );
  typedef const char* (*CppMuMockTypeValueToStringFunction)(
      const void* object1
  );
  typedef void (*CppMuMockTypeCopyFunction)(void* dst, const void* src);

  struct CppMuMockSupport
  {
    void (*strict_order)(void);
    struct CppMuMockExpectedCall* (*expect_one_call)(const char* name);
    void (*expect_no_call)(const char* name);
    struct CppMuMockExpectedCall* (*expect_n_calls)(
        unsigned int number,
        const char* name
    );
    struct CppMuMockActualCall* (*actual_call)(const char* name);
    int (*has_return_value)(void);
    struct CppMuMockValue (*return_value)(void);
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
    struct CppMuMockValue (*get_data)(const char* name);

    void (*disable)(void);
    void (*enable)(void);
    void (*ignore_other_calls)(void);

    void (*check_expectations)(void);
    int (*expected_calls_left)(void);

    void (*clear)(void);
    void (*crash_on_failure)(unsigned should_crash);

    void (*install_comparator)(
        const char* type_name,
        CppMuMockTypeEqualFunction is_equal,
        CppMuMockTypeValueToStringFunction value_to_string
    );
    void (*install_copier)(
        const char* type_name,
        CppMuMockTypeCopyFunction copier
    );
    void (*remove_all_comparators_and_copiers)(void);
  };

  struct CppMuMockSupport* cppmu_mock(void);
  struct CppMuMockSupport* cppmu_mock_scope(const char* scope);

#ifdef __cplusplus
}
#endif

#endif
