#ifndef _ENUM
#define _ENUM

template <typename Value>
class Enum
{
protected:
    Value _section;

    constexpr explicit Enum(Value section) noexcept : _section{section} {}

public:
    constexpr operator Value() const { return _section; }
    explicit operator bool() const = delete;

    constexpr bool operator==(const Enum &other) const { return _section == other._section; }
    constexpr bool operator!=(const Enum &other) const { return _section != other._section; }
};

#endif