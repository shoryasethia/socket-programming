# Socket Programming in C++

Socket programming in C++ using the **Winsock2 API** on Windows. It includes:
* 📬 A **bidirectional chat app** where server and client can both send and receive messages.
* 📂 A **simple file transfer protocol** where server and client can send files to each other.

## 🧠 C++ Socket Functions Summary

| Component       | Description                                             |
| --------------- | ------------------------------------------------------- |
| `socket()`      | Creates a socket descriptor.                            |
| `bind()`        | Binds socket to an address and port.                    |
| `listen()`      | Prepares the socket to accept incoming connections.     |
| `accept()`      | Accepts a connection from a client.                     |
| `recv()/send()` | Receives/Sends data over the connection.                |
| `close()`       | Closes the socket (POSIX) or `closesocket()` (WinSock). |

## 🔄 Differences Between POSIX and Windows

| Feature        | POSIX (Linux)                      | Windows (WinSock)              |
| -------------- | ---------------------------------- | ------------------------------ |
| Header files   | `<sys/socket.h>`, `<netinet/in.h>` | `<winsock2.h>`, `<ws2tcpip.h>` |
| Socket close   | `close()`                          | `closesocket()`                |
| Initialization | No init needed                     | `WSAStartup()`                 |

---

## ✅ Requirements

* Windows OS
* MinGW-w64 with GCC >= 11 (`g++ --version` to check)
* `WSAStartup()` and `closesocket()` for WinSock API
* Compile using `-lws2_32`

---

## 📁 Directory Structure

```
socket-programming/
│
├── chat-app/
│   ├── server.cpp           # Chat server source
│   ├── client.cpp           # Chat client source
│
├── file-transfer/
│   ├── server.cpp           # File transfer server
│   ├── client.cpp           # File transfer client
│   ├── client_files/        # Directory holding files to send (client side)
│   ├── server_files/        # Directory holding files to send (server side)
│   ├── client_received/     # Directory where files are stored (client side)
│   ├── server_received/     # Directory where files are stored (server side)
```

---

## 💬 Bidirectional Chat App

### ✅ Features

* Both server and client can send/receive simultaneously
* Implemented using `std::thread`

Each side (server and client) launches **two threads**:
1. **`receiveMessages(socket)`** – Continuously reads messages from the socket and prints to console.
2. **`sendMessages(socket)`** – Continuously reads user input from terminal and sends via socket.

These run concurrently so that a client can keep listening and sending at the same time.

#### Server Command
Terminal 1
```bash
# Compile
$ g++ -std=c++11 server.cpp -o server.exe -lws2_32

# Run
$ ./server.exe
```

#### Client Command
Terminal 2
```bash
# Compile
$ g++ -std=c++11 client.cpp -o client.exe -lws2_32

# Run
$ ./client.exe
```

---

## 📂 Simple Bidirectional File Transfer Protocol

### ✅ Features

* Send any binary/text file (e.g., `.jpg`, `.txt`, `.pdf`, etc.)
* Transfers files using raw buffer chunks
* Bidirectional: Client <-> Server

### 🧠 How It Works (Step-by-step)

Each side (server/client) performs these steps:

1. **Open File**:
   * File is opened in `binary` read mode on sender side.
   * File is opened in `binary` write mode on receiver side.

2. **Send Metadata**:
   * First, the **file name** (e.g., `image.jpg`) is sent.
   * Then, the **file size** (as a `long long`) is sent to pre-calculate read loops.

3. **Transfer Data**:
   * File data is read in fixed buffer chunks (e.g., 1024 bytes) and sent via `send()`.
   * The receiver writes each chunk to the file until the complete file is received.

4. **Directory**:
   * Sent files are picked from `client_files/`
   * Received files are stored in `server_received/` (or vice versa for reverse transfer)

### 🗂️ Directory Setup
```bash
mkdir client_files client_received server_files server_received
```

### Server Command
Terminal 1
```bash
$ g++ server.cpp -o server.exe -lws2_32
$ ./server.exe
```

### Client Command
Terminal 2
```bash
$ g++ client.cpp -o client.exe -lws2_32
$ ./client.exe
```

---