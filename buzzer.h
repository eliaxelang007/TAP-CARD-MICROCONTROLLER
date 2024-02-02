#ifndef _BUZZER
#define _BUZZER

#include <inttypes.h>

struct Note
{
    const uint16_t frequency_hz;
    const uint16_t duration_ms;
};

constexpr Note CARD_DETECTED[] = {
    {500, 50},
    {0, 50},
    {500, 75}};

constexpr Note ECRUTEAK_CITY[] =
    {{1175, 250},
     {1047, 250},
     {831, 250},
     {784, 375},
     {0, 125},
     {880, 375},
     {0, 125},
     {988, 375},
     {0, 125},
     {392, 375},
     {0, 125},
     {587, 625},
     {0, 125},
     {523, 125},
     {0, 125},
     {523, 625},
     {0, 375},
     {698, 625},
     {0, 125},
     {659, 250},
     {0, 250},
     {659, 250},
     {698, 250},
     {784, 250},
     {659, 250},
     {523, 125},
     {0, 125},
     {523, 250},
     {440, 125},
     {0, 125},
     {523, 625},
     {0, 375},
     {659, 250},
     {784, 125},
     {0, 125},
     {659, 250},
     {587, 375},
     {0, 375},
     {622, 500},
     {1175, 625},
     {0, 125},
     {1047, 125},
     {0, 125},
     {1047, 625},
     {0, 375},
     {1397, 625},
     {0, 125},
     {1319, 250},
     {0, 250},
     {1319, 250},
     {1397, 250},
     {1568, 125},
     {0, 125},
     {1760, 250},
     {1319, 125},
     {0, 125},
     {1319, 250},
     {1047, 125},
     {0, 125},
     {1047, 250},
     {880, 125},
     {0, 125},
     {988, 250},
     {1047, 125},
     {0, 125},
     {1319, 625},
     {0, 125},
     {1397, 125},
     {1319, 125},
     {1175, 750}};

class Buzzer
{
private:
    const uint8_t pin;

    size_t index = 0;

    uint32_t previous_time = 0;

    Note *previous_instructions = nullptr;

public:
    Buzzer(uint8_t pin) : pin{pin} {}

    void reset() volatile
    {
        previous_instructions = nullptr;
    }

    template <size_t S>
    void update(Note const (&notes)[S]) volatile
    {
        const uint32_t current_time = millis();

        goto main;

    play_note:
    {
        index++;

        noTone(pin);

        if (index < S)
        {
            const Note note = notes[index];
            const uint16_t frequency = note.frequency_hz;

            if (frequency > 0)
            {
                tone(pin, frequency);
                // delay(100);
            }

            previous_time = current_time;
        }

        return;
    }

    main:
    {
        if (previous_instructions != notes)
        {
            index = -1;

            previous_instructions = const_cast<Note *>(notes);

            goto play_note;
        }

        if (current_time - previous_time >= abs(notes[index].duration_ms))
        {
            goto play_note;
        }
    }
    }
};

#endif
