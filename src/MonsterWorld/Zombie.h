//
// Created by vvivi on 9/12/2018.
//

#ifndef TEST_CPP11_APP_ZOMBIE_H
#define TEST_CPP11_APP_ZOMBIE_H


#include "Monster.h"

class Zombie : public Monster{
public:
    Zombie(int size, const std::string &name);

    void eat() override;
    void fight();

    void makeNoise();

private:
    int score;
};


#endif //TEST_CPP11_APP_ZOMBIE_H
