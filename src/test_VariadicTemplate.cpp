//
// Created by vvivi on 2018/12/5.
//
#include "gtest/gtest.h"
#include <string>
#include <iostream>
#include <memory>

template <typename T>
std::string to_string(const T& v) {
    std::stringstream ss;
    ss << v;
    return ss.str();
}
std::vector<std::string> to_string_vec() {
    return {};
};

class hex {
public:
    explicit hex(uint32_t v) {
        mValue = v;
    }

public:
    uint32_t mValue;
};

std::ostream& operator<< (std::ostream&os, hex h) {
    os << "0x" << std::hex<< h.mValue;
}

template <typename T1, typename ... T>
std::vector<std::string> to_string_vec(const T1& v1, const T& ... t) {
    std::vector<std::string> vec;

    vec.push_back(to_string(v1));

    auto reminder = to_string_vec(t...);
    vec.insert(vec.end(), reminder.begin(), reminder.end());
    return vec;
}

TEST(CPP_VariadicTemplate, Case1) {
    auto vec = to_string_vec(123, "Hello", 1.1, 2.3f, 3, 'a', hex(129));

    for (auto const &o : vec) {
        std::cout << o << '\n';
    }
}


template <typename ... T>
auto foo(T&& ... args) {
    return std::make_tuple(args...);
}

class bar {
    int a;
    double  b;
    std::string s;
public:
    bar() {
        a = 0;
        b = 1.1;
        s = "test string value.";
    }

    friend  std::ostream& operator << (std::ostream& os, const bar & b) {
        os << "ostream from bar :";
        os << "a = " << b.a << "    ";
        os << "b = " << b.b << "    ";
        os << "s = " << b.s << "    ";
    }

    friend  std::ostream& operator << (std::ostream& os, const bar * b) {
        os << "ostream from bar* :";
        os << "a = " << b->a << "    ";
        os << "b = " << b->b << "    ";
        os << "s = " << b->s << "    ";
    }
};

TEST(CPP_VariadicTemplate, Case2) {
    auto pb = std::make_shared<bar>();
    auto triple = foo(bar(), "1223", pb);
    std::cout << "get first value : " << std::get<0>(triple) << std::endl;
    std::cout << "get second value : "<< std::get<1>(triple) << std::endl;
    std::cout << "get third value : " << std::get<2>(triple) << std::endl;
}
