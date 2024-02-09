#ifndef _SECTION
#define _SECTION

#include <cctype>

#include "variant.h"

// https://stackoverflow.com/questions/21295935/can-a-c-enum-class-have-methods
class Section
{
public:
    enum SectionValue
    {
        A,
        B,
        C
    };

    Section() = default;
    Section(SectionValue section) : _section{section} {}

    operator SectionValue() const { return _section; }
    explicit operator bool() const = delete;

    bool operator==(Section other) const { return _section == other._section; }
    bool operator!=(Section other) const { return _section != other._section; }

    using OptionSection = Option<Section>;

    static OptionSection to_section(char character)
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

        return O::err(None{});
    }

    char to_char()
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

private:
    SectionValue _section;
};

#endif
