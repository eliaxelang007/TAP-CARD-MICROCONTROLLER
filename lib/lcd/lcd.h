#ifndef _LCD
#define _LCD

#include <array>
#include <algorithm>

#include <Print.h>
#include <LiquidCrystal_I2C.h>

template <size_t rows, size_t columns>
class Display : public Print
{
public:
    Display(uint8_t i2c_address = 0x27) noexcept : _lcd{i2c_address, columns, rows},
                                                   _current_buffer{},
                                                   _previous_buffer{},
                                                   _row{0},
                                                   _column{0}
    {
        _clear(_current_buffer);
        _clear(_previous_buffer);
    }

    void initialize() noexcept
    {
        _lcd.init();
        _lcd.backlight();
    }

    size_t write(uint8_t character) noexcept override
    {
        if (_column >= columns || _row >= rows)
            return 0;

        _current_buffer[_row][_column++] = character;

        return 1;
    }

    void clear() noexcept
    {
        set_cursor(0, 0);
        _lcd.setCursor(0, 0);

        _clear(_current_buffer);
    }

    void set_cursor(uint8_t new_column, uint8_t new_row) noexcept
    {
        _row = new_row;
        _column = new_column;
    }

    void show() noexcept
    {
        for (int i = 0; i < rows; i++)
        {
            bool was_same = false;

            for (int j = 0; j < columns; j++)
            {
                const char current_character = _current_buffer[i][j];
                char &previous_character = _previous_buffer[i][j];

                const bool same = current_character == previous_character;

                if (same != was_same || (!was_same && !same))
                {
                    was_same = same;

                    if (!same)
                    {
                        _lcd.setCursor(j, i);
                    }
                }

                if (same)
                {
                    continue;
                }

                _lcd.print(current_character);
                previous_character = current_character;
            }
        }
    }

private:
    std::array<std::array<char, columns>, rows> _current_buffer;
    std::array<std::array<char, columns>, rows> _previous_buffer;

    uint8_t _row;
    uint8_t _column;

    LiquidCrystal_I2C _lcd;

    void _clear(std::array<std::array<char, columns>, rows> &buffer) noexcept
    {
        // https://www.reddit.com/r/cpp_questions/comments/p3ntua/how_to_properly_use_stdarrayfill_method_to/
        // https://godbolt.org/z/eTYhsv85h
        buffer[0].fill({' '});
        buffer.fill(buffer[0]);
    }
};

#endif
