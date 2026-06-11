#pragma once
#include <windows.h>
#include "Car.h"
#include "PartRegistry.h"
#include "ConsoleUI.h"

class CarAssembler {
public:
    void run() {
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);
        while (true) {
            m_car.carType = m_ui.askCarType(m_registry);
            m_car.engine  = m_ui.askEngine(m_registry);
            m_ui.printSelectionResult(m_car);
        }
    }

private:
    Car          m_car;
    PartRegistry m_registry;
    ConsoleUI    m_ui;
};
