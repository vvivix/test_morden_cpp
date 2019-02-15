//
// Created by vvivi on 9/12/2018.
//

#include "gtest/gtest.h"
#include <iostream>

class call_me {
public:
    bool imReal(const char* s) {
        std::cout << "call_me::imReal : " << s << std::endl;
        return true;
    }
};

template <class T, typename Result, typename Param>
Result TestCallClassMemberFunc(
        T* object, Result (T::*method)(Param), Param p) {
    return (object->*method)(p);
}

TEST(CPP11, addr_of_member)
{
    call_me cm;
    TestCallClassMemberFunc(&cm, &call_me::imReal, "par");
    ASSERT_TRUE(cm.imReal("1212"));
}


TEST(CPP11, if_any)
{
    uint bytes[5] = {1,0,4,4,6};
    auto z = std::any_of(bytes, bytes + 5, [](auto x) {return x % 2 == 1;});
    ASSERT_TRUE(z);
}


template <typename...T>
void doVariadicPrint1(T... args) {
    std::cout << sizeof...(args) << std::endl;
}

TEST(CPP11, VariadicTemplates1)
{
    doVariadicPrint1(1);
    doVariadicPrint1(2,3,4,0.1);
}

TEST(CPP11, Split_Method_1) {
    std::string text = "Let me split this into words               ";

    std::istringstream iss(text);
    std::vector<std::string> results((std::istream_iterator<std::string>(iss)),
                                     std::istream_iterator<std::string>());
    for (auto& s : results)
        std::cout << "["  << s << "]" << std::endl;
}

TEST(CPP11, Split_Method_2) {
    std::string text = "Let me split this into words               ";

    std::istringstream iss(text);
    std::vector<std::string> results(std::istream_iterator<std::string>{iss},
                                     std::istream_iterator<std::string>());
    for (auto& s : results)
        std::cout << "["  << s << "]" << std::endl;
}

TEST(CPP11, addr_of_array) {
    uint8_t dummy[16] = {0};
    auto a = dummy;
    auto b = (uint8_t*)&dummy;
    ASSERT_EQ(a, b);
}