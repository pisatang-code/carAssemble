#pragma once
#include <memory>
#include "interfaces/ICarType.h"
#include "interfaces/IEngine.h"

class Car {
public:
    std::shared_ptr<ICarType> carType;
    std::shared_ptr<IEngine>  engine;
};
