#include "DataPool/DataPool.hpp"

namespace sensormoniteringhub{
    namespace datapool{
        void DataPool::StartService()
        {
        }

        void DataPool::StopService()
        {
        }

        void DataPool::Initialize()
        {
            std::dynamic_pointer_cast<systemcontext::ComponentRegistry>(
                systemcontext::ComponentRegistry::GetComponent("ComponentRegistry")
            )->RegisterComponent("DataPool", std::make_shared<DataPool>());
            logger::Logger::LOG("DataPool::Initialize", "Initialization successful!");
        }

        void DataPool::Finalize()
        {
        }
    }
}

