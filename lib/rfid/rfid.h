#ifndef _RFID
#define _RFID

#include <functional>
#include <algorithm>
#include <inttypes.h>

#include <SPI.h>
#include <MFRC522.h>

#include "uid.h"
#include "result.h"

#include "block_index.h"

struct Block
{
    static constexpr uint8_t BLOCK_BYTE_COUNT = 16;
    std::array<uint8_t, BLOCK_BYTE_COUNT> bytes;
};

class RfidScanner
{
public:
    class Card
    {
    public:
        static constexpr uint8_t BLOCK_BYTE_COUNT = Block::BLOCK_BYTE_COUNT;

        constexpr Uid uid() const noexcept
        {
            return Uid(_scanner._mfrc522.uid.uidByte, _scanner._mfrc522.uid.size);
        }

        using Status = MFRC522::StatusCode;
        using ReadResult = Result<Block, Status>;

        ReadResult read_block(BlockIndex block_index) noexcept
        {
            const Status authentication_status = _authenticate_block(block_index);

            if (authentication_status != MFRC522::STATUS_OK)
            {
                return ReadResult::error(authentication_status);
            }

            constexpr uint8_t CRC_BYTES = 2;
            constexpr uint8_t BYTES_TO_READ = BLOCK_BYTE_COUNT + CRC_BYTES;

            std::array<uint8_t, BYTES_TO_READ> buffer = {0};

            uint8_t written_bytes = BYTES_TO_READ;
            const Status read_status = _scanner._mfrc522.MIFARE_Read(block_index.index(), buffer.data(), &written_bytes);

            if (read_status != MFRC522::STATUS_OK)
            {
                return ReadResult::error(read_status);
            }

            Block block = {0};

            std::copy_n(buffer.begin(), BLOCK_BYTE_COUNT, block.bytes.begin());

            return ReadResult::ok(block);
        }

        using WriteResult = Result<std::monostate, Status>;

        WriteResult write_block(WriteBlockIndex write_index, Block block_data) noexcept
        {
            const BlockIndex block_index = write_index.block_index();

            const Status authentication_status = _authenticate_block(block_index);

            if (authentication_status != MFRC522::STATUS_OK)
            {
                return WriteResult::error(authentication_status);
            }

            const Status write_status = _scanner._mfrc522.MIFARE_Write(
                block_index.index(),
                block_data.bytes.data(),
                BLOCK_BYTE_COUNT);

            if (write_status != MFRC522::STATUS_OK)
            {
                return WriteResult::error(write_status);
            }

            return WriteResult::ok(std::monostate{});
        }

    private:
        friend class RfidScanner;

        Status _authenticate_block(BlockIndex block_index) noexcept
        {
            return _scanner._mfrc522.PCD_Authenticate(
                MFRC522::PICC_CMD_MF_AUTH_KEY_A,
                block_index.index(),
                &(_scanner._key),
                &(_scanner._mfrc522.uid));
        }

        explicit Card(RfidScanner &scanner) noexcept : _scanner{scanner} {}

        RfidScanner &_scanner;
    };

    explicit RfidScanner(uint8_t chip_select_pin, uint8_t rest_power_down_pin) noexcept : _mfrc522{chip_select_pin, rest_power_down_pin}, _key{} {}

    void initialize() noexcept
    {
        SPI.begin();
        _mfrc522.PCD_Init();
    }

    void update(std::function<void(Card)> on_tap)
    {
        if (!_mfrc522.PICC_IsNewCardPresent() || !_mfrc522.PICC_ReadCardSerial())
        {
            return;
        }

        on_tap(Card{*this});

        _mfrc522.PICC_HaltA();
    }

private:
    MFRC522 _mfrc522;
    MFRC522::MIFARE_Key _key;
};

using Card = RfidScanner::Card;

#endif _RFID
