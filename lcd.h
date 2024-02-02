#ifndef _LCD
#define _LCD

#include <Print.h>
#include <LiquidCrystal_I2C.h>

#include "serial_io.h"

template <size_t rows, size_t columns>
class Display : public Print
{
public:
    Display(uint8_t i2c_address = 0x27) : lcd{i2c_address, columns, rows}
    {
        for (size_t i = 0; i < rows; i++)
        {
            current_buffer[i][columns] = '\0';
            previous_buffer[i][columns] = '\0';
        }

        clear();
        clear(previous_buffer);
    }

    void initialize()
    {
        lcd.init();
        lcd.backlight();
    }

    size_t write(uint8_t character) override
    {
        if (column >= columns || row >= rows)
            return 0;

        current_buffer[row][column] = character;

        column = (column + 1) % columns;

        if (column == 0)
        {
            row += 1;
        }

        return 1;
    }

    void clear()
    {
        set_cursor(0, 0);
        lcd.setCursor(0, 0);

        clear(current_buffer);
    }

    void set_cursor(uint8_t new_column, uint8_t new_row)
    {
        row = new_row;
        column = new_column;
    }

    void show()
    {
        bool was_same = false;

        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < columns; j++)
            {
                char current_character = current_buffer[i][j];
                char &previous_character = previous_buffer[i][j];

                const bool same = current_character == previous_character;

                if (same != was_same)
                {
                    was_same = same;

                    if (!same)
                    {
                        lcd.setCursor(j, i);
                    }
                }

                if (same)
                    continue;

                lcd.print(current_character);
                previous_character = current_character;
            }
        }
    }

private:
    char current_buffer[rows][columns + 1] = {0};
    char previous_buffer[rows][columns + 1] = {0};

    uint8_t row = 0;
    uint8_t column = 0;

    LiquidCrystal_I2C lcd;

    void clear(char (&buffer)[rows][columns + 1])
    {
        for (size_t i = 0; i < rows; i++)
        {
            for (size_t j = 0; j < columns; j++)
            {
                buffer[i][j] = ' ';
            }
        }
    }
};

#endif
