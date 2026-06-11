#pragma once
#include <vector>
#include <memory>
#include "interfaces/ICarType.h"
#include "interfaces/IEngine.h"
#include "parts/Sedan.h"
#include "parts/SUV.h"
#include "parts/Truck.h"
#include "parts/GMEngine.h"
#include "parts/ToyotaEngine.h"
#include "parts/WIAEngine.h"

class PartRegistry {
public:
    PartRegistry() {
        m_carTypes = {
            std::make_shared<Sedan>(),
            std::make_shared<SUV>(),
            std::make_shared<Truck>()
        };
        m_engines = {
            std::make_shared<GMEngine>(),
            std::make_shared<ToyotaEngine>(),
            std::make_shared<WIAEngine>()
        };
    }

    const std::vector<std::shared_ptr<ICarType>>& carTypes() const { return m_carTypes; }
    const std::vector<std::shared_ptr<IEngine>>&  engines()  const { return m_engines; }

private:
    std::vector<std::shared_ptr<ICarType>> m_carTypes;
    std::vector<std::shared_ptr<IEngine>>  m_engines;
};
