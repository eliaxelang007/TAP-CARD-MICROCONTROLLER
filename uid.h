#ifndef _UID
#define _UID

#include <array>

using std::array;

constexpr uint8_t UID_BYTE_COUNT = 10;
constexpr uint8_t BYTE_HEX_CHARS_LENGTH = 2;
constexpr uint8_t UID_CHARS_LENGTH = UID_BYTE_COUNT * BYTE_HEX_CHARS_LENGTH;

class Uid
{
public:
    Uid(uint8_t (&uid_bytes)[UID_BYTE_COUNT], uint8_t uid_byte_count)
    {
        for (int i = 0; i < uid_byte_count; i++)
        {
            bytes[i] = uid_bytes[i];
        }
    }

    using UidHexCode = array<char, UID_CHARS_LENGTH>;
    using ByteHexCode = array<char, BYTE_HEX_CHARS_LENGTH>;

    String to_string()
    {
      String uid_hex_str;

      for (int i = 0; i < UID_BYTE_COUNT; i++) {
        auto a = to_hex_code(bytes[i]);
        for (int j = 0; j < 2; j++) {
          uid_hex_str.concat(a[j]);
        }
      }
      
      return uid_hex_str;
    }

    static ByteHexCode to_hex_code(uint8_t byte)
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

private:
    Uid() = default;
    uint8_t bytes[UID_BYTE_COUNT] = {0};
};

#endif 
