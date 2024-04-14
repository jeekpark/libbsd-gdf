/**
 * @file Network.hpp
 * @author Taeil-Nam (nam0314@gmail.com)
 * @brief Network 클래스 정의 헤더 파일.
 * @version 0.1
 * @date 2024-04-14
 * 
 * @copyright Copyright (c) 2024
 * 
 */

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
/**
 * @class Network
 * @brief 시스템의 네트워크 로직 처리를 위한 Network 객체를 정의하는 클래스.
 */
class Network
{
private:
    /**
     * @brief 커널의 receive 버퍼에서 한번에 가져올 수 있는 데이터 크기를 나타내는 상수.
     */
    enum { kRecvBufferSize = 1024 };
    /**
     * @brief 네트워크 연결이 완료된 세션의 정보를 저장하는 구조체.
     */
    struct Session
    {
        /**
         * @brief 세션의 네트워크 정보를 저장해놓은 구조체.
         */
        sockaddr_in addr;
        /**
         * @brief 세션의 소켓.
         */
        int32 socket;
        /**
         * @brief 세션이 사용하는 receive buffer.
         */
        std::string recvBuffer;
        /**
         * @brief 세션이 사용하는 send buffer.
         */
        std::string sendBuffer;
        /**
         * @brief 다음에 보내야되는 send buffer의 데이터 위치를 가리키는 인덱스. 
         */
        uint64 sendBufferIndex;
        /**
         * @brief send buffer에 보내야하는 데이터가 남아있는지를 나타내는 변수. 
         */
        bool sendBufferRemain;
        /**
         * @brief 세션이 연결 종료가 예정되어 있는 상태인지 나타내는 변수.
         *
         * 이 변수 값이 true인 경우, 세션은 send buffer에 남아있는 데이터를 다 보낸 뒤 연결을 종료한다.
         */
        bool isReservedDisconnect;
    };

public:
    /**
     * @brief Network 객체의 기본 생성자.
     *
     * mServerSocket 멤버 변수를 -1로 초기화한다.
     */
    Network();
    /**
     * @brief Network 객체의 소멸자.
     *
     * mServerSocket 멤버 변수를 close 한다.\n
     * mSessions를 clear 한다.
     */
    ~Network();

