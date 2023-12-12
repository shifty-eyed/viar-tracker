#include "SocketConnectionWrapper.h"

bool SocketConnectionWrapper::init(TrackerNodeConfig config)
{
    cout << "Connected!\n";
    return true;
}

void SocketConnectionWrapper::close() {
}

bool SocketConnectionWrapper::sendMessage(string message) {
	return true;
}

string SocketConnectionWrapper::receiveMessage()
{
    char buffer[1024];
	return string(buffer);
}
