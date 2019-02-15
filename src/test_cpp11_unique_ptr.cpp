//
// Created by vvivi on 2018/11/28.
//
#include "gtest/gtest.h"

#include <iostream>
#include <memory>

class Investment {
public:
    Investment() { std::cout << "Investment Construction" << std::endl; }
    ~Investment() { std::cout << "Investment Destruction" << std::endl; }
    void foo() {std::cout << "call foo" << std::endl;}
};

TEST(CPP11, UNIQUR_PTR_CASE_0) {
    auto pIvt = std::unique_ptr<Investment>(new Investment);
    pIvt->foo();
}

TEST(CPP11, UNIQUR_PTR_CASE_1) {
    auto pIvt = std::make_unique<Investment>();
    pIvt->foo();
}

TEST(CPP11, UNIQUR_PTR_CASE_2) {
    auto pArray = std::make_unique<int[]>(10);
    pArray[0] = 1;
    pArray[1] = 1;
    for (auto i = 2; i < 10; i++) {
        pArray[i] = pArray[i-1] + pArray[i-2];
    }

    for (auto i = 0; i < 10; i++) {
        std::cout << pArray[i] << " ";
    }
    std::cout << std::endl;
}

TEST(CPP11, UNIQUR_PTR_CASE_3) {
    auto pName = std::make_unique<std::string>("Alice");
    pName = std::make_unique<std::string>("Bob");
    std::cout << *pName << std::endl;
}

TEST(CPP11, SHARED_PTR_CASE_1) {
    auto pIvt = std::make_shared<Investment>();
    auto pIvt2 = pIvt;
    pIvt2->foo();
}