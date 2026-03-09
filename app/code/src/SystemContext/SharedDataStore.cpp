#include "SystemContext/SharedDataStore.hpp"

namespace sensormoniteringhub{
    namespace systemcontext{
        void SharedDataStore::StartService()
        {
        }

        void SharedDataStore::StopService()
        {
        }

        void SharedDataStore::Initialize()
        {
            std::dynamic_pointer_cast<systemcontext::ComponentRegistry>(
                systemcontext::ComponentRegistry::GetComponent("ComponentRegistry")
            )->RegisterComponent("SharedDataStore", std::make_shared<SharedDataStore>());
            logger::Logger::LOG("SharedDataStore::Initialize", "Initialization successful!");
        }

        void SharedDataStore::Finalize()
        {
        }
    }
}
