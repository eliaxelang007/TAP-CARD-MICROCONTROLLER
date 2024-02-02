#ifndef _RFID
#define _RFID

#include <SPI.h>
#include <MFRC522.h>

#include "uid.h"

//using Status = MFRC522::StatusCode;

struct Card {
  Uid uid;
};


typedef void (*OnTapCallback)(Card);

class RfidScanner
{
public:
    RfidScanner(uint8_t chip_select_pin, uint8_t rest_power_down_pin) : mfrc522{chip_select_pin, rest_power_down_pin}, key{} {}

    void initialize()
    {
        SPI.begin();
        mfrc522.PCD_Init();
    }

    void update(OnTapCallback on_tap)
    {
        if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial())
        {
            return;
        }

        on_tap(Card{
          .uid = Uid(mfrc522.uid.uidByte, mfrc522.uid.size) 
        });

        mfrc522.PICC_HaltA();
    }

private:
    MFRC522 mfrc522;
    MFRC522::MIFARE_Key key;
};



#endif _RFID
