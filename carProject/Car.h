#pragma once
#include <memory>
#include "interfaces/ICarType.h"
#include "interfaces/IEngine.h"
#include "interfaces/IBrakeSystem.h"
#include "interfaces/ISteeringSystem.h"

class Car {
public:
    std::shared_ptr<ICarType>        carType;
    std::shared_ptr<IEngine>         engine;
    std::shared_ptr<IBrakeSystem>    brakeSystem;
    std::shared_ptr<ISteeringSystem> steeringSystem;
};
