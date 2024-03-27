#include "../../include/BSD-GDF/Network/Network.hpp"
#include "BSD-GDF/Config/types.hpp"

namespace gdf
{

Network::Network()
: mServerSocket(ERROR)
{

}

Network::~Network()
{
    close(mServerSocket);
    mSessions.clear();
}

bool Network::Init(const int32 IN port)
{
    if (createServerSocket() == FAILURE)
    {
        return FAILURE;
    }
    if (setServerSocket(port) == FAILURE)
    {
        close(mServerSocket);
        return FAILURE;
    }
    return SUCCESS;
}

int32 Network::ConnectNewClient()
{
    // client 연결
    sockaddr_in clientAddr;
    std::memset(&clientAddr, 0, sizeof(clientAddr));
    socklen_t clientAddrLength = sizeof(clientAddr);
    int32 clientSocket = accept(mServerSocket, (sockaddr*)&clientAddr, &clientAddrLength);
    if (clientSocket == ERROR)
    {
        LOG(LogLevel::Error) << "Failed to connect client on server socket"
            << "(errno: " << errno << " - " << strerror(errno) << ") on accept()";
        return ERROR;
    }
    // client socket non-blocking 설정
    if (fcntl(clientSocket, F_SETFL, O_NONBLOCK) == ERROR)
    {
        LOG(LogLevel::Error) << "Failed to set non-blocking fd on client socket"
            << "(errno: " << errno << " - " << strerror(errno) << ") on fcntl()";
        close(clientSocket);
        return ERROR;
    }
    // client session 추가
    mSessions[clientSocket].addr = clientAddr;
    mSessions[clientSocket].socket = clientSocket;
    mSessions[clientSocket].sendBufferRemain = false;
    mSessions[clientSocket].sendBufferIndex = 0;
    mSessions[clientSocket].recvSize = 0;
    mSessions[clientSocket].sendBuffer.reserve(1024);
    return clientSocket;
}

void Network::RecvFromClient(const int32 IN socket)
{
    // client로부터 메세지 수신 시도
    struct Session& session = mSessions[socket];
    int32 recvLen = recv(socket, session.recvBuffer, sizeof(session.recvBuffer), 0);
    // 오류 발생시
    if (recvLen == ERROR)
    {
        LOG(LogLevel::Error) << "Failed to receive message from client(" << GetIPString(socket) << ")"
            << "(errno:" << errno << " - " << strerror(errno) << ") on recv()";
        close(socket);
        mSessions.erase(socket);
        return;
    }
    // 상대방과 연결이 끊긴 경우
    else if (recvLen == 0)
    {
        LOG(LogLevel::Notice) << "Client(IP: " << GetIPString(socket) << ") disconnected";
        close(socket);
        mSessions.erase(socket);
        return;
    }
    // 메시지 수신 완료
    session.recvSize = recvLen;
    session.recvBuffer[recvLen] = '\0';
    LOG(LogLevel::Notice) << "Received message from client(" << GetIPString(socket) << ") "
        << std::strlen(session.recvBuffer) << "bytes\n" << session.recvBuffer;
}

void Network::FetchToSendBuffer(const int32 IN socket, const std::string& IN buf)
{
    struct Session& session = mSessions[socket];
    session.sendBuffer.erase(0, session.sendBufferIndex);
    session.sendBufferIndex = 0;
    session.sendBuffer += buf;
    session.sendBufferRemain = true;
    
}

void Network::SendToClient(const int32 IN socket)
{
    struct Session& session = mSessions[socket];
    if (session.sendBufferRemain == false)
    {
        return ;
    }
    const char* c_sendBuffer = session.sendBuffer.c_str();
    uint64 remainLen = std::strlen(c_sendBuffer + session.sendBufferIndex);
    int sendLen = send(socket,
                       c_sendBuffer + session.sendBufferIndex,
                       remainLen,
                       0);
    // 오류 발생시
    if (sendLen == -1)
    {
        LOG(LogLevel::Error) << "Failed to send message to client(" << GetIPString(socket) << ")"
            << "(errno:" << errno << " - " << strerror(errno) << ") on send()";
        close(socket);
        mSessions.erase(socket);
        return;
    }
    // 메세지 전송 완료
    session.sendBufferIndex += static_cast<uint64>(sendLen);
    if (sendLen == remainLen)
    {
        session.sendBufferRemain = false;
        session.sendBufferIndex = 0;
        session.sendBuffer.clear();
    }
    LOG(LogLevel::Notice) << "Sent message to client(" << GetIPString(socket) << ") "
        << sendLen << "bytes";
}

int32 Network::GetServerSocket() const
{
    return mServerSocket;
}

std::string Network::GetIPString(const int32 IN socket) const
{
    if (socket == GetServerSocket())
    {
        return mServerIPString;
    }
    if (mSessions.find(socket) != mSessions.end())
    {
        return inet_ntoa(mSessions.at(socket).addr.sin_addr);
    }
    return "Unknown client(doesn't have session))";
}

bool Network::createServerSocket()
{
    mServerSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (mServerSocket == ERROR)
    {
        LOG(LogLevel::Error) << "Failed to create server socket"
            << "(errno:" << errno << " - " << strerror(errno) << ") on socket()";
        return FAILURE;
    }
    return SUCCESS;
}

bool Network::setServerSocket(const int32 IN port)
{
    int32 reuseOption = 1; // socket 사용 후, 다시 사용하기 까지의 delay 제거(개발자 테스트 편의용)
    int32 keepaliveOption = 1; // 상대방과 연결이 끊어졌는지 60초마다 확인 (TCP 연결 2시간 뒤부터 keepalive 메세지 전송 시작)
    int32 nodelayOption = 1; // 작은 size의 메세지라도, 모으지 않고 바로 보냄 (Nagle 알고리즘 비활성화)

    // server socket option 설정
    if (setsockopt(mServerSocket, SOL_SOCKET, SO_REUSEADDR, &reuseOption, sizeof(reuseOption)) == ERROR
        || setsockopt(mServerSocket, SOL_SOCKET, SO_KEEPALIVE, &keepaliveOption, sizeof(keepaliveOption)) == ERROR
        || setsockopt(mServerSocket, IPPROTO_TCP, TCP_NODELAY, &nodelayOption, sizeof(nodelayOption)) == ERROR)
    {
        LOG(LogLevel::Error) << "Failed to set socket option on server socket"
            << "(errno:" << errno << " - " << strerror(errno) << ") on setsockopt()";
        return FAILURE;
    }
    // server socket non-blocking 설정
    if (fcntl(mServerSocket, F_SETFL, O_NONBLOCK) == ERROR)
    {
        LOG(LogLevel::Error) << "Failed to set non-blocking fd on server socket"
            << "(errno:" << errno << " - " << strerror(errno) << ") on fcntl()";
        return FAILURE;
    }
    // server socket address 설정 (IP address, port 바인딩)
    sockaddr_in serverAddress;
    std::memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(port);
    mServerIPString = inet_ntoa(serverAddress.sin_addr);
    if (bind(mServerSocket, (sockaddr*)&serverAddress, sizeof(serverAddress)) == ERROR)
    {
        LOG(LogLevel::Error) << "Failed to bind server socket "
            << "(errno:" << errno << " - " << strerror(errno) << ") on bind()";
        return FAILURE;
    }
    // server socket listen (TCP 연결 준비)
    if (listen(mServerSocket, SOMAXCONN) == ERROR)
    {
        LOG(LogLevel::Error) << "Failed to listen on server socket"
            << "(errno:" << errno << " - " << strerror(errno) << ") on listen()";
        return FAILURE;
    }

    return SUCCESS;
}

}
