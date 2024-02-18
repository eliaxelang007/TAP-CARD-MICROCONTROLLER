#ifndef _SECTION
#define _SECTION

#include <cctype>
#include <variant>

#include "result.h"
#include "enum.h"

// https://stackoverflow.com/questions/21295935/can-a-c-enum-class-have-methods
// https://stackoverflow.com/questions/68315363/class-cant-have-constants-of-own-type-inside

namespace _
{
    enum class Section
    {
        A,
        B,
        C
    };
}

class Section : public Enum<_::Section>
{
private:
    using Value = _::Section;

    constexpr explicit Section(Value section) noexcept : Enum<Value>{section} {}

public:
    static const Section A;
    static const Section B;
    static const Section C;

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
