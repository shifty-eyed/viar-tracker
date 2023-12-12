#pragma once

#include "TrackerNodeConfig.h"

#include <iostream>
#ifdef __WINDOWS__
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#endif


#define SOCKET_ERROR_STRING "__SOCKET_ERROR__"

class SocketConnectionWrapper {
public:
	bool init(TrackerNodeConfig config);
	void close();
	bool sendMessage(string message);
	string receiveMessage();
	string getLastError() { return lastError;; };

private:
#ifdef __WINDOWS__
	WSADATA wsaData;
	SOCKET clientSocket;
	sockaddr_in serverAddress;
#endif
	string lastError;

};

