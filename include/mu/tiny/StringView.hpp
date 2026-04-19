/**
 * @file
 * @brief Non-owning view over a NUL-terminated character sequence.
 *
 * StringView stores a pointer and a length without allocating or copying.
 * It is always the custom implementation regardless of @ref
 * MUTINY_USE_STD_STRING, keeping the type trivially copyable and free of
 * standard-library dependencies.
 */

#ifndef INCLUDED_MU_TINY_STRING_VIEW_HPP
#define INCLUDED_MU_TINY_STRING_VIEW_HPP

#include "mu/tiny/String.hpp"

namespace mu {
namespace tiny {

/**
 * @brief Non-owning, read-only view of a NUL-terminated string.
 *
 * The referenced data must outlive the view. Construction from a `const char*`
 * or a @ref String is implicit so that existing call sites require no changes
 * when a function parameter is widened from `const String&` to `StringView`.
 */
class MUTINY_EXPORT StringView
{
public:
  /** @brief Construct from a NUL-terminated C string. */
  StringView(const char* str);
  /** @brief Construct from a pointer and an explicit length. */
  StringView(const char* str, size_t len);
  /** @brief Construct from a @ref String (caller must ensure lifetime). */
  StringView(const String& str);

  /** @brief Pointer to the character data. */
  const char* data() const { return data_; }
  /** @brief NUL-terminated pointer to the character data. */
  const char* c_str() const { return data_; }
  /** @brief Number of characters (not counting the NUL terminator). */
  size_t size() const { return size_; }
  /** @brief True if the view has zero characters. */
  bool empty() const { return size_ == 0; }
  /** @brief Access the character at @p pos (unchecked). */
  const char& operator[](size_t pos) const { return data_[pos]; }

private:
  const char* data_;
  size_t size_;
};

/** @brief True if both views refer to the same character sequence. */
MUTINY_EXPORT bool operator==(StringView left, StringView right);
/** @brief True if the views refer to different character sequences. */
MUTINY_EXPORT bool operator!=(StringView left, StringView right);

} // namespace tiny
} // namespace mu

#endif
