//
// Created by vvivi on 2019/2/21.
//
#include <gtest/gtest.h>
#include <iostream>
using namespace std;

class Base
{
public:
    virtual ~Base(){ cout <<"B::~B()"<<endl; }
};

class Derived : public Base
{
public:
    virtual ~Derived() { cout <<"D::D~()"<<endl; }
};

TEST(delete_boj_array, CASE_1) {
    Base* pBase = new Derived[10];
    delete[] pBase;
}

