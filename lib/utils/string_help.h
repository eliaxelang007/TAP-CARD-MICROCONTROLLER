#ifndef _STRING_HELP
#define _STRING_HELP

#include <Arduino.h>
#include <array>

#include "slice.h"

using StringPart = Slice<const char>;

constexpr size_t strings_length() noexcept
{
    return 0;
}

template <typename... Args>
constexpr size_t strings_length(StringPart part, Args... texts) noexcept
{
    return part.length() + strings_length(texts...);
}

template <typename... Args>
constexpr size_t strings_length(const Args &...texts) noexcept
{
    return strings_length(StringPart{texts}...);
}

constexpr void add_to_buffer(String &buffer) noexcept
{
    (void)(buffer); // Unused
}

template <typename... Args>
constexpr void add_to_buffer(String &buffer, StringPart part, Args... parts) noexcept
{
    buffer.concat(part.begin(), part.length());
    add_to_buffer(buffer, parts...);
}

template <typename... Args>
constexpr void add_to_buffer(String &buffer, const Args &...texts) noexcept
{
    add_to_buffer(buffer, StringPart{texts}...);
}

template <typename... Args>
String build_string(StringPart part, Args... texts) noexcept
{
    const size_t length = strings_length(part, texts...);

    String buffer;
    buffer.reserve(length);

    add_to_buffer(buffer, part, texts...);

    return buffer;
}

template <typename... Args>
String build_string(const Args &...texts) noexcept
{
    return build_string(StringPart{texts}...);
}

#endif