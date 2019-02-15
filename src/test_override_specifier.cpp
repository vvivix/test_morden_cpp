#include "gtest/gtest.h"

#include <iostream>

class VBase {
public:
    virtual ~VBase() = default;

    virtual void callA() { std::cout << "callA from VBase" << std::endl; }

    virtual void callB() { std::cout << "callB from VBase" << std::endl; }
};

class VDerive : public VBase {
public:
    ~VDerive() override = default;

    void callA() override { std::cout << "callA from VDerive" << std::endl; }

    void callB() override { std::cout << "callB from VDerive" << std::endl; }
};

TEST(CPP11, OVERRIDE_SPECIFIER_CASE_1){
    VBase base;
    base.callA();
    base.callB();

    VDerive derive;
    derive.callA();
    derive.callB();

    VBase *pDerive = new VDerive();
    pDerive->callA();
    pDerive->callB();
    delete pDerive;
}