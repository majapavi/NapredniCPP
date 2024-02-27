#pragma once
#include <string>
#include <array>
#include <fmt/format.h>

class Log {
    std::string _sender;
    int         _priority;
    std::array<char, 3> _code;
    std::string _message;
public:
    Log(std::string sender, int priority, std::array<char, 3> code,
        std::string message);

    std::string const& sender() { return _sender; }
    int& priority() { return _priority; } //prioritet se moze mijenjati i izvan funkcije
    std::array<char, 3> const& code() { return _code; }
    std::string const& message() { return _message; }

    std::string const& sender() const { return _sender; }
    int const& priority() const { return _priority; }
    std::array<char, 3> const& code() const { return _code; }
    std::string const& message() const { return _message; }
};

template <>
struct std::tuple_size<Log> {
    static constexpr int value = 4;
};

template <>
struct std::tuple_element<0, Log> {
    using type = const std::string;
};

template <>
struct std::tuple_element<1, Log> {
    using type = int;       //prioritet se moze mijenjati i izvan funkcije
};

template <>
struct std::tuple_element<2, Log> {
    using type = const std::array<char, 3>;
};

template <>
struct std::tuple_element<3, Log> {
    using type = const std::string;
};

template <std::size_t N> decltype(auto) get(Log const&);
template <std::size_t N> decltype(auto) get(Log&);
template <std::size_t N> decltype(auto) get(Log&&);

/////////////////////////////////////////////////////////////////////////

template <>
inline decltype(auto) get<0>(Log const& objekt) {
    return objekt.sender();
}

template <>
inline decltype(auto) get<1>(Log const& objekt) {
    return objekt.priority();
}

template <>
inline decltype(auto) get<2>(Log const& objekt) {
    return objekt.code();
}

template <>
inline decltype(auto) get<3>(Log const& objekt) {
    return objekt.message();
}

/////////////////////////////////////////////////////////////////////////

template <>
inline decltype(auto) get<0>(Log& objekt) {
    return objekt.sender();
}

template <>
inline decltype(auto) get<1>(Log& objekt) {
    return objekt.priority();
}

template <>
inline decltype(auto) get<2>(Log& objekt) {
    return objekt.code();
}

template <>
inline decltype(auto) get<3>(Log& objekt) {
    return objekt.message();
}

/////////////////////////////////////////////////////////////////////////

template <>
inline decltype(auto) get<0>(Log&& objekt) {
    return objekt.sender();
}

template <>
inline decltype(auto) get<1>(Log&& objekt) {
    return objekt.priority();
}

template <>
inline decltype(auto) get<2>(Log&& objekt) {
    return objekt.code();
}

template <>
inline decltype(auto) get<3>(Log&& objekt) {
    return objekt.message();
}
