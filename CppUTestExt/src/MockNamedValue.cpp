#include "CppUTestExt/MockNamedValue.hpp"

#include "CppUTestExt/MockNamedValueComparatorsAndCopiersRepository.hpp"

#include "CppUTest/PlatformSpecificFunctions.hpp"
#include "CppUTest/TestHarness.hpp"

namespace cpputest {
namespace extensions {

MockNamedValueComparatorsAndCopiersRepository*
    MockNamedValue::defaultRepository_ = nullptr;
const double MockNamedValue::defaultDoubleTolerance = 0.005;

void
MockNamedValue::setDefaultComparatorsAndCopiersRepository(
    MockNamedValueComparatorsAndCopiersRepository* repository)
{
  defaultRepository_ = repository;
}

MockNamedValueComparatorsAndCopiersRepository*
MockNamedValue::getDefaultComparatorsAndCopiersRepository()
{
  return defaultRepository_;
}

MockNamedValue::MockNamedValue(const String& name)
  : name_(name)
  , type_("int")
  , isConstObject_(false)
  , size_(0)
  , comparator_(nullptr)
  , copier_(nullptr)
{
  value_.intValue_ = 0;
}

MockNamedValue::MockNamedValue(MockNamedValue&& other) noexcept
  : name_(static_cast<String&&>(other.name_))
  , type_(static_cast<String&&>(other.type_))
  , isConstObject_(other.isConstObject_)
  , value_(other.value_)
  , size_(other.size_)
  , comparator_(other.comparator_)
  , copier_(other.copier_)
{
}

MockNamedValue::~MockNamedValue() {}

void
MockNamedValue::setValue(bool value)
{
  type_ = "bool";
  value_.boolValue_ = value;
}

void
MockNamedValue::setValue(unsigned int value)
{
  type_ = "unsigned int";
  value_.unsignedIntValue_ = value;
}

void
MockNamedValue::setValue(int value)
{
  type_ = "int";
  value_.intValue_ = value;
}

void
MockNamedValue::setValue(long int value)
{
  type_ = "long int";
  value_.longIntValue_ = value;
}

void
MockNamedValue::setValue(unsigned long int value)
{
  type_ = "unsigned long int";
  value_.unsignedLongIntValue_ = value;
}

void
MockNamedValue::setValue(long long value)
{
  type_ = "long long int";
  value_.longLongIntValue_ = value;
}

void
MockNamedValue::setValue(unsigned long long value)
{
  type_ = "unsigned long long int";
  value_.unsignedLongLongIntValue_ = value;
}

void
MockNamedValue::setValue(double value)
{
  setValue(value, defaultDoubleTolerance);
}

void
MockNamedValue::setValue(double value, double tolerance)
{
  type_ = "double";
  value_.doubleValue_.value = value;
  value_.doubleValue_.tolerance = tolerance;
}

void
MockNamedValue::setValue(void* value)
{
  type_ = "void*";
  value_.pointerValue_ = value;
}

void
MockNamedValue::setValue(const void* value)
{
  type_ = "const void*";
  value_.constPointerValue_ = value;
}

void
MockNamedValue::setValue(void (*value)())
{
  type_ = "void (*)()";
  value_.functionPointerValue_ = value;
}

void
MockNamedValue::setValue(const char* value)
{
  type_ = "const char*";
  value_.stringValue_ = value;
}

void
MockNamedValue::setMemoryBuffer(const unsigned char* value, size_t size)
{
  type_ = "const unsigned char*";
  value_.memoryBufferValue_ = value;
  size_ = size;
}

void
MockNamedValue::setConstObjectPointer(const String& type, const void* objectPtr)
{
  type_ = type;
  isConstObject_ = true;
  value_.constObjectPointerValue_ = objectPtr;
  if (defaultRepository_) {
    comparator_ = defaultRepository_->getComparatorForType(type);
    copier_ = defaultRepository_->getCopierForType(type);
  }
}

void
MockNamedValue::setObjectPointer(const String& type, void* objectPtr)
{
  type_ = type;
  isConstObject_ = false;
  value_.objectPointerValue_ = objectPtr;
  if (defaultRepository_) {
    comparator_ = defaultRepository_->getComparatorForType(type);
    copier_ = defaultRepository_->getCopierForType(type);
  }
}

void
MockNamedValue::setSize(size_t size)
{
  size_ = size;
}

void
MockNamedValue::setName(const char* name)
{
  name_ = name;
}

String
MockNamedValue::getName() const
{
  return name_;
}

String
MockNamedValue::getType() const
{
  return type_;
}

bool
MockNamedValue::getBoolValue() const
{
  STRCMP_EQUAL("bool", type_.c_str());
  return value_.boolValue_;
}

unsigned int
MockNamedValue::getUnsignedIntValue() const
{
  if (type_ == "int" && value_.intValue_ >= 0)
    return static_cast<unsigned int>(value_.intValue_);
  else {
    STRCMP_EQUAL("unsigned int", type_.c_str());
    return value_.unsignedIntValue_;
  }
}

int
MockNamedValue::getIntValue() const
{
  STRCMP_EQUAL("int", type_.c_str());
  return value_.intValue_;
}

long int
MockNamedValue::getLongIntValue() const
{
  if (type_ == "int")
    return value_.intValue_;
  else if (type_ == "unsigned int")
    return static_cast<long int>(value_.unsignedIntValue_);
  else {
    STRCMP_EQUAL("long int", type_.c_str());
    return value_.longIntValue_;
  }
}

unsigned long int
MockNamedValue::getUnsignedLongIntValue() const
{
  if (type_ == "unsigned int")
    return value_.unsignedIntValue_;
  else if (type_ == "int" && value_.intValue_ >= 0)
    return static_cast<unsigned long int>(value_.intValue_);
  else if (type_ == "long int" && value_.longIntValue_ >= 0)
    return static_cast<unsigned long int>(value_.longIntValue_);
  else {
    STRCMP_EQUAL("unsigned long int", type_.c_str());
    return value_.unsignedLongIntValue_;
  }
}

long long
MockNamedValue::getLongLongIntValue() const
{
  if (type_ == "int")
    return value_.intValue_;
  else if (type_ == "unsigned int")
    return static_cast<long long int>(value_.unsignedIntValue_);
  else if (type_ == "long int")
    return value_.longIntValue_;
  else if (type_ == "unsigned long int")
    return static_cast<long long int>(value_.unsignedLongIntValue_);
  else {
    STRCMP_EQUAL("long long int", type_.c_str());
    return value_.longLongIntValue_;
  }
}

unsigned long long
MockNamedValue::getUnsignedLongLongIntValue() const
{
  if (type_ == "unsigned int")
    return value_.unsignedIntValue_;
  else if (type_ == "int" && value_.intValue_ >= 0)
    return static_cast<unsigned long long int>(value_.intValue_);
  else if (type_ == "long int" && value_.longIntValue_ >= 0)
    return static_cast<unsigned long long int>(value_.longIntValue_);
  else if (type_ == "unsigned long int")
    return value_.unsignedLongIntValue_;
  else if (type_ == "long long int" && value_.longLongIntValue_ >= 0)
    return static_cast<unsigned long long int>(value_.longLongIntValue_);
  else {
    STRCMP_EQUAL("unsigned long long int", type_.c_str());
    return value_.unsignedLongLongIntValue_;
  }
}

double
MockNamedValue::getDoubleValue() const
{
  STRCMP_EQUAL("double", type_.c_str());
  return value_.doubleValue_.value;
}

double
MockNamedValue::getDoubleTolerance() const
{
  STRCMP_EQUAL("double", type_.c_str());
  return value_.doubleValue_.tolerance;
}

const char*
MockNamedValue::getStringValue() const
{
  STRCMP_EQUAL("const char*", type_.c_str());
  return value_.stringValue_;
}

void*
MockNamedValue::getPointerValue() const
{
  STRCMP_EQUAL("void*", type_.c_str());
  return value_.pointerValue_;
}

const void*
MockNamedValue::getConstPointerValue() const
{
  STRCMP_EQUAL("const void*", type_.c_str());
  return value_.pointerValue_;
}

void (*MockNamedValue::getFunctionPointerValue() const)()
{
  STRCMP_EQUAL("void (*)()", type_.c_str());
  return value_.functionPointerValue_;
}

const unsigned char*
MockNamedValue::getMemoryBuffer() const
{
  STRCMP_EQUAL("const unsigned char*", type_.c_str());
  return value_.memoryBufferValue_;
}

const void*
MockNamedValue::getConstObjectPointer() const
{
  return value_.constObjectPointerValue_;
}

void*
MockNamedValue::getObjectPointer() const
{
  return value_.objectPointerValue_;
}

bool
MockNamedValue::isConstObject() const
{
  return isConstObject_;
}

size_t
MockNamedValue::getSize() const
{
  return size_;
}

MockNamedValueComparator*
MockNamedValue::getComparator() const
{
  return comparator_;
}

MockNamedValueCopier*
MockNamedValue::getCopier() const
{
  return copier_;
}

bool
MockNamedValue::equals(const MockNamedValue& p) const
{
  if ((type_ == "long int") && (p.type_ == "int"))
    return value_.longIntValue_ == p.value_.intValue_;
  else if ((type_ == "int") && (p.type_ == "long int"))
    return value_.intValue_ == p.value_.longIntValue_;
  else if ((type_ == "unsigned int") && (p.type_ == "int"))
    return (p.value_.intValue_ >= 0) &&
           (value_.unsignedIntValue_ ==
               static_cast<unsigned int>(p.value_.intValue_));
  else if ((type_ == "int") && (p.type_ == "unsigned int"))
    return (value_.intValue_ >= 0) &&
           (static_cast<unsigned int>(value_.intValue_) ==
               p.value_.unsignedIntValue_);
  else if ((type_ == "unsigned long int") && (p.type_ == "int"))
    return (p.value_.intValue_ >= 0) &&
           (value_.unsignedLongIntValue_ ==
               static_cast<unsigned long>(p.value_.intValue_));
  else if ((type_ == "int") && (p.type_ == "unsigned long int"))
    return (value_.intValue_ >= 0) &&
           (static_cast<unsigned long>(value_.intValue_) ==
               p.value_.unsignedLongIntValue_);
  else if ((type_ == "unsigned int") && (p.type_ == "long int"))
    return (p.value_.longIntValue_ >= 0) &&
           (value_.unsignedIntValue_ ==
               static_cast<unsigned long>(p.value_.longIntValue_));
  else if ((type_ == "long int") && (p.type_ == "unsigned int"))
    return (value_.longIntValue_ >= 0) &&
           (static_cast<unsigned long>(value_.longIntValue_) ==
               p.value_.unsignedIntValue_);
  else if ((type_ == "unsigned int") && (p.type_ == "unsigned long int"))
    return value_.unsignedIntValue_ == p.value_.unsignedLongIntValue_;
  else if ((type_ == "unsigned long int") && (p.type_ == "unsigned int"))
    return value_.unsignedLongIntValue_ == p.value_.unsignedIntValue_;
  else if ((type_ == "long int") && (p.type_ == "unsigned long int"))
    return (value_.longIntValue_ >= 0) &&
           (static_cast<unsigned long>(value_.longIntValue_) ==
               p.value_.unsignedLongIntValue_);
  else if ((type_ == "unsigned long int") && (p.type_ == "long int"))
    return (p.value_.longIntValue_ >= 0) &&
           (value_.unsignedLongIntValue_ ==
               static_cast<unsigned long>(p.value_.longIntValue_));
  else if ((type_ == "long long int") && (p.type_ == "int"))
    return value_.longLongIntValue_ == p.value_.intValue_;
  else if ((type_ == "int") && (p.type_ == "long long int"))
    return value_.intValue_ == p.value_.longLongIntValue_;
  else if ((type_ == "long long int") && (p.type_ == "long int"))
    return value_.longLongIntValue_ == p.value_.longIntValue_;
  else if ((type_ == "long int") && (p.type_ == "long long int"))
    return value_.longIntValue_ == p.value_.longLongIntValue_;
  else if ((type_ == "long long int") && (p.type_ == "unsigned int"))
    return (value_.longLongIntValue_ >= 0) &&
           (static_cast<unsigned long long>(value_.longLongIntValue_) ==
               p.value_.unsignedIntValue_);
  else if ((type_ == "unsigned int") && (p.type_ == "long long int"))
    return (p.value_.longLongIntValue_ >= 0) &&
           (value_.unsignedIntValue_ ==
               static_cast<unsigned long long>(p.value_.longLongIntValue_));
  else if ((type_ == "long long int") && (p.type_ == "unsigned long int"))
    return (value_.longLongIntValue_ >= 0) &&
           (static_cast<unsigned long long>(value_.longLongIntValue_) ==
               p.value_.unsignedLongIntValue_);
  else if ((type_ == "unsigned long int") && (p.type_ == "long long int"))
    return (p.value_.longLongIntValue_ >= 0) &&
           (value_.unsignedLongIntValue_ ==
               static_cast<unsigned long long>(p.value_.longLongIntValue_));
  else if ((type_ == "long long int") && (p.type_ == "unsigned long long int"))
    return (value_.longLongIntValue_ >= 0) &&
           (static_cast<unsigned long long>(value_.longLongIntValue_) ==
               p.value_.unsignedLongLongIntValue_);
  else if ((type_ == "unsigned long long int") && (p.type_ == "long long int"))
    return (p.value_.longLongIntValue_ >= 0) &&
           (value_.unsignedLongLongIntValue_ ==
               static_cast<unsigned long long>(p.value_.longLongIntValue_));
  else if ((type_ == "unsigned long long int") && (p.type_ == "int"))
    return (p.value_.intValue_ >= 0) &&
           (value_.unsignedLongLongIntValue_ ==
               static_cast<unsigned long long>(p.value_.intValue_));
  else if ((type_ == "int") && (p.type_ == "unsigned long long int"))
    return (value_.intValue_ >= 0) &&
           (static_cast<unsigned long long>(value_.intValue_) ==
               p.value_.unsignedLongLongIntValue_);
  else if ((type_ == "unsigned long long int") && (p.type_ == "unsigned int"))
    return value_.unsignedLongLongIntValue_ == p.value_.unsignedIntValue_;
  else if ((type_ == "unsigned int") && (p.type_ == "unsigned long long int"))
    return value_.unsignedIntValue_ == p.value_.unsignedLongLongIntValue_;
  else if ((type_ == "unsigned long long int") && (p.type_ == "long int"))
    return (p.value_.longIntValue_ >= 0) &&
           (value_.unsignedLongLongIntValue_ ==
               static_cast<unsigned long long>(p.value_.longIntValue_));
  else if ((type_ == "long int") && (p.type_ == "unsigned long long int"))
    return (value_.longIntValue_ >= 0) &&
           (static_cast<unsigned long long>(value_.longIntValue_) ==
               p.value_.unsignedLongLongIntValue_);
  else if ((type_ == "unsigned long long int") &&
           (p.type_ == "unsigned long int"))
    return value_.unsignedLongLongIntValue_ == p.value_.unsignedLongIntValue_;
  else if ((type_ == "unsigned long int") &&
           (p.type_ == "unsigned long long int"))
    return value_.unsignedLongIntValue_ == p.value_.unsignedLongLongIntValue_;

  if (type_ != p.type_)
    return false;

  if (type_ == "bool")
    return value_.boolValue_ == p.value_.boolValue_;
  else if (type_ == "int")
    return value_.intValue_ == p.value_.intValue_;
  else if (type_ == "unsigned int")
    return value_.unsignedIntValue_ == p.value_.unsignedIntValue_;
  else if (type_ == "long int")
    return value_.longIntValue_ == p.value_.longIntValue_;
  else if (type_ == "unsigned long int")
    return value_.unsignedLongIntValue_ == p.value_.unsignedLongIntValue_;
  else if (type_ == "long long int")
    return value_.longLongIntValue_ == p.value_.longLongIntValue_;
  else if (type_ == "unsigned long long int")
    return value_.unsignedLongLongIntValue_ ==
           p.value_.unsignedLongLongIntValue_;
  else if (type_ == "const char*")
    return String(value_.stringValue_) == String(p.value_.stringValue_);
  else if (type_ == "void*")
    return value_.pointerValue_ == p.value_.pointerValue_;
  else if (type_ == "const void*")
    return value_.constPointerValue_ == p.value_.constPointerValue_;
  else if (type_ == "void (*)()")
    return value_.functionPointerValue_ == p.value_.functionPointerValue_;
  else if (type_ == "double")
    return (doubles_equal(value_.doubleValue_.value,
        p.value_.doubleValue_.value,
        value_.doubleValue_.tolerance));
  else if (type_ == "const unsigned char*") {
    if (size_ != p.size_) {
      return false;
    }
    return String::MemCmp(value_.memoryBufferValue_,
               p.value_.memoryBufferValue_,
               size_) == 0;
  }

  if (comparator_)
    return comparator_->isEqual(
        value_.constObjectPointerValue_, p.value_.constObjectPointerValue_);

  return false;
}

bool
MockNamedValue::compatibleForCopying(const MockNamedValue& p) const
{
  if (type_ == p.type_)
    return true;

  if ((type_ == "const void*") && (p.type_ == "void*"))
    return true;

  return false;
}

String
MockNamedValue::toString() const
{
  if (type_ == "bool")
    return StringFrom(value_.boolValue_);
  else if (type_ == "int")
    return StringFrom(value_.intValue_) + " " +
           BracketsFormattedHexStringFrom(value_.intValue_);
  else if (type_ == "unsigned int")
    return StringFrom(value_.unsignedIntValue_) + " " +
           BracketsFormattedHexStringFrom(value_.unsignedIntValue_);
  else if (type_ == "long int")
    return StringFrom(value_.longIntValue_) + " " +
           BracketsFormattedHexStringFrom(value_.longIntValue_);
  else if (type_ == "unsigned long int")
    return StringFrom(value_.unsignedLongIntValue_) + " " +
           BracketsFormattedHexStringFrom(value_.unsignedLongIntValue_);
  else if (type_ == "long long int")
    return StringFrom(value_.longLongIntValue_) + " " +
           BracketsFormattedHexStringFrom(value_.longLongIntValue_);
  else if (type_ == "unsigned long long int")
    return StringFrom(value_.unsignedLongLongIntValue_) + " " +
           BracketsFormattedHexStringFrom(value_.unsignedLongLongIntValue_);
  else if (type_ == "const char*")
    return value_.stringValue_;
  else if (type_ == "void*")
    return StringFrom(value_.pointerValue_);
  else if (type_ == "void (*)()")
    return StringFrom(value_.functionPointerValue_);
  else if (type_ == "const void*")
    return StringFrom(value_.constPointerValue_);
  else if (type_ == "double")
    return StringFrom(value_.doubleValue_.value);
  else if (type_ == "const unsigned char*")
    return StringFromBinaryWithSizeOrNull(value_.memoryBufferValue_, size_);

  if (comparator_)
    return comparator_->valueToString(value_.constObjectPointerValue_);

  return StringFromFormat(
      "No comparator found for type: \"%s\"", type_.c_str());
}

} // namespace extensions
} // namespace cpputest
