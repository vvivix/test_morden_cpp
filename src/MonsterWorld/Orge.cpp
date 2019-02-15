//
// Created by vvivi on 9/12/2018.
//

#include "Orge.h"

void Orge::eat() {
    power += 5000;
}

Orge::Orge(int size, const std::string &name) : Monster(size, name) {}
