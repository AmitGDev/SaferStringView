// NOLINTBEGIN(llvm-header-guard)
#ifndef AMITG_SAFERSTRINGVIEW_HPP_
#define AMITG_SAFERSTRINGVIEW_HPP_
// NOLINTEND(llvm-header-guard)

/*
    SaferStringView.hpp
    Copyright (c) 2025, Amit Gefen

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to
    deal in the Software without restriction, including without limitation the
    rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
    sell copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
    IN THE SOFTWARE.
*/

#include <cstdint>
#include <string>
#include <string_view>
#include <variant>

/**
 * @brief String view that owns rvalues, views lvalues.
 *
 * Prevents dangling references by taking ownership of temporary strings.
 * Lvalue strings must outlive the SaferStringView.
 *
 * @warning Cannot detect temporaries passed via string_view:
 *          SaferStringView(std::string_view(std::to_string(2025))) // Dangling!
 */
template <typename T>
  requires requires { typename std::char_traits<T>; }
class SaferStringView final {
 public:
  // Non-owning: from lvalue string reference (stores view)
  explicit SaferStringView(const std::basic_string<T> &str)
      : storage_(std::basic_string_view<T>(str)) {}

  // Owning: from rvalue string (stores owned string)
  explicit SaferStringView(std::basic_string<T> &&str)
      : storage_(std::move(str)) {}

  // Non-owning: from string_view
  explicit SaferStringView(std::basic_string_view<T> view) : storage_(view) {}

  // Non-owning: from const char* (common use case)
  explicit SaferStringView(const T *str)
      : storage_(std::basic_string_view<T>(str)) {}

  // C++23: Deducing this for implicit conversion operator
  // Implicit conversion for drop-in string_view replacement
  // NOLINTNEXTLINE(google-explicit-constructor,hicpp-explicit-conversions)
  constexpr operator std::basic_string_view<T>(
      this const SaferStringView &self) {
    return std::visit(
        [](const auto &val) -> std::basic_string_view<T> { return val; },
        self.storage_);
  }

 private:
#ifdef TEST_SAFERSTRINGVIEW
 public:
#endif
  std::variant<std::basic_string<T>, std::basic_string_view<T>> storage_;
};

#endif