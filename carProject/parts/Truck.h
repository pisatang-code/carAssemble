#pragma once
#include "../interfaces/ICarType.h"

class Truck : public ICarType {
public:
    std::string getName() const override { return "Truck"; }
};
