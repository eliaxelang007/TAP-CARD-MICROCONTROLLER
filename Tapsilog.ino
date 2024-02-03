#include "serial_io.h"
#include "networking.h"
#include "uid.h"
#include "buzzer.h"
#include "lcd.h"
#include "rfid.h"
#include "section.h"

constexpr uint8_t SECOND_MILLISECONDS = 1000;

ServerApi api{"https://tap-card-server-production.up.railway.app"};

Display<2, 16> display{0x27};
RfidScanner scanner{5, 0};
Buzzer buzzer{13};

Section section = Section::A;

const uint8_t red = 34;
const uint8_t green = 35;

void setup()
{
  println("Hello, world!");

  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);

  scanner.initialize();
  display.initialize();

  display.print("Welcome to");
  display.set_cursor(0, 1);
  display.print("AniTAP!");

  // api.initialize("ANIMO", "2401Taft");
  // api.initialize("Cedric", "cedric1278");

  // Wifi: PLDTHOMEFIBR1cd30; Password: PLDTWIFIDQ3f4

  String ssid = readln("What's the wifi's ssid?");
  String password = readln("What's the wifi's password?");

  section = char_to_section(readln("What's this terminal's section")[0]);

  api.initialize(ssid, password);
}

int cards_detected = 0;

void fail_led()
{
  digitalWrite(red, HIGH);
  digitalWrite(green, LOW);
}
void success_led()
{
  digitalWrite(red, LOW);
  digitalWrite(green, HIGH);
}

void loop()
{
  buzzer.update(CARD_DETECTED);

  scanner.update(
      [](Card card)
      {
        display.clear();
        buzzer.reset();

        display.print("Loading...");
        display.show();

        const ServerResponse response = api.send_tap(section, card.uid, 100 * SECOND_MILLISECONDS);

        display.clear();

        if (response.status < 200 || response.status >= 300)
        {
          fail_led();
          display.print("Error...");
          return;
        }

        success_led();

        if (response.status == 200)
        {
          success_led();

          display.print("Name:");
          display.set_cursor(0, 1);
          display.print(response.response);
        }
        else if (response.status == 201)
        {
          fail_led();

          display.print("10");

          String section = "";

          section.concat((char)section_to_char(section));

          display.print(section);

          display.print(" Terminal:");

          display.set_cursor(0, 1);

          display.print("Found 10");
          display.print(response.response);
        }
      });

  display.show();
}
