#ifndef _RFID
#define _RFID

#include <functional>
#include <algorithm>
#include <inttypes.h>

#include <SPI.h>
#include <MFRC522.h>

#include "uid.h"
#include "variant.h"

class BlockIndex
{
public:
    static Option<BlockIndex> from_index(uint8_t index)
    {
        using O = Option<BlockIndex>;

        constexpr uint8_t TOTAL_BLOCKS = 63;

        return (index >= 0 && index <= TOTAL_BLOCKS) ? O::ok(BlockIndex{index}) : O::err(None{});
    }

    uint8_t index() const
    {
        return _index;
    }

private:
    friend class WriteBlockIndex;

    uint8_t _index;

    explicit BlockIndex(uint8_t index) : _index{index} {}
};

class WriteBlockIndex
{
public:
    static Option<WriteBlockIndex> from_index(BlockIndex block_index)
    {
        using O = Option<WriteBlockIndex>;

        constexpr uint8_t BLOCK_OFFSET = 3;

        const uint8_t index = block_index.index();

        const bool is_manufacturer = index <= 0;
        const bool is_trailer = index >= BLOCK_OFFSET && (index - BLOCK_OFFSET) % 4 == 0;

        return (is_manufacturer || is_trailer) ? O::err(None{}) : O::ok(WriteBlockIndex{block_index});
    }

    BlockIndex block_index()
    {
        return _block_index;
    }

private:
    BlockIndex _block_index;
    explicit WriteBlockIndex(BlockIndex index) : _block_index{index} {}
};

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

        Uid uid()
        {
            return Uid(_scanner.mfrc522.uid.uidByte, _scanner.mfrc522.uid.size);
        }

        using Status = MFRC522::StatusCode;
        using ReadResult = Result<Block, Status>;

        ReadResult read_block(BlockIndex block_index)
        {
            const Status authentication_status = _authenticate_block(block_index);

            if (authentication_status != MFRC522::STATUS_OK)
            {
                return ReadResult::err(authentication_status);
            }

            constexpr uint8_t CRC_BYTES = 2;
            constexpr uint8_t BYTES_TO_READ = BLOCK_BYTE_COUNT + CRC_BYTES;

            std::array<uint8_t, BYTES_TO_READ> buffer = {0};

            uint8_t written_bytes = BYTES_TO_READ;
            const Status read_status = _scanner.mfrc522.MIFARE_Read(block_index.index(), buffer.data(), &written_bytes);

            if (read_status != MFRC522::STATUS_OK)
            {
                return ReadResult::err(read_status);
            }

            Block block = {0};

            std::copy_n(buffer.begin(), BLOCK_BYTE_COUNT, block.bytes.begin());

            return ReadResult::ok(block);
        }

        using WriteResult = Result<None, Status>;

        WriteResult write_block(WriteBlockIndex write_index, Block block_data)
        {
            const BlockIndex block_index = write_index.block_index();

            const Status authentication_status = _authenticate_block(block_index);

            if (authentication_status != MFRC522::STATUS_OK)
            {
                return WriteResult::err(authentication_status);
            }

            const Status write_status = _scanner.mfrc522.MIFARE_Write(
                block_index.index(),
                block_data.bytes.data(),
                BLOCK_BYTE_COUNT);

            if (write_status != MFRC522::STATUS_OK)
            {
                return WriteResult::err(write_status);
            }

            return WriteResult::ok(None{});
        }

    private:
        friend class RfidScanner;

        Status _authenticate_block(BlockIndex block_index)
        {
            return _scanner.mfrc522.PCD_Authenticate(
                MFRC522::PICC_CMD_MF_AUTH_KEY_A,
                block_index.index(),
                &(_scanner.key),
                &(_scanner.mfrc522.uid));
        }

        explicit Card(RfidScanner &scanner) : _scanner{scanner} {}

        RfidScanner &_scanner;
    };

    RfidScanner(uint8_t chip_select_pin, uint8_t rest_power_down_pin) : mfrc522{chip_select_pin, rest_power_down_pin}, key{} {}

    void initialize()
    {
        SPI.begin();
        mfrc522.PCD_Init();
    }

    void update(std::function<void(Card)> on_tap)
    {
        if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial())
        {
            return;
        }

        on_tap(Card{*this});

        mfrc522.PICC_HaltA();
    }

private:
    MFRC522 mfrc522;
    MFRC522::MIFARE_Key key;
};

#endif _RFID
