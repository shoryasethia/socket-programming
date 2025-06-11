## 🧠 GOAL: Build a TCP-based Client-Server Greeter

> A uni-directional messaging system where:

* Server accepts connection from client
* Client can send messages

---

# ✅ PHASE 1: Detailed Understanding of Server Setup

### 🔹 Step 1: Include Necessary Headers

```cpp
#include <winsock2.h>
#include <iostream>
```

* **`<winsock2.h>`**: Contains all Windows socket APIs (like `socket()`, `bind()`, `recv()`, etc.)
* **`<iostream>`**: For input/output (e.g., `std::cout`)

#### ⚠️ Note:

We also add a special directive:

```cpp
#pragma comment(lib, "ws2_32.lib")
```

This links your code with the `ws2_32.lib` library which contains all the WinSock functions.

---

### 🔹 Step 2: Initialize WinSock

```cpp
WSADATA wsa;
WSAStartup(MAKEWORD(2,2), &wsa);
```

#### 🔍 What's going on here?

* **`WSADATA`**: A struct that receives details about the WinSock DLL.
* **`MAKEWORD(2,2)`**: Requests version 2.2 of WinSock.
* **`WSAStartup()`**: Initializes WinSock. This must be called before using any socket function.

🧠 If this fails, the program cannot use sockets at all.

---

### 🔹 Step 3: Create the Server Socket

```cpp
SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
```

#### 🔍 Explanation of parameters:

| Parameter     | Meaning                                                                     |
| ------------- | --------------------------------------------------------------------------- |
| `AF_INET`     | Address family: IPv4                                                        |
| `SOCK_STREAM` | Socket type: TCP (reliable, connection-based)                               |
| `0`           | Protocol: 0 lets system choose appropriate one for `SOCK_STREAM` (TCP here) |

📌 This function returns a `SOCKET` (an unsigned int), **not** a file descriptor like in Linux.

---

### 🔹 Step 4: Define Server Address

```cpp
sockaddr_in serverAddr{};
serverAddr.sin_family = AF_INET;
serverAddr.sin_port = htons(8080);
serverAddr.sin_addr.s_addr = INADDR_ANY;
```

#### 🔍 What each field means:

| Field                           | Meaning                                                                      |
| ------------------------------- | ---------------------------------------------------------------------------- |
| `sockaddr_in`                   | Struct used to store address info for IPv4                                   |
| `.sin_family = AF_INET`         | Set address family to IPv4                                                   |
| `.sin_port = htons(8080)`       | Port number (8080), converted to **network byte order** using `htons()`      |
| `.sin_addr.s_addr = INADDR_ANY` | Binds socket to all available interfaces on the system (localhost + LAN IPs) |

---

### 🔹 Step 5: Bind the Socket

```cpp
bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
```

#### 🔍 Purpose:

Associates the socket with the address/port you just defined.

| Parameter                | Description                      |
| ------------------------ | -------------------------------- |
| `serverSocket`           | The socket you created           |
| `(sockaddr*)&serverAddr` | Casted address structure pointer |
| `sizeof(serverAddr)`     | Size of that struct              |

📌 This lets the OS know that your socket listens on a particular port.

---

### 🔹 Step 6: Start Listening for Clients

```cpp
listen(serverSocket, 5);
```

#### 🔍 Purpose:

Puts the socket into a **passive listening mode** to accept connections.

| Parameter | Description                                                                     |
| --------- | ------------------------------------------------------------------------------- |
| `5`       | Backlog queue length – max number of pending connections waiting to be accepted |

---

### 🔹 Step 7: Accept a Client Connection

```cpp
SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
```

#### 🔍 Explanation:

| Parameter      | Purpose                                                             |
| -------------- | ------------------------------------------------------------------- |
| `serverSocket` | The socket you are listening on                                     |
| `nullptr`      | You can pass an optional pointer to a `sockaddr` to get client info |
| Returns        | A new socket for communicating with the connected client            |

🧠 Your original `serverSocket` keeps listening; this `clientSocket` handles communication.

---

### 🔹 Step 8: Receive Data from Client

```cpp
char buffer[1024] = {0};
recv(clientSocket, buffer, sizeof(buffer), 0);
```

#### 🔍 `recv()` parameters:

