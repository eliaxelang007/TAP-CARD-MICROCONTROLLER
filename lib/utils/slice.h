#ifndef _SLICE
#define _SLICE

template <typename T>
class Slice
{
public:
    constexpr Slice(T &data) noexcept : _data{data}, _length{1} {}
    constexpr Slice(T &data, size_t length) noexcept : _data{data}, _length{length} {}

    template <size_t length>
    constexpr Slice(T (&raw_array)[length]) noexcept : _data{raw_array[0]}, _length{length} {}

    template <size_t length>
    constexpr Slice(std::array<T, length> &array) noexcept : _data{*array.data()}, _length{length} {}

    constexpr size_t length() const noexcept
    {
        return _length;
    }

    constexpr T &operator[](size_t index) noexcept
    {
        return (&_data)[index];
    }

    constexpr const T &operator[](size_t index) const noexcept
    {
        return (&_data)[index];
    }

    constexpr const T *begin() const noexcept
    {
        return &_data;
    }

    constexpr const T *end() const noexcept
    {
        return (&_data) + _length;
    }

private:
    T &_data;
    size_t _length;
};

template <>
template <size_t size>
constexpr Slice<const char>::Slice(const char (&raw_array)[size]) noexcept : _data{raw_array[0]}, _length{size - 1} {}

#endif