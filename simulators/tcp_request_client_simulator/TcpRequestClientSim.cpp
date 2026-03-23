#include <iostream>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <chrono>
#include <cstdint>
#include "../nlohmann/json.hpp"

constexpr int PORT = 9090;
constexpr int BUFFER_SIZE = 4096;

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
        std::cout << "Enter 1 for GET_EVENTS with both timestamp and zone id\n"
            "Enter 2 for GET_EVENTS with only timestamp\n"
            "Enter 3 for GET_EVENTS with only zone id\n" 
            "Enter 4 for GET_LATEST\n" 
            "Enter -1 for exit" 
            << std::endl;
        std::cout << "Enter request (or quit): ";
        std::cin >> reqNo;
        nlohmann::json jsonReq;
        if (reqNo == -1)
            break;
        else{
            if(reqNo == 1 || reqNo == 2 || reqNo == 3){
                jsonReq["request_id"] = ("RN" + std::to_string(reqId++));
                jsonReq["request_type"] = "GET_EVENTS";
            }
            switch (reqNo){
                case 1:
                {
                    auto now = std::chrono::system_clock::now();
                    auto nowMs = std::chrono::duration_cast<std::chrono::milliseconds>(
                        now.time_since_epoch()
                    ).count();
                    jsonReq["zone_id"] = "ROOM_A";
                    jsonReq["to_time"] = nowMs;
                    jsonReq["from_time"] = nowMs - 5000;
                    if(reqId % 3 == 0){
                        jsonReq["limit"] = 2;
                    }
                }
                break;
                case 2:
                {
                    auto now = std::chrono::system_clock::now();
                    auto nowMs = std::chrono::duration_cast<std::chrono::milliseconds>(
                        now.time_since_epoch()
                    ).count();
                    jsonReq["to_time"] = nowMs;
                    jsonReq["from_time"] = nowMs - 5000;
                    if(reqId % 3 == 0){
                        jsonReq["limit"] = 2;
                    }
                }
                break;
                case 3:
                {
                    jsonReq["zone_id"] = "ROOM_A";
                }
                break;
                case 4:
                {
                    jsonReq["request_id"] = ("RN" + std::to_string(reqId++));
                    jsonReq["request_type"] = "GET_LATEST";
                }
            }
            if(reqNo == 1 || reqNo == 2 || reqNo == 3){
                std::cout<<"Enter the limit you want to give for the output, -1 if you want all the results"<<std::endl;
                std::cin >> reqNo;
                if(reqId != -1){
                    jsonReq["limit"] = reqNo;
                }
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