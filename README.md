# SaferStringView v2.0.0
A Safe String View Wrapper for C++20 applications (Header-Only Class)

**Author:** Amit Gefen  
**License:** MIT License

## Overview
* Provides a safe alternative to `std::string_view` that prevents dangling references from temporary strings.
* Automatically takes ownership of rvalue strings while efficiently viewing lvalue strings.
* Drop-in replacement for `std::string_view` with implicit conversion support.
* Leverages C++20 concepts for type safety.
* Zero-overhead abstraction using `std::variant` for storage.

## Features
* **Automatic Lifetime Management:**
   * Stores rvalue strings (temporaries) by value to prevent dangling references.
   * Stores lvalue strings as views for efficiency.
   * Safely handles string literals and `const char*` pointers.
* **Drop-in Replacement:**
   * Implicit conversion to `std::string_view` for seamless integration.
   * Works with existing APIs that accept `std::string_view`.
* **Type Safety:**
   * Uses C++20 concepts to ensure only valid character types are used.
   * Explicit constructors prevent accidental conversions.
* **Flexible Construction:**
   * Supports construction from `std::string` (lvalue and rvalue).
   * Supports construction from `std::string_view`.
   * Supports construction from string literals and `const char*`.
* **Memory Efficient:**
   * Only stores owned strings when necessary (rvalue temporaries).
   * Views existing strings without copying when safe.

## Usage
- Include the header file:

```cpp
#include "SaferStringView.hpp"
```

- Create SaferStringView instances:

```cpp
// From lvalue string (stores view)
std::string my_string = "Hello World";
SaferStringView ssv1(my_string);

// From rvalue string (takes ownership)
SaferStringView ssv2(std::to_string(2025));

// From string literal (stores view)
SaferStringView ssv3("String Literal");
```

- Use as drop-in replacement for std::string_view:

```cpp
void ProcessString(std::string_view sv) {
  std::cout << "Processing: " << sv << std::endl;
}

SaferStringView ssv(std::to_string(2025));
ProcessString(ssv);  // Implicit conversion to string_view
```

## Example Usage
See the `TestSuite.cpp` file for a comprehensive example demonstrating various construction scenarios and usage patterns.

## Problem Solved
Traditional `std::string_view` can lead to dangling references when constructed from temporary strings:

```cpp
// DANGEROUS with std::string_view
std::string_view sv = std::to_string(2025);  // sv now points to destroyed string!
std::cout << sv;  // Undefined behavior!
```

SaferStringView solves this by automatically detecting and storing temporaries:

```cpp
// SAFE with SaferStringView
SaferStringView ssv(std::to_string(2025));  // Owns the string
std::cout << std::string_view(ssv);  // Works correctly!
```

## Technical Details
* **Storage:** Uses `std::variant<std::basic_string<T>, std::basic_string_view<T>>` to store either owned strings or views.
* **Character Types:** Supports any character type with `std::char_traits` (char, wchar_t, char8_t, char16_t, char32_t).
* **Testing:** Compile with `#define _TEST` to enable test-specific features for verifying ownership.

## Dependencies
* C++20 compiler with concepts support
* Standard Library (no external dependencies)
