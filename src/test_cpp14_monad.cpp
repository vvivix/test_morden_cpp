//
// Created by vvivi on 9/13/2018.
//

#include <gtest/gtest.h>

using namespace std;

struct Address {
    string *name_of_house = nullptr;
};

struct Person {
    Address *address;
};

template<typename T>
class maybe;

template<typename T>
maybe<T> __maybe(T *context) {
    return maybe<T>(context);
};

template<typename T>
class maybe {
    T *context;
public:
    maybe(T *context) : context(context) {}

    template<typename Func>
    auto with(Func evaluator) {
        return context == nullptr ? nullptr : __maybe(evaluator(context));
    }

    template<typename Func>
    auto exec(Func evaluator) {
        if (context) evaluator(context);
        return *this;
    }
};

void printf_name_of_house(Person *p) {
//    if (p && p->address && p->address->name_of_house)
//        cout << "name is : " << *p->address->name_of_house << endl;
    auto z = maybe<Person>(p)
            .with([](auto x) { return x->address; })
            .with([](auto x) { return x->name_of_house; })
            .exec([](auto x) { cout << "name is :" << *x << endl; });
}

TEST(CPP_DESIGN_PARTTERN, MONAD) {
    Person person;
    person.address = new Address{new string("oh my big house")};

    printf_name_of_house(&person);

    delete person.address->name_of_house;
    delete person.address;
}
