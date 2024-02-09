#ifndef _UID
#define _UID

#include <array>
#include <algorithm>
#include <inttypes.h>

class Uid
{
public:
    static constexpr uint8_t BYTE_COUNT = 10;
    static constexpr uint8_t BYTE_HEX_CHARS_LENGTH = 2;
    static constexpr uint8_t CHARS_LENGTH = BYTE_COUNT * BYTE_HEX_CHARS_LENGTH;

    using Bytes = std::array<char, BYTE_COUNT>;
    using HexString = std::array<char, CHARS_LENGTH>;
    using HexCodeChars = std::array<char, BYTE_HEX_CHARS_LENGTH>;

    explicit Uid(const uint8_t (&uid_bytes)[BYTE_COUNT], uint8_t uid_byte_count) : _bytes{}
    {
        std::copy_n(uid_bytes, uid_byte_count, _bytes.begin());
    }

    HexString to_chars()
    {
        HexString uid_hex_str = {};

        for (int i = 0; i < BYTE_COUNT; i++)
        {
            const HexCodeChars hex_code = to_hex_code(_bytes[i]);
            std::copy_n(hex_code.begin(), 2, uid_hex_str.begin() + (i * 2));
        }

        return uid_hex_str;
    }

    const Bytes &bytes()
    {
        return _bytes;
    }

private:
    Bytes _bytes;

    static HexCodeChars to_hex_code(uint8_t byte)
    {
        constexpr uint8_t MAX_HEXADECIMAL = 16;
        return {digit_to_char(byte / 16), digit_to_char(byte % MAX_HEXADECIMAL)};
    }

    static char digit_to_char(uint8_t byte)
    {
        constexpr uint8_t SINGLE_DIGIT_COUNT = 9;
        if (byte <= SINGLE_DIGIT_COUNT)
        {
            return '0' + byte;
        }
        return 'a' + (byte - 1 - SINGLE_DIGIT_COUNT);
    }
};

#endif