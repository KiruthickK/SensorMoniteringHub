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
    std::string request;
    while (true)
    {
        std::cout << "Enter request (or quit): ";
        std::getline(std::cin, request);
        nlohmann::json jsonReq;
        if (request == "quit")
            break;
        else if(request == "GET_EVENTS"){
            jsonReq["request_id"] = ("RN" + std::to_string(reqId++));
            jsonReq["zone_id"] = "ROOM_A";
            jsonReq["request_type"] = "GET_EVENTS";
            auto now = std::chrono::system_clock::now();
            auto nowMs = std::chrono::duration_cast<std::chrono::milliseconds>(
                now.time_since_epoch()
            ).count();
            jsonReq["to_time"] = nowMs;
            jsonReq["from_time"] = nowMs - 5000;
            if(reqId % 3 == 0){
                jsonReq["limit"] = 2;
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