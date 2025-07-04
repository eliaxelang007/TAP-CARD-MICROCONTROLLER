#ifndef _RESULT
#define _RESULT

#include <functional>
#include <utility>
#include <variant>

template <typename T, typename E>
class Result
{
public:
    constexpr static Result ok(T &&ok) noexcept
    {
        return Result{
            Tag::Ok,
            Value{std::in_place_index<0>, ok}};
    }

    constexpr static Result error(E &&error) noexcept
    {
        return Result{
            Tag::Error,
            Value{std::in_place_index<1>, error}};
    }

    constexpr static Result ok(const T &ok) noexcept
    {
        return Result{
            Tag::Ok,
            Value{std::in_place_index<0>, ok}};
    }

    constexpr static Result error(const E &error) noexcept
    {
        return Result{
            Tag::Error,
            Value{std::in_place_index<1>, error}};
    }

    // template <typename M>
    // M match(
    //     std::function<M(T)> on_ok,
    //     std::function<M(E)> on_error) &&
    // {
    //     switch (_tag)
    //     {
    //     case Tag::Ok:
    //         return on_ok(std::get<0>(std::move(_value)));
    //     case Tag::Error:
    //         return on_error(std::get<1>(std::move(_value)));
    //     }
    // }

    template <typename M>
    constexpr M match(
        M (*on_ok)(T),
        M (*on_error)(E)) &&
    {
        switch (_tag)
        {
        case Tag::Ok:
            return on_ok(std::get<0>(std::move(_value)));
        case Tag::Error:
            return on_error(std::get<1>(std::move(_value)));
        }
    }

    // template <typename M>
    // Result<M, E> map(
    //     std::function<M(T)> map) &&
    // {
    //     using R = Result<M, E>;

    //     switch (_tag)
    //     {
    //     case Tag::Ok:
    //         return R::ok(map(std::get<0>(std::move(_value))));
    //     case Tag::Error:
    //         return R::error(std::get<1>(std::move(_value)));
    //     }
    // }

    constexpr T expect(const char *message)
    {
        return match(
            [](T ok)
            { return ok; },
            [](E error)
            { return panic(message); });
    }

    template <typename M>
    constexpr Result<M, E> map(
        M (*map)(T)) const
    {
        using R = Result<M, E>;

        switch (_tag)
        {
        case Tag::Ok:
            return R::ok(map(std::get<0>(_value)));
        case Tag::Error:
            return R::error(E{std::get<1>(_value)});
        }
    }

private:
    enum class Tag
    {
        Ok,
        Error
    } _tag;

    using Value = std::variant<T, E>;
    Value _value;

    constexpr Result(Tag tag, Value &&value) noexcept : _tag{tag}, _value{std::move(value)} {}
};

template <typename T>
[[noreturn]] constexpr T never() noexcept
{
    while (true)
    {
    }
}

template <typename P, typename T>
[[noreturn]] constexpr T panic(P printable) noexcept
{
    println(printable);
    return never();
}

template <typename T>
using Option = Result<T, std::monostate>;

#endif