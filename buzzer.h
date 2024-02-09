#ifndef _BUZZER
#define _BUZZER

#include <array>
#include <inttypes.h>

struct Note
{
    const uint16_t frequency_hz;
    const uint16_t duration_ms;

    constexpr explicit Note(uint16_t frequency_hz, uint16_t duration_ms) : frequency_hz{frequency_hz}, duration_ms{duration_ms} {}
};

constexpr std::array<Note, 3> CARD_DETECTED = {
    Note{500, 50},
    Note{0, 50},
    Note{500, 75}};

constexpr std::array<Note, 70> ECRUTEAK_CITY = {
    Note{1175, 250},
    Note{1047, 250},
    Note{831, 250},
    Note{784, 375},
    Note{0, 125},
    Note{880, 375},
    Note{0, 125},
    Note{988, 375},
    Note{0, 125},
    Note{392, 375},
    Note{0, 125},
    Note{587, 625},
    Note{0, 125},
    Note{523, 125},
    Note{0, 125},
    Note{523, 625},
    Note{0, 375},
    Note{698, 625},
    Note{0, 125},
    Note{659, 250},
    Note{0, 250},
    Note{659, 250},
    Note{698, 250},
    Note{784, 250},
    Note{659, 250},
    Note{523, 125},
    Note{0, 125},
    Note{523, 250},
    Note{440, 125},
    Note{0, 125},
    Note{523, 625},
    Note{0, 375},
    Note{659, 250},
    Note{784, 125},
    Note{0, 125},
    Note{659, 250},
    Note{587, 375},
    Note{0, 375},
    Note{622, 500},
    Note{1175, 625},
    Note{0, 125},
    Note{1047, 125},
    Note{0, 125},
    Note{1047, 625},
    Note{0, 375},
    Note{1397, 625},
    Note{0, 125},
    Note{1319, 250},
    Note{0, 250},
    Note{1319, 250},
    Note{1397, 250},
    Note{1568, 125},
    Note{0, 125},
    Note{1760, 250},
    Note{1319, 125},
    Note{0, 125},
    Note{1319, 250},
    Note{1047, 125},
    Note{0, 125},
    Note{1047, 250},
    Note{880, 125},
    Note{0, 125},
    Note{988, 250},
    Note{1047, 125},
    Note{0, 125},
    Note{1319, 625},
    Note{0, 125},
    Note{1397, 125},
    Note{1319, 125},
    Note{1175, 750}};

class Buzzer
{
public:
    explicit Buzzer(uint8_t pin) : _pin{pin} {}

    void reset() volatile
    {
        _previous_song = nullptr;
    }

    template <size_t note_count>
    void play(std::array<Note, note_count> &song) volatile
    {
        const uint32_t current_time = millis();

        const bool was_playing = _previous_song == song;
        const bool switched_song = !was_playing;

        if (switched_song)
        {
            _previous_song = song.data();
            _note_index = 0;
        }

        if (_note_index >= note_count && was_playing)
        {
            return;
        }

        const bool continue_current_note = current_time - _previous_time < song[_note_index].duration_ms;

        if (continue_current_note && was_playing)
        {
            return;
        }

        noTone(_pin);

        if (was_playing)
        {
            _note_index++;

            if (_note_index >= note_count)
            {
                return;
            }
        }

        const uint16_t frequency = song[_note_index].frequency_hz;

        if (frequency > 0)
        {
            tone(_pin, frequency);
        }

        _previous_time = current_time;
    }

private:
    uint8_t _pin;
    size_t _note_index = 0;
    uint32_t _previous_time = 0;
    Note *_previous_song = nullptr;
};

#endif
