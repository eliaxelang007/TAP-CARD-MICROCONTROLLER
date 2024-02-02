#include "serial_io.h"
#include "networking.h"
#include "uid.h"
#include "buzzer.h"
#include "lcd.h"
#include "rfid.h"

constexpr uint8_t SECOND_MILLISECONDS = 1000;

ServerApi api{"https://tap-card-server-production.up.railway.app"};

Display<2, 16> display{0x27};
RfidScanner scanner{5, 0};
Buzzer buzzer{13};

void setup()
{
    println("Hello, world!");

    //api.initialize("ANIMO", "2401Taft");
    //api.initialize("Cedric", "cedric1278");

    // Wifi: PLDTHOMEFIBR1cd30; Password: PLDTWIFIDQ3f4

    String ssid = readln("What's the wifi's ssid?");
    String password = readln("What's the wifi's password?");
    
    api.initialize(ssid, password);
    
    scanner.initialize();
    display.initialize();
    display.print("Welcome to");
    display.set_cursor(0, 1);
    display.print("AniTAP!");
}

int cards_detected = 0;

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
            
            const ServerResponse response = api.send_tap(card.uid, 100 * SECOND_MILLISECONDS);

            display.clear();

            if (response.status != HTTP_CODE_OK) {
              display.print("Error...");
            } else {
              display.print("Name");
              display.set_cursor(0, 1);
              display.print(response.response);
            } 
        });

    display.show();
}

// using Status = MFRC522::StatusCode;

// ReadResult read_block(uint8_t block_index)
// {
//     Status authentication_status = authenticate_block(rfid, key, block_index);

//     if (authentication_status != MFRC522::STATUS_OK)
//     {
//         return ReadResult::err(authentication_status);
//     }

//     constexpr uint8_t CRC_BYTES = 2;
//     constexpr uint8_t BYTES_TO_READ = BLOCK_SIZE + CRC_BYTES;

//     uint8_t buffer[BYTES_TO_READ] = {0};
//     uint8_t written_bytes = BYTES_TO_READ;

//     Status read_status = rfid.MIFARE_Read(block_index, buffer, &written_bytes);

//     if (read_status != MFRC522::STATUS_OK)
//     {
//         return ReadResult::err(read_status);
//     }

//     Block block = {0};

//     for (int i = 0; i < BLOCK_SIZE; i++)
//     {
//         block.bytes[i] = buffer[i];
//     }

//     return ReadResult::ok(block);
// }
