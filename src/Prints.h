
#ifndef PRINT_OVERLOADS_H
#define PRINT_OVERLOADS_H

#include <set>
#include <map>
#include <vector>
#include <list>
#include <tuple>
#include <iostream>
#include <utility>

template <typename T, typename U>
std::ostream & operator << (std::ostream &os, const std::pair<T, U> &p);
template <typename T>
std::ostream & operator << (std::ostream &os, const std::vector<T> &v);
template <typename T>
std::ostream & operator << (std::ostream &os, const std::set<T> &s);
template <typename T, typename U>
std::ostream & operator << (std::ostream &os, const std::map<T, U> &m);

template <int IDX, int MAX, typename... Args>
struct PRINT_TUPLE {
    static void print(std::ostream & strm, const std::tuple<Args...> & t) {
        strm << std::get<IDX>(t) << (IDX + 1 == MAX ? "" : ",");
        PRINT_TUPLE<IDX + 1, MAX, Args...>::print(strm, t);
    }
};

// partial specialization to end recursion
template <int MAX, typename... Args>
struct PRINT_TUPLE<MAX, MAX, Args...> {
    static void print(std::ostream & strm, const std::tuple<Args...> & t) {
    }
};

template <typename... Args>
std::ostream & operator << (std::ostream & strm, 
                            const std::tuple<Args...> & t) {
    strm << '[';
    PRINT_TUPLE<0, sizeof...(Args), Args...>::print(strm, t);
    return strm << ']';
}




/* __uint128_t overload */

std::ostream & operator << (std::ostream & os, __uint128_t s)
{
    if ((uint64_t)(s>>64)) os << (uint64_t)(s>>64);
    os << (uint64_t)s;
    return os;
}



template <typename T, typename U>
std::ostream & operator << (std::ostream &os, const std::pair<T, U> &p)
{
    os << '(' << p.first << ',' << p.second << ')';
    return os;
}

template <typename T, std::size_t N>
std::ostream & operator << (std::ostream &os, const std::array<T,N> &a)
{
    os << '[';
    for (int i = 0; i < N; ++i)
        os << a[i] << ',';
    os << "\b]";
    return os;
}

template <typename T>
std::ostream & operator << (std::ostream &os, const std::vector<T> &v)
{
    if (!v.empty()) {
        os << '[';
        for (const T &it : v)
            os << it << ',';
        os << "\b]";
    }
    return os;
}

template <typename T>
std::ostream & operator << (std::ostream &os, const std::list<T> &v)
{
    if (!v.empty()) {
        os << '[';
        for (auto it = v.cbegin(); it != v.cend(); ++it)
            os << *it << (std::next(it) == v.cend() ? "" : "->");
        os << ']';
    }
    return os;
}

template <typename T>
std::ostream & operator << (std::ostream &os, const std::set<T> &s)
{
    if (!s.empty()) {
        os << '(';
        for (const T &it : s)
            os << it << ',';
        os << "\b)";
    }
    return os;
}

template <typename T, typename U>
std::ostream & operator << (std::ostream &os, const std::map<T, U> &m)
{
    if (!m.empty()) {
        os << '[';
        std::for_each(m.begin(), m.end(),
                [&os] (const typename std::map<T, U>::value_type &it) { 
                    os << "(" << it.first << "," << it.second << "),"; 
                } );
        os << "\b]";
    }
    return os;
}

#endif

