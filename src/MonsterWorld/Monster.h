//
// Created by vvivi on 9/12/2018.
//

#ifndef TEST_CPP11_APP_MONSTER_H
#define TEST_CPP11_APP_MONSTER_H


#include <string>
#include <tuple>
#include <ostream>

class Monster {
    std::string name;
protected:
    int power;
    int size;
public:
    Monster(int size, const std::string &name);

    int getSize() const;

    void setSize(int size);

    const std::string &getName() const;

    std::string setName(const std::string &name);

    virtual void eat() = 0;

    bool operator==(const Monster &rhs) const {
        return std::tie(size, power, name) == std::tie(rhs.size, rhs.power, rhs.name);
    }

    bool operator!=(const Monster &rhs) const {
        return !(rhs == *this);
    }

    bool operator<(const Monster &rhs) const;

    bool operator>(const Monster &rhs) const;

    bool operator<=(const Monster &rhs) const;

    bool operator>=(const Monster &rhs) const;

    friend std::ostream &operator<<(std::ostream &os, const Monster &monster);
};


#endif //TEST_CPP11_APP_MONSTER_H
