#ifndef _SECTION
#define _SECTION

enum class Section : uint8_t
{
    A,
    B,
    C
};

char section_to_char(Section section)
{
    using S = Section;

    switch (section)
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