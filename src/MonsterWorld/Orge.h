//
// Created by vvivi on 9/12/2018.
//

#ifndef TEST_CPP11_APP_ORGE_H
#define TEST_CPP11_APP_ORGE_H


#include "Monster.h"

class Orge : public Monster{
public:
    Orge(int size, const std::string &name);

    void eat() override;
};


#endif //TEST_CPP11_APP_ORGE_H