    /**
     * @brief 서버의 소켓을 생성하고 설정하는 함수.
     * 
     * private 멤버 함수 createServerSocket(), setServerSocket(port)를 호출한다.
     *
     * @param port 소켓이 사용할 port number.
     * @return true : 소켓 생성 및 설정 성공.
     * @return false : 소켓 생성 및 설정 실패.
     */
    bool Init(const int32 IN port);
    /**
     * @brief 클라이언트의 TCP 연결 요청을 수락하는 함수.
     *
     * 연결이 성공적으로 완료되면, 클라이언트 소켓을 non-blocking으로 설정하고 mSessions에 클라이언트 세션을 추가한 뒤 
     * 연결된 클라이언트 소켓을 반환한다.
     * 
     * @return int32 : 연결된 클라이언트의 소켓. (연결 실패시 -1 반환)
     */
    int32 ConnectNewClient();
    /**
     * @brief 클라이언트와 연결을 종료하는 함수.
     *
     * 클라이언트의 소켓을 close하고 세션을 삭제한다.
     * 
     * @param socket 연결을 종료할 클라이언트의 소켓.
     */
    void DisconnectClient(const int32 IN socket);
    /**
     * @brief 클라이언트로부터 전송된 데이터를 가져오는 함수.
     * 
     * 한번에 가져올 수 있는 데이터의 최대 크기는 kRecvBufferSize 이다.\n
     * recv() 함수를 통해 데이터를 가져오며, 가져온 데이터는 클라이언트 세션의 recvBuffer에 추가된다.
     *
     * @param socket 클라이언트의 소켓.
     * @return true : 데이터 수신 성공.
     * @return false : 클라이언트와 연결이 끊기거나, 오류 발생.
     */
    bool RecvFromClient(const int32 IN socket);
    /**
     * @brief 클라이언트에게 데이터를 전송하는 함수.
     * 
     * 클라이언트 세션의 sendBuffer에 데이터가 없는 경우 무시된다.\n
     * 클라이언트 세션의 sendBuffer에 데이터가 없고, 연결 종료 예약이 되어있는 경우 클라이언트와 연결을 종료한다.\n
     * send() 함수를 통해 데이터를 전송한다.\n
     * 데이터 전송 후 sendBuffer에 보내야 하는 데이터가 남아있는 경우, sendBufferIndex가 해당 위치를 가리키고 있게된다.\n
     * 데이터 전송 후 sendBuffer에 데이터가 남아있지 않은 경우, sendBufferIndex를 0으로 초기화한다.
     *
     * @param socket 클라이언트의 소켓.
     * @return true : 데이터 전송 완료.
     * @return false : 오류 발생.
     */
    bool SendToClient(const int32 IN socket);
    /**
     * @brief 클라이언트 세션의 sendBuffer에 데이터를 추가하는 함수.
     *
     * 클라이언트 세션의 sendBuffer에 데이터를 추가하고, sendBufferRemain = true로 설정한다.
     * 
     * @param socket 클라이언트의 소켓.
     * @param buf 추가할 데이터.
     */
    void PushToSendBuffer(const int32 IN socket, const std::string& IN buf);
    /**
     * @brief 클라이언트 세션의 recvBuffer에서 데이터를 가져오는 함수.
     *
     * endString 매개 변수를 통해 특정 구분자까지만 데이터를 가져올 수 있으며, 
     * endString 없이 호출하는 경우 모든 데이터를 가져온다.
     * 
     * @param socket 클라이언트의 소켓.
     * @param buf 가져온 데이터를 저장할 buffer.
     * @param endString 구분자.
     * @return true : 데이터 가져오기 성공.
     * @return false : 가져올 데이터가 없음.
     */
    bool PullFromRecvBuffer(const int32 IN socket, std::string& OUT buf, const std::string& endString = "\0");
    /**
     * @brief 클라이언트 세션의 연결 종료를 예약하는 함수.
     *
     * 보내야하는 데이터를 전부 보내고 연결을 끊어야 하는 경우 사용된다.\n
     * 이 함수가 호출되면, 해당 클라이언트 세션의 sendBuffer 데이터를 전부 보낸 뒤 연결이 종료된다.
     * 
     * @param socket 클라이언트의 소켓.
     */
    void ReserveDisconnectClient(const int32 IN socket);
    /**
     * @brief 특정 클라이언트 세션의 recvBuffer를 초기화하는 함수.
     * 
     * @param socket 클라이언트의 소켓.
     */
    void ClearRecvBuffer(const int32 IN socket);
    /**
     * @brief 특정 클라이언트 세션의 sendBuffer를 초기화하는 함수.
     * 
     * @param socket 클라이언트의 소켓.
     */
    void ClearSendBuffer(const int32 IN socket);
    /**
     * @brief 서버 소켓을 반환하는 함수.
     * 
     * @return int32 
     */
    int32 GetServerSocket() const;
    /**
     * @brief 특정 소켓의 IP를 문자열로 반환하는 함수.
     * 
     * @param socket 대상 소켓.
     * @return const std::string : 대상 소켓의 IP 주소. 
     */
    const std::string GetIPString(const int32 IN socket) const;
    /**
     * @brief 특정 클라이언트 세션을 반환하는 함수.
     * 
     * @param socket 클라이언트 소켓.
     * @return const struct Session& : 클라이언트의 세션.
     */
    const struct Session& GetSession(const int32 IN socket) const;
private:
    /**
     * @brief Network 객체의 복사 생성자. (사용되지 않음)
     *
     * @param Network 복사할 Network 객체.
     */
    Network(const Network& Network); // = delete
    /**
     * @brief Network 객체의 복사 대입 연산자. (사용되지 않음)
     * 
     * @param Network 복사할 Network 객체.
     * @return const Network& : 복사된 Network 객체.
     */
    const Network& operator=(const Network& Network); // = delete

    /**
     * @brief 서버에서 사용될 IPv4/TCP 기반의 소켓을 생성한다.
     * 
     * @return true : 소켓 생성 성공.
     * @return false : 소켓 생성 실패.
     */
    bool createServerSocket();
    /**
     * @brief 서버 소켓을 설정하고, 클라이언트의 연결 요청을 대기한다.
     *
     * 설정을 마치고, mServerIPString 멤버 변수에 서버 소켓의 IP를 문자열 형태로 저장한다.
     *
     * [설정 목록]
     * - non-blocking : socket을 non-blocking으로 설정.
     * - IP, port number : socket에 IP 주소와 port number 설정.
     * - reuseOption : socket 사용 후, 다시 사용하기 까지의 delay 제거.
     * - keepaliveOption : 상대방과 연결이 끊어졌는지 60초마다 확인. (TCP 연결 2시간 뒤부터 keepalive 메세지 전송 시작)
     * - nodelayOption : 작은 size의 메세지라도, 모아놓지 않고 바로 보내도록 설정. (Nagle 알고리즘 비활성화)
     * - listen : 클라이언트의 연결 요청을 받을 수 있는 상태로 설정.
     * 
     * @param port 소켓에 설정할 port number.
     * @return true : 소켓 설정 성공.
     * @return false : 소켓 설정 실패.
     */
    bool setServerSocket(const int32 IN port);

private:
    /**
     * @brief 서버 소켓의 fd를 저장하는 멤버 변수.
     */
    int32 mServerSocket;
    /**
     * @brief 서버 소켓의 IP를 문자열 형태로 저장하는 멤버 변수.
     */
    std::string mServerIPString;
    /**
     * @brief 서버와 연결된 세션의 목록을 저장하는 멤버 변수.
     *
     * key = 세션의 소켓, value = Session 구조체.
     */
    std::map<int32, struct Session> mSessions;
};

}
