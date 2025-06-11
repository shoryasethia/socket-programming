#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")
using namespace std;

void receiveFile(SOCKET sock) {
    char buffer[1024];
    string header = "";
    while (true) {
        int bytes = recv(sock, buffer, sizeof(buffer), 0);
        if (bytes <= 0) break;

        header += string(buffer, buffer + bytes);
        size_t pos = header.find('\n');
        if (pos != string::npos) {
            string meta = header.substr(0, pos);
            header.erase(0, pos + 1);

            if (meta.substr(0, 6) == "/file:") {
                // Parse header
                size_t name_pos = meta.find(':', 6);
                string filename = meta.substr(6, name_pos - 6);
                int filesize = stoi(meta.substr(name_pos + 1));

                ofstream out("server_received/" + filename, ios::binary); 

                int received = header.size();
                out.write(header.data(), received);

                while (received < filesize) {
                    int n = recv(sock, buffer, sizeof(buffer), 0);
                    if (n <= 0) break;
                    out.write(buffer, n);
                    received += n;
                }

                out.close();
                cout << "File '" << filename << "' received!" << endl;
            }
        }
    }
}

void sendFile(SOCKET sock) {
    string command;
    while (true) {
        cout << "Type /send <filename>: ";
        getline(cin, command);
        if (command.substr(0, 6) == "/send ") {
            string filename = command.substr(6);
            ifstream file("server_files/" + filename, ios::binary);
            if (!file) {
                cout << "Cannot open file.\n";
                continue;
            }

            file.seekg(0, ios::end);
            int filesize = file.tellg();
            file.seekg(0);

            string header = "/file:" + filename + ":" + to_string(filesize) + "\n";
            send(sock, header.c_str(), header.size(), 0);

            char buffer[1024];
            while (!file.eof()) {
                file.read(buffer, sizeof(buffer));
                send(sock, buffer, file.gcount(), 0);
            }

            file.close();
            cout << "File sent: " << filename << endl;
        }
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
    listen(serverSocket, 1);
    cout << "Waiting for client...\n";

    SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
    cout << "Client connected!\n";

    thread recvThread(receiveFile, clientSocket);
    thread sendThread(sendFile, clientSocket);

    recvThread.join();
    sendThread.join();

    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
