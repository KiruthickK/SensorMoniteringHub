#include <PersistenceManager/PersistenceManager.hpp>
#include <Logger/Logger.hpp>
namespace sensormoniteringhub{
    namespace persistencemanager{
            
            /// @brief 
            void PersistenceManager::StartService(){
                logger::Logger::LOG("PersistenceManager::StartService","Starting service");
                logger::Logger::LOG("PersistenceManager::StartService","Opening DB...");
                if(OpenDataBase()){
                    logger::Logger::LOG("PersistenceManager::StartService","DB opened succesfully");
                }else{
                    logger::Logger::LOG("PersistenceManager::StartService","DB opening Failed!", logger::LOGLEVEL::ERROR_LEVEL);
                }
            }

            /// @brief 
            /// @param sensorDataContainer 
            /// @return 
            bool PersistenceManager::GetRecoveredSensorData(std::vector<sensordatareceiver::SensorData> &sensorDataContainer){
                if(!dbHandler_){
                    logger::Logger::LOG("PersistenceManager::GetRecoveredSensorData", "Database is not open!", logger::LOGLEVEL::ERROR_LEVEL);
                    return false;
                }
                if(CheckTableExistsInDb()){
                    logger::Logger::LOG("PersistenceManager::GetRecoveredSensorData", "Table '"+ dbTableName_ +"' present in the db.");
                    std::string const query{
                        "SELECT sensorid_, timestamp_, motionintensity_, temperature_, batterylevel_, zoneid_ FROM "
                        + dbTableName_ + 
                        " ORDER BY timestamp_ DESC "
                        " LIMIT ? ;"
                    };
                    sqlite3_stmt* stmt{nullptr};
                    if(sqlite3_prepare_v2(dbHandler_, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK){
                        logger::Logger::LOG("PersistenceManager::GetRecoveredSensorData", "Compiling query failed", logger::LOGLEVEL::ERROR_LEVEL);
                        return false;
                    }
                    if(sqlite3_bind_int64(stmt, 1, maxEvents_) != SQLITE_OK){
                        logger::Logger::LOG("PersistenceManager::GetRecoveredSensorData", "Binding query with max events failed", logger::LOGLEVEL::ERROR_LEVEL);
                        return false;
                    }
                    int stepResult{SQLITE_ROW};
                    while((stepResult = sqlite3_step(stmt)) == SQLITE_ROW){
                        sensordatareceiver::SensorData sensorData;
                        sensorData.sensorId_ = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
                        sensorData.timeStamp_ = sqlite3_column_int64(stmt, 1);
                        sensorData.motionIntensity_ = sqlite3_column_double(stmt, 2);
                        sensorData.temperature_ = sqlite3_column_double(stmt, 3);
                        sensorData.batteryLevel_ = sqlite3_column_int(stmt, 4);
                        sensorData.zoneId_ = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
                        sensorDataContainer.push_back(sensorData);
                    }
                    if(stepResult != SQLITE_DONE){
                        logger::Logger::LOG("PersistenceManager::GetRecoveredSensorData", 
                            "Query execution failed: " + std::string(sqlite3_errmsg(dbHandler_)), 
                            logger::LOGLEVEL::ERROR_LEVEL);
                        sqlite3_finalize(stmt);
                        return false;
                    }
                    sqlite3_finalize(stmt);
                }else{
                    logger::Logger::LOG("PersistenceManager::StartService","Data does not Exists in the db");
                }
                if(sensorDataContainer.empty()){
                    logger::Logger::LOG("PersistenceManager::GetRecoveredSensorData", "No data row present to restore!");
                    return false;
                }
                logger::Logger::LOG("PersistenceManager::GetRecoveredSensorData", "Data restored from DB, now clearing th DB data.");
                if(!CleanupData()){
                    logger::Logger::LOG("PersistenceManager::GetRecoveredSensorData", "DB cleanup failed!");
                }
                return true;
            }

            /// @brief method to truncate the database table
            /// @return 
            bool PersistenceManager::CleanupData(){
                std::string const query{
                    "DELETE FROM " + dbTableName_ + ";"
                };
                char* errMsg = nullptr;
                if(sqlite3_exec(dbHandler_, query.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK){
                    logger::Logger::LOG("PersistenceManager::CleanupData", "Table truncate failed! Error details:" + std::string(errMsg), logger::LOGLEVEL::ERROR_LEVEL);
                    return false;
                }
                logger::Logger::LOG("PersistenceManager::CleanupData", "Data stored in db are truncated successfully.");
                return true;
            }

            /// @brief method for opening data base
            /// @return true if success
            bool PersistenceManager::OpenDataBase(){
                // checking whether the directory is present or not, if not present, creating one
                std::error_code ec;
                std::filesystem::create_directories(dbPath_, ec);
                if(ec){
                    logger::Logger::LOG("PersistenceManager::OpenDataBase", "Failed to create directory!", logger::LOGLEVEL::ERROR_LEVEL);
                    return false;
                }
                std::string dbFilePath{dbPath_ + dbName_};
                int resultCode{sqlite3_open(dbFilePath.c_str(), &dbHandler_)};
                if(resultCode == SQLITE_OK){
                    logger::Logger::LOG("PersistenceManager::OpenDataBase", "DB opened successfully!");
                    return true;
                }
                logger::Logger::LOG("PersistenceManager::OpenDataBase", "Failed to open DB!", logger::LOGLEVEL::ERROR_LEVEL);
                return false;
            }

            /// @brief method to check whether the table is present, and table is present with some rows, to retrieve the data
            /// @return true if present, false if not
            bool PersistenceManager::CheckTableExistsInDb(){
                if(!dbHandler_){
                    logger::Logger::LOG("PersistenceManager::CheckTableExistsInDb", "Database is not open!", logger::LOGLEVEL::ERROR_LEVEL);
                    return false;
                }
                std::string queryToCheckTableIsPresent{"SELECT EXISTS (SELECT 1 FROM sqlite_master WHERE type='table' AND name=?);"};
                sqlite3_stmt* stmt = nullptr;
                int resultCode = sqlite3_prepare_v2(dbHandler_, queryToCheckTableIsPresent.c_str(), -1, &stmt, nullptr);
                if(resultCode != SQLITE_OK){
                    logger::Logger::LOG("PersistenceManager::CheckTableExistsInDb", "Got Error, details: "+ std::string(sqlite3_errmsg(dbHandler_)), logger::LOGLEVEL::ERROR_LEVEL);
                    return false;
                }
                if(sqlite3_bind_text(stmt, 1, dbTableName_.c_str(), -1, SQLITE_STATIC) != SQLITE_OK){
                    logger::Logger::LOG("PersistenceManager::CheckTableExistsInDb", "Got Error, details: "+ std::string(sqlite3_errmsg(dbHandler_)), logger::LOGLEVEL::ERROR_LEVEL);
                    sqlite3_finalize(stmt);
                    return false;
                }
                bool exists = false;
                if(sqlite3_step(stmt) == SQLITE_ROW){
                    exists = sqlite3_column_int(stmt, 0) == 1;
                }
                sqlite3_finalize(stmt);
                return exists;
            }
            
            /// @brief method to create table in the db
            /// @return 
            bool PersistenceManager::CreateTableInDb(){
                if(!dbHandler_){
                    logger::Logger::LOG("PersistenceManager::CreateTableInDb", "Database is not open, persisting sensor data failed", logger::LOGLEVEL::ERROR_LEVEL);
                    return false;
                }
                std::string query{      
                    "CREATE TABLE IF NOT EXISTS "
                    + dbTableName_ + 
                    " ("
                    "  id               INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "  sensorid_        TEXT    NOT NULL,                  "
                    "  timestamp_        INTEGER,                          "
                    "  motionintensity_ REAL,                              "
                    "  temperature_      REAL,                             "
                    "  batterylevel_    INTEGER,                           "
                    "  zoneid_          TEXT                              "
                    ");"
                };
                char* errMsg = nullptr;
                if(sqlite3_exec(dbHandler_, query.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK){
                    logger::Logger::LOG("PersistenceManager::CreateTableInDb", "Table creation failed! Error details:" + std::string(errMsg), logger::LOGLEVEL::ERROR_LEVEL);
                    return false;
                }
                logger::Logger::LOG("PersistenceManager::CreateTableInDb", "Table created successfully!");
                return true;
            }
            
            void PersistenceManager::SetMaxEvents(uint64_t maxEvents){
                maxEvents_ = maxEvents;
            }


            /// @brief method to transfer the whole data in vector to db during application stop service
            /// @param sensorDataContainer 
            void PersistenceManager::PersistSensorData(std::vector<sensordatareceiver::SensorData> const &sensorDataContainer){
                if(!dbHandler_){
                    logger::Logger::LOG("PersistenceManager::PersistSensorData", "Database is not open, persisting sensor data failed", logger::LOGLEVEL::ERROR_LEVEL);
                    return;
                }
                if(!CheckTableExistsInDb()){
                    logger::Logger::LOG("PersistenceManager::PersistSensorData", "Table does not exists in DB, creating a table!");
                    if(!CreateTableInDb()){
                        logger::Logger::LOG("PersistenceManager::PersistSensorData", "Unable to create table, persisting sensor data failed", logger::LOGLEVEL::ERROR_LEVEL);
                        return;
                    }
                }
                std::string const query{
                    "INSERT INTO "
                    + dbTableName_ +
                    "(sensorid_, timestamp_, motionintensity_, temperature_, batterylevel_, zoneid_) "
                    "VALUES (?, ?, ?, ?, ?, ?);"
                };
                sqlite3_stmt* stmt = nullptr;
                if(sqlite3_prepare_v2(dbHandler_, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK){
                    logger::Logger::LOG("PersistenceManager::PersistSensorData", "Query compliation failed, persisting sensor data failed", logger::LOGLEVEL::ERROR_LEVEL);
                    return;
                }
                bool isFailedFlag{false};
                /**
                 * we are using transaction because, if we go without transaction, for every step() api, we will be calling write to disk
                 * So if the data is large, like 10000, that many disk writes will happen, and make the process take more time.
                 * While using transactions, regardless of how many rows we want to write into db
                 * the disk write will happen all at once with commit; or all will be discarded with rollback;
                 */
                sqlite3_exec(dbHandler_, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);
                for(auto const& sensorData : sensorDataContainer){
                    if(sqlite3_bind_text(stmt,   1, sensorData.sensorId_.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK){
                        logger::Logger::LOG("PersistenceManager::PersistSensorData", "Binding data failed, failed to persist sensor data to db!", logger::LOGLEVEL::ERROR_LEVEL);
                        isFailedFlag = true;
                        break;
                    }
                    if(sqlite3_bind_int64(stmt, 2, sensorData.timeStamp_) != SQLITE_OK){
                        logger::Logger::LOG("PersistenceManager::PersistSensorData", "Binding data failed, failed to persist sensor data to db!", logger::LOGLEVEL::ERROR_LEVEL);
                        isFailedFlag = true;
                        break;
                    }
                    if(sqlite3_bind_double(stmt, 3, sensorData.motionIntensity_) != SQLITE_OK){
                        logger::Logger::LOG("PersistenceManager::PersistSensorData", "Binding data failed, failed to persist sensor data to db!", logger::LOGLEVEL::ERROR_LEVEL);
                        isFailedFlag = true;
                        break;
                    }
                    if(sqlite3_bind_double(stmt, 4, sensorData.temperature_) != SQLITE_OK){
                        logger::Logger::LOG("PersistenceManager::PersistSensorData", "Binding data failed, failed to persist sensor data to db!", logger::LOGLEVEL::ERROR_LEVEL);
                        isFailedFlag = true;
                        break;
                    }
                    if(sqlite3_bind_int(stmt, 5, sensorData.batteryLevel_) != SQLITE_OK){
                        logger::Logger::LOG("PersistenceManager::PersistSensorData", "Binding data failed, failed to persist sensor data to db!", logger::LOGLEVEL::ERROR_LEVEL);
                        isFailedFlag = true;
                        break;
                    }
                    if(sqlite3_bind_text(stmt,   6, sensorData.zoneId_.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK){
                        logger::Logger::LOG("PersistenceManager::PersistSensorData", "Binding data failed, failed to persist sensor data to db!", logger::LOGLEVEL::ERROR_LEVEL);
                        isFailedFlag = true;
                        break;
                    }

                    if(sqlite3_step(stmt) != SQLITE_DONE){
                        logger::Logger::LOG("PersistenceManager::PersistSensorData", "Query execution failed, persisting sensor data failed", logger::LOGLEVEL::ERROR_LEVEL);
                        isFailedFlag = true;
                        break;
                    }
                    sqlite3_reset(stmt);
                }
                if(!isFailedFlag){
                    sqlite3_exec(dbHandler_, "COMMIT;", nullptr, nullptr, nullptr);
                    logger::Logger::LOG("PersistenceManager::PersistSensorData", "persisting sensor data completed! Stored " + std::to_string(sensorDataContainer.size()) + " sensor datas!");
                } else {
                    sqlite3_exec(dbHandler_, "ROLLBACK;", nullptr, nullptr, nullptr);
                }
                sqlite3_finalize(stmt);
            }

            /// @brief method for stopping service : closing the db connection
            void PersistenceManager::StopService(){
                logger::Logger::LOG("PersistenceManager::StopService", "closing database");
                if(sqlite3_close(dbHandler_) != SQLITE_OK){
                    logger::Logger::LOG("PersistenceManager::StopService", "closing database failed", logger::LOGLEVEL::ERROR_LEVEL);
                }
            }
    }
}