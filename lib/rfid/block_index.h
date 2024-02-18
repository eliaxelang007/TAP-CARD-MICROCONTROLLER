#ifndef _BLOCK_INDEX
#define _BLOCK_INDEX

#include "result.h"
#include <inttypes.h>

class BlockIndex
{
public:
    class OutOfRange
    {
    };

    using MaybeBlockIndex = Result<BlockIndex, OutOfRange>;

    constexpr static MaybeBlockIndex from_index(uint8_t index) noexcept
    {
        return (_is_valid(index)) ? MaybeBlockIndex::ok(BlockIndex{index}) : MaybeBlockIndex::error(OutOfRange{});
    }

    template <uint8_t index>
    constexpr static BlockIndex from_index() noexcept
    {
        static_assert(_is_valid(index), "Out of range block index!");
        return BlockIndex(index);
    }

    constexpr uint8_t index() const noexcept
    {
        return _index;
    }

private:
    friend class WriteBlockIndex;

    uint8_t _index;

    static constexpr bool _is_valid(uint8_t index) noexcept
    {
        constexpr uint8_t TOTAL_BLOCKS = 63;
        return index >= 0 && index <= TOTAL_BLOCKS;
    }

    constexpr explicit BlockIndex(uint8_t index) noexcept : _index{index} {}
};

class WriteBlockIndex
{
public:
    class Unwritable
    {
    };

    using MaybeWriteBlockIndex = Result<WriteBlockIndex, Unwritable>;

    constexpr static MaybeWriteBlockIndex from_index(BlockIndex block_index) noexcept
    {
        return (_is_valid(block_index)) ? MaybeWriteBlockIndex::ok(WriteBlockIndex{block_index}) : MaybeWriteBlockIndex::error(Unwritable{});
    }

    template <uint8_t index>
    constexpr static WriteBlockIndex from_index() noexcept
    {
        constexpr BlockIndex block_index = BlockIndex::from_index<index>();
        static_assert(_is_valid(block_index), "Unwritable block!");
        return WriteBlockIndex(block_index);
    }

    constexpr BlockIndex block_index() const noexcept
    {
        return _block_index;
    }

private:
    constexpr static bool _is_valid(BlockIndex block_index) noexcept
    {
        constexpr uint8_t BLOCK_OFFSET = 3;

        const uint8_t index = block_index.index();

        const bool is_trailer_block = index >= BLOCK_OFFSET && (index - BLOCK_OFFSET) % 4 == 0;
        const bool is_manufacturer_block = index <= 0;

        return !is_trailer_block && !is_manufacturer_block;
    }

    BlockIndex _block_index;
    constexpr explicit WriteBlockIndex(BlockIndex index) noexcept : _block_index{index} {}
};

/*
#generate_indexes.py
#!/usr/bin/python

def is_valid(index):
    BLOCK_OFFSET = 3
    is_trailer_block = (index >= BLOCK_OFFSET) and ((index - BLOCK_OFFSET) % 4 == 0)
    is_manufacturer_block = index <= 0

    return (not is_trailer_block) and (not is_manufacturer_block)

valid_indexes = [index for index in range(0, 64) if is_valid(index)]

for i in valid_indexes:
    print(f"static const WriteBlockIndex BLOCK_{i};")

for i in valid_indexes:
    print(f"constexpr WriteBlockIndex WriteBlockIndex::BLOCK_{i} = WriteBlockIndex(BlockIndex::BLOCK_{i});")
*/

#endif