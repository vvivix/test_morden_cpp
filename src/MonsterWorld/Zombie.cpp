//
// Created by vvivi on 9/12/2018.
//

#include <iostream>
#include "Zombie.h"

void Zombie::eat() {
    power += 1000;
}

Zombie::Zombie(int size, const std::string &name) : Monster(size, name) {}

void Zombie::fight() {
    const auto powerIncrese = size * 3;
    if (power > powerIncrese) {
        power -= powerIncrese;
        score += powerIncrese;
    }


}

void Zombie::makeNoise() {
    std::cout <<"brrrrrrrrrrrrrrrraaaaaaaaaaaaaaaaaaaaaannnnnnnnnnnnnnnnnnnnzzzzzzzzzzzzz" << std::endl;
}
