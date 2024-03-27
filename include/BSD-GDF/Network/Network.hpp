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
    bool RecvFromClient(const int32 IN socket);
    bool SendToClient(const int32 IN socket);
    void FetchToSendBuffer(const int32 IN socket, const std::string& IN buf);
    bool PullFromRecvBuffer(const int32 IN socket, std::string& OUT buf, const std::string& endString = "\0");
    int32 GetServerSocket() const;
    const std::string GetIPString(const int32 IN socket) const;
private:
    Network(const Network& Network); // = delete
    const Network& operator=(const Network& Network); // = delete

    bool createServerSocket();
    bool setServerSocket(const int32 IN port);

private:
    enum { RECV_BUFFER_SIZE = 1024 };
    struct Session
    {
        sockaddr_in addr;
        int32 socket;
        std::string recvBuffer;
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
