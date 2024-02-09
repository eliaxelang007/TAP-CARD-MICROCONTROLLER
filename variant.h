#ifndef _RESULT
#define _RESULT

#include <utility>
#include <functional>

template <typename T, typename E>
class Result
{
public:
    static Result ok(const T &ok)
    {
        Result result{};

        result._tag = Tag::Ok;
        result._value.ok = ok;

        return result;
    }

    static Result ok(T &&ok)
    {
        Result result{};

        result._tag = Tag::Ok;
        result._value.ok = std::move(ok);

        return result;
    }

    static Result err(const E &error)
    {
        Result result{};

        result._tag = Tag::Err;
        result._value.error = error;

        return result;
    }

    static Result err(E &&error)
    {
        Result result{};

        result._tag = Tag::Err;
        result._value.error = std::move(error);

        return result;
    }

    template <typename M>
    M match(
        std::function<M(T)> on_ok,
        std::function<M(E)> on_error) &&
    {
        switch (_tag)
        {
        case Tag::Ok:
            return on_ok(std::move(_value.ok));
        case Tag::Err:
            return on_error(std::move(_value.error));
        }
    }

    template <typename M>
    Result<M, E> map(
        std::function<M(T)> map) &&
    {
        using R = Result<M, E>;

        switch (_tag)
        {
        case Tag::Ok:
            return R::ok(map(std::move(_value.ok)));
        case Tag::Err:
            return R::err(std::move(_value.error));
        }
    }

    Result(const Result &other) : _tag{other._tag}
    {
        switch (_tag)
        {
        case Tag::Ok:
        {
            _value.ok = other._value.ok;
            break;
        }
        case Tag::Err:
        {
            _value.error = other._value.error;
            break;
        }
        }
    }

    Result(Result &&other) noexcept : _tag{other._tag}
    {
        switch (_tag)
        {
        case Tag::Ok:
        {
            _value.ok = std::move(other._value.ok);
            break;
        }
        case Tag::Err:
        {
            _value.error = std::move(other._value.error);
            break;
        }
        }
    }

    Result &operator=(const Result &other)
    {
        return *this = Result(other);
    }

    Result &operator=(Result &&other) noexcept
    {
        if (this == &other)
            return *this;

        _tag = other._tag;

        switch (_tag)
        {
        case Tag::Ok:
        {
            _value.ok = std::move(other._value.ok);
            break;
        }
        case Tag::Err:
        {
            _value.error = std::move(other._value.error);
            break;
        }
        }

        return *this;
    }

    ~Result()
    {
        switch (_tag)
        {
        case Tag::Ok:
        {
            _value.ok.~T();
            break;
        }
        case Tag::Err:
        {
            _value.error.~E();
            break;
        }
        }
    }

private:
    enum class Tag
    {
        Ok,
        Err
    } _tag;
    union Variant
    {
        T ok;
        E error;

        explicit Variant() {}

        Variant(const Variant &other) = delete;
        Variant(Variant &&other) noexcept = delete;
        Variant &operator=(const Variant &other) = delete;
        Variant &operator=(Variant &&other) noexcept = delete;

        ~Variant() {}
    } _value;

    explicit Result() {}
};

struct None
{
};

template <typename T>
using Option = Result<T, None>;

#endif