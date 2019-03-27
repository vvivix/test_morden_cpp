// from https://www.youtube.com/watch?v=rF3TfkknQK0&list=PLk6CEY9XxSIDZhQURp6d8Sgp-A0yKKDKV&index=4
#include <gtest/gtest.h>

using namespace std;

class Car {
    int position;
    std::vector<class Observer *> observerList;
public:
    Car() : position(0) {}

    int getPosition() { return position; }

    void setPosition(int pos) {
        position = pos;
        notify();
    }

    void attach(Observer *obs) {
        observerList.emplace_back(obs);
    }

    void detach(Observer *obs) {
        observerList.erase(std::remove(observerList.begin(), observerList.end(), obs), observerList.end());
    }

    void notify();
};

class Observer {
    Car *_car;
public:
    explicit Observer(Car *car) {
        _car = car;
        _car->attach(this);
    }

    virtual void update() = 0;

protected:
    Car *getCar() { return _car; }
};

class LeftObserver : public Observer {
public:
    explicit LeftObserver(Car *car) : Observer(car) {}

    void update() override {
        if ( getCar()->getPosition() < 0 ) {
            std::cout << "left side" << "\n";
        }
    }
};

class RightObserver : public Observer {
public:
    explicit RightObserver(Car *car) : Observer(car) {}

    void update() override {
        if ( getCar()->getPosition() > 0 ) {
            std::cout << "right side" << "\n";
        }
    }
};

class MiddleObserver : public Observer {
public:
    explicit MiddleObserver(Car *car) : Observer(car) {}

    void update() override {
        if ( getCar()->getPosition() == 0 ) {
            std::cout << "middle side" << "\n";
        }
    }
};

void Car::notify() {
    for (auto ob : observerList) {
        ob->update();
    }
}


TEST(Observer, Case1) {
    auto car = new Car;

    LeftObserver leftObserver(car);
    RightObserver rightObserver(car);
    MiddleObserver middleObserver(car);

    std::cout << "hit left/right button to drive a car in your city!!!! and press ESC to close" << "\n";

    char pressButton;
    bool breakLoop = false;

    while (!breakLoop) {
        cin >> pressButton;

        switch (pressButton) {
            case 'l': { // l --> pressed for left
                car->setPosition(-1);
                break;
            }
            case 'c': { // c --> pressed for center
                car->setPosition(0);
                break;
            }
            case 'r' : { // r --> pressed for right
                car->setPosition(1);
                break;
            }
            case 'b': { // b --> press for break;
                breakLoop = true;
                break;
            }
            default: {
                cout << "your input " << int(pressButton) << "\n";
                break;
            }
        }
    }
}
