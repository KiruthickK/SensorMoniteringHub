#include <iostream>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fstream>
#include <chrono>
#include <cstdint>
#include "../nlohmann/json.hpp"

constexpr int PORT = 8090;
constexpr int BUFFER_SIZE = 4096;

nlohmann::json GetConfigFile(){
    std::string configPath{"./config/SMH_Config.json"};
    std::ifstream file(configPath);
    if (!file.is_open()) {
        std::cerr<<"Config file is not available!"<<std::endl;
        return nullptr;
    }
    nlohmann::json config;
    file >> config;
    return config;
}
int main()
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    int reqId{0};
    if (sock < 0)
    {
        std::cout << "Socket creation failed\n";
        return 1;
    }
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
    if (connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
    {
        std::cout << "Connection failed\n";
        return 1;
    }
    std::cout << "Connected to server\n";
    int reqNo;
    std::string request;
    while (true)
    {
        std::cout << "Enter 1 for clear event\n"
            "Enter 2 for config Change request [which will change trigger the restart of the application as well]\n" 
            "Enter 3 for requesting shutdown\n"
            "Enter 100 for Invalid request\n" 
            "Enter -1 for exit" 
            << std::endl;
        std::cout << "Enter request (or quit): ";
        std::cin >> reqNo;
        nlohmann::json jsonReq;
        jsonReq["order_id"] = ("ORN" + std::to_string(reqId++));
        if (reqNo == -1)
            break;
        else{
            if(reqNo == 1){
                jsonReq["order_type"] = "CLEAR_EVENTS";
            }else if(reqNo == 2){
                jsonReq["order_type"] = "CONFIG_CHANGE";
                jsonReq["new_config"] = GetConfigFile();
            }else if(reqNo == 3){
                jsonReq["order_type"] = "SHUTDOWN_REQUEST";
            }else{
                jsonReq["order_hype"] = "CLEAR_HORN"; //invalid request
            }
            request = jsonReq.dump();
        }
        request += "\n";
        send(sock, request.c_str(), request.size(), 0);
        char buffer[BUFFER_SIZE];
        int bytes = recv(sock, buffer, sizeof(buffer), 0);
        if (bytes <= 0)
        {
            std::cout << "Server closed connection\n";
            break;
        }
        std::string response(buffer, bytes);
        std::cout << "Response: " << response << std::endl;
    }
    close(sock);
    return 0;
}