#pragma once
#include <string>

class ICarType {
public:
    virtual std::string getName() const = 0;
    virtual ~ICarType() = default;
};
