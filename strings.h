#ifndef _STRINGS
#define _STRINGS

#include <array>

template <typename T>
struct StringPart
{
};

template <>
struct StringPart<const char &>
{
    static constexpr size_t length = 1;
    const char &part;

    const char *data()
    {
        return &part;
    }
};

template <size_t size>
struct StringPart<const std::array<char, size> &>
{
    static constexpr size_t length = size;
    const std::array<char, size> &part;

    const char *data()
    {
        return part.data();
    }
};

template <size_t size>
struct StringPart<const char (&)[size]>
{
    static constexpr size_t length = size;
    const char (&part)[size];

    const char *data()
    {
        return part;
    }
};

constexpr size_t total_chars_length()
{
    return 0;
}

template <typename T, typename... Args>
constexpr size_t total_chars_length(const T &text, const Args &...texts)
{
    return StringPart<decltype(text)>::length + total_chars_length(texts...);
}

void add_to_buffer(String &buffer)
{
    (void)(buffer); // Telling the compiler that this is unused.
}

template <typename T, typename... Args>
void add_to_buffer(String &buffer, const T &text, const Args &...texts)
{
    using part_type = StringPart<decltype(text)>;

    const size_t length = part_type::length;
    const char *data = part_type{text}.data();

    buffer.concat(data, length);
    add_to_buffer(buffer, texts...);
}

template <typename T, typename... Args>
String build_string(const T &text, const Args &...texts)
{
    const size_t length = total_chars_length(text, texts...);

    String buffer;
    buffer.reserve(length);

    add_to_buffer(buffer, text, texts...);

    return buffer;
}

#endif