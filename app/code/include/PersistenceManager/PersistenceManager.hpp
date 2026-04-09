#include <Events/IEvents.hpp>
#include <sqlite3.h>
#include <iostream>
#include <filesystem>

namespace sensormoniteringhub{
    namespace persistencemanager{
        class PersistenceManager : public IEvents {
        private:
            std::string const dbPath{"../../data/"};
            std::string const dbName{"smh.db"};
        public:
            virtual void StartService();
            virtual void StopService();
            static void Initialize();
            static void Finalize();
        };
    }
}