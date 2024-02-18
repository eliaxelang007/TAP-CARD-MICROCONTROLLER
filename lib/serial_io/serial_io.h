#ifndef _SERIAL_IO
#define _SERIAL_IO

#define DEBUG_MODE

#include <Arduino.h>

void begin() noexcept
{
    static bool began = false;

    if (began)
    {
        return;
    }

    Serial.begin(9600);

    began = true;
}

template <typename P>
void print(P printable) noexcept
{
    begin();
    Serial.print(printable);
}

template <typename P>
void println(P printable) noexcept
{
    begin();
    Serial.println(printable);
}

template <typename P>
void debug(P printable) noexcept
{
#ifdef DEBUG_MODE
    print(printable);
#endif
}

template <typename P>
void debugln(P printable) noexcept
{
#ifdef DEBUG_MODE
    println(printable);
#endif
}

template <typename P>
String readln(P printable) noexcept
{
    print(printable);
    println(" ");

    while (!Serial.available())
    {
        continue;
    }

    return Serial.readStringUntil('\n');
}

#endif
