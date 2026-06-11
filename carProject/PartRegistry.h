#pragma once
#include <vector>
#include <memory>
#include "interfaces/ICarType.h"
#include "interfaces/IEngine.h"
#include "interfaces/IBrakeSystem.h"
#include "interfaces/ISteeringSystem.h"
#include "parts/Sedan.h"
#include "parts/SUV.h"
#include "parts/Truck.h"

class PartRegistry {
public:
    PartRegistry() {
        m_carTypes = {
            std::make_shared<Sedan>(),
            std::make_shared<SUV>(),
            std::make_shared<Truck>()
        };
    }

    const std::vector<std::shared_ptr<ICarType>>&        carTypes()        const { return m_carTypes; }
    const std::vector<std::shared_ptr<IEngine>>&         engines()         const { return m_engines; }
    const std::vector<std::shared_ptr<IBrakeSystem>>&    brakeSystems()    const { return m_brakeSystems; }
    const std::vector<std::shared_ptr<ISteeringSystem>>& steeringSystems() const { return m_steeringSystems; }

private:
    std::vector<std::shared_ptr<ICarType>>        m_carTypes;
    std::vector<std::shared_ptr<IEngine>>         m_engines;
    std::vector<std::shared_ptr<IBrakeSystem>>    m_brakeSystems;
    std::vector<std::shared_ptr<ISteeringSystem>> m_steeringSystems;
};