| Parameter        | Meaning                             |
| ---------------- | ----------------------------------- |
| `clientSocket`   | The socket from which to read       |
| `buffer`         | Where to store the received message |
| `sizeof(buffer)` | Maximum bytes to receive            |
| `0`              | Flags (set to 0 normally)           |

---

### 🔹 Step 9: Close Sockets

```cpp
closesocket(clientSocket);
closesocket(serverSocket);
WSACleanup();
```

* `closesocket()` → closes the socket
* `WSACleanup()` → deinitializes WinSock

---

Excellent! Let’s now go through the **client side** of the socket program in detail — just like we did with the server. Again, we’ll cover **each line and parameter** so you fully understand what’s happening.

---

## ✅ PHASE 2: Client Setup — Line-by-Line Explanation

This part is responsible for **initiating the connection to the server** and sending messages.

### 🔹 Step 1: Include Headers and Link WinSock

```cpp
#include <winsock2.h>
#include <iostream>
```

* **`winsock2.h`**: Provides WinSock API (needed for sockets in Windows)
* **`iostream`**: Enables standard input/output (`std::cout`, `std::cin`)

🔧 Also include:

```cpp
#pragma comment(lib, "ws2_32.lib")
```

This tells the linker to link the `ws2_32.lib` file automatically, which contains implementation for all socket functions.

---

### 🔹 Step 2: Initialize WinSock

```cpp
WSADATA wsa;
WSAStartup(MAKEWORD(2,2), &wsa);
```

#### 🔍 Parameters

* `MAKEWORD(2,2)` – Requests WinSock version 2.2.
* `&wsa` – A pointer to `WSADATA`, which receives details about the implementation.

📌 Must be called before any socket function is used.

---

### 🔹 Step 3: Create the Client Socket

```cpp
SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
```

Just like the server:

| Parameter     | Explanation                                                       |
| ------------- | ----------------------------------------------------------------- |
| `AF_INET`     | IPv4 addressing                                                   |
| `SOCK_STREAM` | TCP (stream-based)                                                |
| `0`           | Let system pick appropriate protocol for TCP (i.e., IPPROTO\_TCP) |

📌 Returns a `SOCKET` type.

---

### 🔹 Step 4: Define Server Address

```cpp
sockaddr_in serverAddr{};
serverAddr.sin_family = AF_INET;
serverAddr.sin_port = htons(8080);
serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
```

#### 🔍 Explanation:

* `sockaddr_in`: Struct for an IPv4 address.
* `.sin_family = AF_INET`: Must match the socket's address family.
* `.sin_port = htons(8080)`: Port number (in network byte order).
* `.sin_addr.s_addr = inet_addr("127.0.0.1")`: Converts string IP to binary. This is the IP address of the server.

📌 You can replace `"127.0.0.1"` with actual IP if server is remote.

---

### 🔹 Step 5: Connect to the Server

```cpp
connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
```

#### 🔍 Parameters:

| Parameter                | Explanation                |
| ------------------------ | -------------------------- |
| `clientSocket`           | Your client socket         |
| `(sockaddr*)&serverAddr` | Address of the server      |
| `sizeof(serverAddr)`     | Size of the address struct |

If this succeeds, you're now connected!

---

### 🔹 Step 6: Send a Message to the Server

```cpp
const char* message = "Hello from Client!";
send(clientSocket, message, strlen(message), 0);
```

#### 🔍 Parameters:

| Parameter         | Meaning                    |
| ----------------- | -------------------------- |
| `clientSocket`    | Socket connected to server |
| `message`         | Data to be sent            |
| `strlen(message)` | Size of message            |
| `0`               | Flags (usually 0)          |

📌 `send()` is a system call that transmits data over the socket to the remote endpoint.

---

### 🔹 Step 7: Close the Socket and Cleanup

```cpp
closesocket(clientSocket);
WSACleanup();
```

* `closesocket()` — closes the open socket connection
* `WSACleanup()` — frees the resources allocated by `WSAStartup`

---

### ⚙️ Compile and Run Instructions
To compile and run the server and client on Windows, follow these commands in your terminal or command prompt:


| Task           | Command                                     |
| -------------- | ------------------------------------------- |
| Check compiler | `g++ --version`                             |
| Compile Server | `g++ server.cpp -o server.exe -lws2_32`     |
| Compile Client | `g++ client.cpp -o client.exe -lws2_32`     |
| Run Server     | `./server.exe`                              |
| Run Client     | `./client.exe`                              |

> Make sure to run the server first before starting the client.