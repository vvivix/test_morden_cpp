//
// Created by vvivi on 9/12/2018.
//

#include "gtest/gtest.h"
#include <iostream>

std::tuple<char, int, bool> mytuple() {
    char a = 'a';
    int i = 123;
    bool b = true;
    return std::make_tuple(a, i, b);
}

TEST(CPP17_tuple, CASE1)
{
    char a; int i; bool b;
    std::tie(a, i, b) = mytuple();

    std::cout << "a = " << a << " i = " << i << " b = " << b << std::endl;
    FAIL()<<"test failed...";
}