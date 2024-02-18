#ifndef _METHOD
#define _METHOD

#include "enum.h"

namespace _
{
    enum class HttpMethod
    {
        Get,
        Post,
        Patch,
        Put,
        Delete
    };
}

class HttpMethod : public Enum<_::HttpMethod>
{
private:
    using Value = _::HttpMethod;

    constexpr explicit HttpMethod(Value section) noexcept : Enum<Value>{section} {}

public:
    static const HttpMethod Get;
    static const HttpMethod Post;
    static const HttpMethod Patch;
    static const HttpMethod Put;
    static const HttpMethod Delete;
};

constexpr HttpMethod HttpMethod::Get = HttpMethod(HttpMethod::Value::Get);
constexpr HttpMethod HttpMethod::Post = HttpMethod(HttpMethod::Value::Post);
constexpr HttpMethod HttpMethod::Patch = HttpMethod(HttpMethod::Value::Patch);
constexpr HttpMethod HttpMethod::Put = HttpMethod(HttpMethod::Value::Put);
constexpr HttpMethod HttpMethod::Delete = HttpMethod(HttpMethod::Value::Delete);

#endif