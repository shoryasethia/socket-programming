#include <winsock2.h>
#include <iostream>
#include <thread>
#include <string>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

void receiveMessages(SOCKET serverSocket) {
    char buffer[1024];
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytesReceived = recv(serverSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0) {
            cout << "\nServer disconnected.\n";
            break;
        }
        cout << "\nServer: " << buffer << endl;
    }
}

void sendMessages(SOCKET serverSocket) {
    string message;
    while (true) {
        getline(cin, message);
        send(serverSocket, message.c_str(), message.size(), 0);
        if (message == "exit") break;
    }
}

int main() {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    cout << "Connected to server!\n";

    thread recvThread(receiveMessages, clientSocket);
    thread sendThread(sendMessages, clientSocket);

    sendThread.join();
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}