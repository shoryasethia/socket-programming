#include <winsock2.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

int main(){
  WSADATA wsa;
  WSAStartup(MAKEWORD(2,2), &wsa);

  SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);

  sockaddr_in serverAddr{};
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(8080);
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

  connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));

  const char* message = "Hello from Client!";
  send(clientSocket, message, strlen(message), 0);

  closesocket(clientSocket);
  WSACleanup();

}