#pragma once
#include <string>
#include "ICarType.h"

class IEngine {
public:
    virtual std::string getName() const = 0;
    virtual bool isCompatibleWith(const ICarType& carType) const = 0;
    virtual ~IEngine() = default;
};
