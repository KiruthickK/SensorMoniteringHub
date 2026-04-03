#include "../nlohmann/json.hpp"
#include <chrono>
#include <cstdint>
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <thread>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/prctl.h>
#include <csignal>
#include <thread>
#include <atomic>

#include <filesystem>
#include <fstream>
#include <chrono>

namespace fs = std::filesystem;
std::atomic<bool> runFlag;
class UDPSender
{
public:

    UDPSender(const std::string& ip, int port)
        : serverIp_(ip), serverPort_(port)
    {}

    bool Start()
    {
        sock_ = socket(AF_INET, SOCK_DGRAM, 0);
        if (sock_ < 0)
        {
            perror("socket");
            return false;
        }

        serverAddr_.sin_family = AF_INET;
        serverAddr_.sin_port = htons(serverPort_);

        inet_pton(AF_INET, serverIp_.c_str(), &serverAddr_.sin_addr);

        return true;
    }

    bool PerformHandshake(const std::string& clientName)
    {
        std::string handshake = "Hello from " + clientName;

        sendto(sock_,
               handshake.c_str(),
               handshake.size(),
               0,
               (sockaddr*)&serverAddr_,
               sizeof(serverAddr_));

        char buffer[1024];

        socklen_t len = sizeof(serverAddr_);

        int bytes = recvfrom(sock_,
                             buffer,
                             sizeof(buffer),
                             0,
                             (sockaddr*)&serverAddr_,
                             &len);

        if (bytes <= 0)
            return false;

        std::string reply(buffer, buffer + bytes);

        std::cout << "Server reply: " << reply << std::endl;

        return reply == "READY";
    }



void SendData()
{
    std::vector<std::string> jsonFiles;

    std::string inputDir = "./SensorDataSource";

    // Collect all json files
    for (const auto& entry : fs::directory_iterator(inputDir))
    {
        if (entry.is_regular_file() && entry.path().extension() == ".json")
        {
            jsonFiles.push_back(entry.path().string());
        }
    }

    if (jsonFiles.empty())
    {
        std::cout << "No JSON files found in inputs directory\n";
        return;
    }

    std::cout << "Found " << jsonFiles.size() << " JSON files\n";
    bool endFlag{false};
    while (!endFlag) // not using runflag here because, we might miss sending BYE message
    {
        for (const auto& file : jsonFiles)
        {
            std::ifstream in(file);
            nlohmann::json j;
            if (!in)
            {
                std::cout << "Failed to open file: " << file << "\n";
                continue;
            }
            try {
                in >> j;
            } catch (const std::exception& e) {
                std::cout << "JSON parse error: " << e.what() << "\n";
                continue;
            }
            if(j.contains("timestamp")){
                auto now = std::chrono::system_clock::now().time_since_epoch();
                j["timestamp"] = std::chrono::duration_cast<std::chrono::milliseconds>(now).count();
            }
            std::string msg{
                j.dump()
            };
            if(runFlag.load() == false){
                msg = "BYE";
                endFlag = true;
            }

            std::vector<uint8_t> bytes(msg.begin(), msg.end());

            sendto(sock_,
                   bytes.data(),
                   bytes.size(),
                   0,
                   (sockaddr*)&serverAddr_,
                   sizeof(serverAddr_));
            if(endFlag){
                break;
            }
            std::cout << "Sent file: " << file << std::endl;
            
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }
}

private:

    int sock_;
    std::string serverIp_;
    int serverPort_;

    sockaddr_in serverAddr_{};
};


void SignalHandler(int signal)
{
    if(signal == SIGTERM || signal == SIGINT)
    {
        runFlag.store(false);
    }
}

int main()
{
    runFlag.store(true);
    std::signal(SIGTERM, SignalHandler);
    std::signal(SIGINT, SignalHandler);
    UDPSender sender("127.0.0.1", 8080);
    if (!sender.Start())
        return -1;

    if (!sender.PerformHandshake("Client1"))
    {
        std::cout << "Server busy or handshake failed\n";
        return 0;
    }

    sender.SendData();
    return 0;
}