#pragma once
#include "../interfaces/ICarType.h"

class SUV : public ICarType {
public:
    std::string getName() const override { return "SUV"; }
};
