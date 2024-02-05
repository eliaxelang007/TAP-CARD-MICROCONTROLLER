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

        result.tag = Tag::Ok;
        result.value.ok = ok;

        return result;
    }

    static Result ok(T &&ok)
    {
        Result result{};

        result.tag = Tag::Ok;
        result.value.ok = std::move(ok);

        return result;
    }

    static Result err(const E &err)
    {
        Result result{};

        result.tag = Tag::Err;
        result.value.err = err;

        return result;
    }

    static Result err(E &&err)
    {
        Result result{};

        result.tag = Tag::Err;
        result.value.err = std::move(err);

        return result;
    }

    template <typename M>
    M match(
        std::function<M(T)> on_ok,
        std::function<M(E)> on_err) &&
    {
        switch (tag)
        {
        case Tag::Ok:
            return on_ok(std::move(value.ok));
        case Tag::Err:
            return on_err(std::move(value.err));
        }
    }

    Result(const Result &other) : tag{other.tag}
    {
        switch (tag)
        {
        case Tag::Ok:
        {
            value.ok = other.value.ok;
            break;
        }
        case Tag::Err:
        {
            value.err = other.value.err;
            break;
        }
        }
    }

    Result(Result &&other) noexcept : tag{other.tag}
    {
        switch (tag)
        {
        case Tag::Ok:
        {
            value.ok = std::move(other.value.ok);
            break;
        }
        case Tag::Err:
        {
            value.err = std::move(other.value.err);
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

        tag = other.tag;

        switch (tag)
        {
        case Tag::Ok:
        {
            value.ok = std::move(other.value.ok);
            break;
        }
        case Tag::Err:
        {
            value.err = std::move(other.value.err);
            break;
        }
        }

        return *this;
    }

    ~Result()
    {
        switch (tag)
        {
        case Tag::Ok:
        {
            value.ok.~T();
            break;
        }
        case Tag::Err:
        {
            value.err.~E();
            break;
        }
        }
    }

private:
    enum class Tag
    {
        Ok,
        Err
    } tag;
    union Variant
    {
        T ok;
        E err;

        explicit Variant() {}

        Variant(const Variant &other) = delete;
        Variant(Variant &&other) noexcept = delete;
        Variant &operator=(const Variant &other) = delete;
        Variant &operator=(Variant &&other) noexcept = delete;

        ~Variant() {}
    } value;

    explicit Result() {}
};

#endif