#pragma once
#include "../interfaces/ICarType.h"

class Sedan : public ICarType {
public:
    std::string getName() const override { return "Sedan"; }
};
