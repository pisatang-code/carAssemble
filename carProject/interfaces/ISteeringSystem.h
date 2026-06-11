#pragma once
#include <string>

class ISteeringSystem {
public:
    virtual std::string getName() const = 0;
    virtual ~ISteeringSystem() = default;
};
