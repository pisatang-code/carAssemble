#pragma once
#include <string>
#include "ICarType.h"
#include "ISteeringSystem.h"

class IBrakeSystem {
public:
    virtual std::string getName() const = 0;
    virtual bool isCompatibleWithCarType(const ICarType& carType) const = 0;
    virtual bool isCompatibleWithSteering(const ISteeringSystem& steering) const = 0;
    virtual ~IBrakeSystem() = default;
};
