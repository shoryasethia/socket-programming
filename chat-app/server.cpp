#include <winsock2.h>
#include <iostream>
#include <thread>
#include <string>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

void receiveMessages(SOCKET clientSocket) {
    char buffer[1024];
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0) {
            cout << "\nClient disconnected.\n";
            break;
        }
        cout << "\nClient: " << buffer << endl;
    }
}

void sendMessages(SOCKET clientSocket) {
    string message;
    while (true) {
        getline(cin, message);
        send(clientSocket, message.c_str(), message.size(), 0);
        if (message == "exit") break;
    }
}

int main() {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, 5);

    cout << "Waiting for connection...\n";
    SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
    cout << "Client connected!\n";

    thread recvThread(receiveMessages, clientSocket);
    thread sendThread(sendMessages, clientSocket);

    sendThread.join();
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}
