#include "mutiny/test/GroupLocation.hpp"

namespace mu {
namespace tiny {
namespace test {

GroupLocation* GroupLocation::head_ = nullptr;

GroupLocation::GroupLocation(
    const char* group_name,
    const char* file_name,
    size_t line_number
) noexcept
  : group_(group_name)
  , file_(file_name)
  , line_number_(line_number)
  , next_(head_)
{
  head_ = this;
}

const char* GroupLocation::get_group() const
{
  return group_;
}

const char* GroupLocation::get_file() const
{
  return file_;
}

size_t GroupLocation::get_line_number() const
{
  return line_number_;
}

GroupLocation* GroupLocation::get_next() const
{
  return next_;
}

GroupLocation* GroupLocation::get_head()
{
  return head_;
}

void GroupLocation::set_head(GroupLocation* head)
{
  head_ = head;
}

} // namespace test
} // namespace tiny
} // namespace mu
