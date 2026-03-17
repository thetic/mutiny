#ifndef INCLUDED_CPPMU_MOCKCHECKEDEXPECTEDCALL_HPP
#define INCLUDED_CPPMU_MOCKCHECKEDEXPECTEDCALL_HPP

#include "CppMu/MockExpectedCall.hpp"
#include "CppMu/MockNamedValue.hpp"
#include "CppMu/MockNamedValueList.hpp"

namespace cppmu {

class MockCheckedExpectedCall : public MockExpectedCall
{

public:
  MockCheckedExpectedCall();
  MockCheckedExpectedCall(unsigned int num_calls);
  ~MockCheckedExpectedCall() override;

  MockExpectedCall& with_name(const cppmu::String& name) override;
  MockExpectedCall& with_call_order(unsigned int call_order) override
  {
    return with_call_order(call_order, call_order);
  }
  MockExpectedCall& with_call_order(
      unsigned int initial_call_order,
      unsigned int final_call_order
  ) override;
  MockExpectedCall& with_bool_parameter(
      const cppmu::String& name,
      bool value
  ) override;
  MockExpectedCall& with_int_parameter(
      const cppmu::String& name,
      int value
  ) override;
  MockExpectedCall& with_unsigned_int_parameter(
      const cppmu::String& name,
      unsigned int value
  ) override;
  MockExpectedCall& with_long_int_parameter(
      const cppmu::String& name,
      long int value
  ) override;
  MockExpectedCall& with_unsigned_long_int_parameter(
      const cppmu::String& name,
      unsigned long int value
  ) override;
  MockExpectedCall& with_long_long_int_parameter(
      const cppmu::String& name,
      long long value
  ) override;
  MockExpectedCall& with_unsigned_long_long_int_parameter(
      const cppmu::String& name,
      unsigned long long value
  ) override;
  MockExpectedCall& with_double_parameter(
      const cppmu::String& name,
      double value
  ) override;
  MockExpectedCall& with_double_parameter(
      const cppmu::String& name,
      double value,
      double tolerance
  ) override;
  MockExpectedCall& with_string_parameter(
      const cppmu::String& name,
      const char* value
  ) override;
  MockExpectedCall& with_pointer_parameter(
      const cppmu::String& name,
      void* value
  ) override;
  MockExpectedCall& with_const_pointer_parameter(
      const cppmu::String& name,
      const void* value
  ) override;
  MockExpectedCall& with_function_pointer_parameter(
      const cppmu::String& name,
      void (*value)()
  ) override;
  MockExpectedCall& with_memory_buffer_parameter(
      const cppmu::String& name,
      const unsigned char* value,
      size_t size
  ) override;
  MockExpectedCall& with_parameter_of_type(
      const cppmu::String& type_name,
      const cppmu::String& name,
      const void* value
  ) override;
  MockExpectedCall& with_output_parameter_returning(
      const cppmu::String& name,
      const void* value,
      size_t size
  ) override;
  MockExpectedCall& with_output_parameter_of_type_returning(
      const cppmu::String& type_name,
      const cppmu::String& name,
      const void* value
  ) override;
  MockExpectedCall& with_unmodified_output_parameter(
      const cppmu::String& name
  ) override;
  MockExpectedCall& ignore_other_parameters() override;

  MockExpectedCall& and_return_value(bool value) override;
  MockExpectedCall& and_return_value(int value) override;
  MockExpectedCall& and_return_value(unsigned int value) override;
  MockExpectedCall& and_return_value(long int value) override;
  MockExpectedCall& and_return_value(unsigned long int value) override;
  MockExpectedCall& and_return_value(long long value) override;
  MockExpectedCall& and_return_value(unsigned long long value) override;
  MockExpectedCall& and_return_value(double value) override;
  MockExpectedCall& and_return_value(const char* value) override;
  MockExpectedCall& and_return_value(void* value) override;
  MockExpectedCall& and_return_value(const void* value) override;
  MockExpectedCall& and_return_value(void (*value)()) override;

  virtual MockNamedValue return_value();

  MockExpectedCall& on_object(void* object_ptr) override;

  virtual MockNamedValue get_input_parameter(const cppmu::String& name);
  virtual MockNamedValue get_output_parameter(const cppmu::String& name);
  virtual cppmu::String get_input_parameter_type(const cppmu::String& name);
  virtual cppmu::String get_input_parameter_value_string(
      const cppmu::String& name
  );

  virtual bool has_input_parameter_with_name(const cppmu::String& name);
  virtual bool has_input_parameter(const MockNamedValue& parameter);
  virtual bool has_output_parameter_with_name(const cppmu::String& name);
  virtual bool has_output_parameter(const MockNamedValue& parameter);
  virtual bool relates_to(const cppmu::String& function_name);
  virtual bool relates_to_object(const void* object_ptr) const;

  virtual bool is_fulfilled();
  virtual bool can_match_actual_calls();
  virtual bool is_matching_actual_call_and_finalized();
  virtual bool is_matching_actual_call();
  virtual bool are_parameters_matching_actual_call();
  virtual bool is_out_of_order() const;

  virtual void call_was_made(unsigned int call_order);
  virtual void input_parameter_was_passed(const cppmu::String& name);
  virtual void output_parameter_was_passed(const cppmu::String& name);
  virtual void finalize_actual_call_match();
  virtual void was_passed_to_object();
  virtual void reset_actual_call_matching_state();

  virtual cppmu::String call_to_string();
  virtual cppmu::String missing_parameters_to_string();

  static constexpr unsigned int no_expected_call_order = 0;

  virtual unsigned int get_actual_calls_fulfilled() const;

protected:
  void set_name(const cppmu::String& name);
  cppmu::String get_name() const;

private:
  cppmu::String function_name_;

  class MockExpectedFunctionParameter : public MockNamedValue
  {
  public:
    MockExpectedFunctionParameter(const cppmu::String& name);
    void set_matches_actual_call(bool b);
    bool is_matching_actual_call() const;

  private:
    bool matches_actual_call_{ false };
  };

  MockExpectedFunctionParameter* item(MockNamedValueListNode* node);

  bool ignore_other_parameters_;
  bool is_actual_call_match_finalized_;
  unsigned int initial_expected_call_order_;
  unsigned int final_expected_call_order_;
  bool out_of_order_;
  MockNamedValueList* input_parameters_;
  MockNamedValueList* output_parameters_;
  MockNamedValue return_value_;
  void* object_ptr_;
  bool is_specific_object_expected_;
  bool was_passed_to_object_;
  unsigned int actual_calls_;
  unsigned int expected_calls_;
};

} // namespace cppmu

#endif
