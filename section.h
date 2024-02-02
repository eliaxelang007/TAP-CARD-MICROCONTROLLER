#ifndef _SECTION
#define _SECTION

enum class Section : uint8_t
{
    A,
    B,
    C
};

Section char_to_section(char character) {
    using S = Section;

    switch (character) {
    case 'a':
      return Section::A;

    case 'b':
      return Section::B;

    case 'c':
       return Section::C;
    }

    return Section::A;
}

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
