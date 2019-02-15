//
// Created by vvivi on 9/12/2018.
//

#include <gtest/gtest.h>
#include "MonsterWorld/Zombie.h"

TEST(MONSTER_WORLD, SCENCE_1) {
    std::cout << "welcome to the world ruled by monsters!" << std::endl;
    Zombie zombie(100, "Bob");
    std::string prev_name = zombie.setName("Zed");
    std::string name =  zombie.getName();
    zombie.makeNoise();

}

