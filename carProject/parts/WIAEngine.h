#pragma once
#include "../interfaces/IEngine.h"

class WIAEngine : public IEngine {
public:
    std::string getName() const override { return "WIA"; }
    bool isCompatibleWith(const ICarType& carType) const override {
        return carType.getName() != "Truck";
    }
};
