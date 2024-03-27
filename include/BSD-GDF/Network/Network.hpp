#pragma once

#include <map>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include <fcntl.h>

#include "../Config.hpp"
#include <BSD-GDF/Logger.hpp>

namespace gdf
{

class Network
{
public:
    Network();
    ~Network();

    bool Init(const int32 IN port);
    int32 ConnectNewClient();
    void RecvFromClient(const int32 IN socket);
    void SendToClient(const int32 IN socket);
    void FetchToSendBuffer(const int32 IN socket, const std::string& IN buf);
    int32 GetServerSocket() const;
    std::string GetIPString(const int32 IN socket) const;
private:
    Network(const Network& Network); // = delete
    const Network& operator=(const Network& Network); // = delete

    bool createServerSocket();
    bool setServerSocket(const int32 IN port);

private:
    enum eBufferSize
    {
        RecvBufferSize = 1024,
        SendBufferSize = 1024,
    };
    struct Session
    {
        sockaddr_in addr;
        int32 socket;
        char hostName[NI_MAXHOST];
        char recvBuffer[RecvBufferSize];
        int64 recvSize;
        std::string sendBuffer;
        uint64 sendBufferIndex;
        bool sendBufferRemain;
    };
private:
    int32 mServerSocket;
    std::string mServerIPString;
    std::map<int32, struct Session> mSessions;
};

}
