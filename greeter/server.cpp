#include <winsock2.h>
#include <iostream>
using namespace std;

#pragma comment(lib, "ws2_32.lib")

int main(){
  WSADATA wsa;
  WSAStartup(MAKEWORD(2,2), &wsa);

  SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);

  sockaddr_in serverAddr{};
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(8080);
  serverAddr.sin_addr.s_addr = INADDR_ANY;

  bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
  listen(serverSocket, 5);
  cout<<"Waiting for connection."<<endl;

  SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);

  char buffer[1024] = {0};
  recv(clientSocket, buffer, sizeof(buffer), 0);
  cout<<"Client: "<<buffer<<endl;

  closesocket(clientSocket);
  closesocket(serverSocket);
  WSACleanup();
}

