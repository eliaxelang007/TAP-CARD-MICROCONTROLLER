#ifndef _SECTION
#define _SECTION

#include <cctype>
#include <variant>

#include "result.h"

// https://stackoverflow.com/questions/21295935/can-a-c-enum-class-have-methods
// https://stackoverflow.com/questions/68315363/class-cant-have-constants-of-own-type-inside
class Section
{
private:
    enum class Value
    {
        A,
        B,
        C
    } _section;

    constexpr explicit Section(Value section) noexcept : _section{section} {}

public:
    static const Section A;
    static const Section B;
    static const Section C;

    constexpr operator Value() const { return _section; }
    explicit operator bool() const = delete;

    constexpr bool operator==(const Section &other) const { return _section == other._section; }
    constexpr bool operator!=(const Section &other) const { return _section != other._section; }

    using OptionSection = Option<Section>;

    constexpr static OptionSection from_char(char character) noexcept
    {
        using O = OptionSection;
        using S = Section;

        switch (tolower(character))
        {
        case 'a':
            return O::ok(S::A);

        case 'b':
            return O::ok(S::B);

        case 'c':
            return O::ok(S::C);
        }

        return O::error(std::monostate{});
    }

    constexpr char to_char() const noexcept;
};

constexpr Section Section::A = Section(Section::Value::A);
constexpr Section Section::B = Section(Section::Value::B);
constexpr Section Section::C = Section(Section::Value::C);

constexpr char Section::to_char() const noexcept
{
    using S = Section;

    switch (_section)
    {
    case S::A:
        return 'a';
    case S::B:
        return 'b';
    case S::C:
        return 'c';
    }
}

#endif
