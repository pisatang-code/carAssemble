#pragma once
#include <iostream>
#include "Car.h"
#include "PartRegistry.h"
#include "ConsoleUI.h"

class CarAssembler {
public:
    void run() {
        m_car.carType = m_ui.askCarType(m_registry);
        std::cout << "\n" << m_car.carType->getName() << " 를 선택하셨습니다.\n";
    }

private:
    Car          m_car;
    PartRegistry m_registry;
    ConsoleUI    m_ui;
};
