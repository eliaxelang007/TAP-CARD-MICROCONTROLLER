#ifndef _UID
#define _UID

#include <array>
#include <algorithm>
#include <inttypes.h>

class Uid
{
private:
    static constexpr uint8_t BYTE_COUNT = 10;
    static constexpr uint8_t BYTE_HEX_CHARS_LENGTH = 2;

    using HexCodeChars = std::array<char, BYTE_HEX_CHARS_LENGTH>;
    using Bytes = std::array<char, BYTE_COUNT>;

public:
    static constexpr uint8_t CHARS_LENGTH = BYTE_COUNT * BYTE_HEX_CHARS_LENGTH;
    using HexChars = std::array<char, CHARS_LENGTH>;

    constexpr explicit Uid(const uint8_t (&uid_bytes)[BYTE_COUNT], uint8_t uid_byte_count) noexcept : _bytes{}
    {
        std::copy_n(uid_bytes, uid_byte_count, _bytes.begin());
    }

    constexpr HexChars to_chars() const noexcept
    {
        HexChars uid_hex_str = {};

        for (int i = 0; i < BYTE_COUNT; i++)
        {
            const HexCodeChars hex_code = _to_hex_code(_bytes[i]);
            std::copy_n(hex_code.begin(), 2, uid_hex_str.begin() + (i * 2));
        }

        return uid_hex_str;
    }

    constexpr const Bytes &bytes() const noexcept
    {
        return _bytes;
    }

private:
    Bytes _bytes;

    constexpr static HexCodeChars _to_hex_code(uint8_t byte) noexcept
    {
        constexpr uint8_t MAX_HEXADECIMAL = 16;
        return {_digit_to_char(byte / 16), _digit_to_char(byte % MAX_HEXADECIMAL)};
    }

    constexpr static char _digit_to_char(uint8_t byte) noexcept
    {
        constexpr uint8_t SINGLE_DIGIT_COUNT = 9;
        return (byte <= SINGLE_DIGIT_COUNT) ? ('0' + byte) : ('a' + (byte - 1 - SINGLE_DIGIT_COUNT));
    }
};

#endif