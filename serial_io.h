#ifndef _SERIAL_IO
#define _SERIAL_IO

#define DEBUG_MODE

void begin()
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
void print(P printable)
{
    begin();
    Serial.print(printable);
}

template <typename P>
void println(P printable)
{
    begin();
    Serial.println(printable);
}

template <typename P>
void debug(P printable)
{
#ifdef DEBUG_MODE
    print(printable);
#endif
}

template <typename P>
void debugln(P printable)
{
#ifdef DEBUG_MODE
    println(printable);
#endif
}

String readln(String message)
{
    println(message);

    while (!Serial.available())
    {
        continue;
    }

    return Serial.readStringUntil('\n');
}

#endif
