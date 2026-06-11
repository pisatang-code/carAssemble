#pragma once
#include "../interfaces/IEngine.h"

class GMEngine : public IEngine {
public:
    std::string getName() const override { return "GM"; }
    bool isCompatibleWith(const ICarType&) const override { return true; }
};
