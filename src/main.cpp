#define DEBUG_MODE

#include <Arduino.h>

#include "serial_io.h"
#include "networking.h"
#include "uid.h"
#include "buzzer.h"
#include "lcd.h"
#include "rfid.h"
#include "section.h"

constexpr uint16_t SECOND_MILLISECONDS = 1000;

ServerApi api{
    "https://tap-card-server-production.up.railway.app"};

Display<2, 16> display{0x27};
RfidScanner scanner{5, 0};
Buzzer buzzer{13};

Section section = Section::A;

void setup()
{
    println("Starting program...");

    scanner.initialize();
    display.initialize();

    display.print("Welcome to");
    display.set_cursor(0, 1);
    display.print("AniTAP!");

    String ssid = readln("What's the wifi's ssid?");
    String password = readln("What's the wifi's password?");

    section = Section::from_char(readln("What's this terminal's section")[0])
                  .expect("[Error] Invalid section!");

    api.initialize(ssid, password);
}

int cards_detected = 0;

void loop()
{
    buzzer.play(CARD_DETECTED);

    scanner.update(
        [](Card card)
        {
            display.clear();
            buzzer.reset();

            display.print("Loading...");
            display.show();

            const ServerResponse response = api.send_tap(
                card.uid(),
                100 * SECOND_MILLISECONDS);

            display.clear();

            if (response.status < 200 || response.status >= 300)
            {
                display.print("Error...");
                return;
            }

            if (response.status == 200)
            {
                display.print("Name:");
                display.set_cursor(0, 1);
                display.print(response.response);
            }

            if (response.status == 201)
            {
                display.print("Invalid section:");
                display.set_cursor(0, 1);
                display.print("10");
                display.print((char)section.to_char());
                display.print(" != ");
                display.print("10");
                display.print(response.response);
            }
        });

    display.show();
}
