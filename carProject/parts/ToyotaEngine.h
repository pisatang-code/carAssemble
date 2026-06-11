#pragma once
#include "../interfaces/IEngine.h"

class ToyotaEngine : public IEngine {
public:
    std::string getName() const override { return "TOYOTA"; }
    bool isCompatibleWith(const ICarType& carType) const override {
        return carType.getName() != "SUV";
    }
};
