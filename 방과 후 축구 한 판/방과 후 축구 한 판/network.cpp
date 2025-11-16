#include "network.h"

extern bool gameover;

// --- connect함수 ---
bool ConnectToServer(SOCKET& g_ServerSocket, const char* ipAddress, uint16_t port)
{
    g_ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (g_ServerSocket == INVALID_SOCKET)
    {
        std::cerr << "Socket creation failed" << std::endl;
        WSACleanup();
        return false;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress, &serverAddr.sin_addr);
    if (connect(g_ServerSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        std::cerr << "Connection to server failed" << std::endl;
        closesocket(g_ServerSocket);
        WSACleanup();
        return false;
    }
    return true;
}

// --- 수신된 패킷을 처리하는 함수 ---
void ProcessPacket(SOCKET socket, const PacketHeader& header, char* payload)
{
    uint16_t type = ntohs(header.type);

    switch (type)
    {
    case PKT_RENDER_DATA:
    {
        /* memcpy(&g_LatestRenderData, &header, sizeof(PacketHeader));
         memcpy(((char*)&g_LatestRenderData) + sizeof(PacketHeader),
             payload, sizeof(PacketRenderData) - sizeof(PacketHeader));*/
        break;
    }
    case PKT_LOGIN_RESULT:
    {
        /*memcpy(&g_LatestLoginResult, &header, sizeof(PacketHeader));
        memcpy(((char*)&g_LatestLoginResult) + sizeof(PacketHeader),
            payload, sizeof(PacketLoginResult) - sizeof(PacketHeader));*/
        break;
    }
    case PKT_GAMEOVER:                                  // 이런 식으로 처리?
    {
        recv_gameover(socket, header, &gameover);
        break;
    }
    default:
        std::cout << "Unknown packet type: " << type << std::endl;
        break;
    }
}

// --- 클라이언트 네트워크 스레드 ---
DWORD WINAPI ClientNetworkThread(LPVOID lpParam)
{
    SOCKET sock = (SOCKET)lpParam;

    while (true)
    {
        //// =====================
        //// (1) 클라이언트 입력 전송
        //// =====================
        //PacketInputkey keyPkt;
        //PacketInputspecialkey specialPkt;

        //memcpy(keyPkt.key, g_currentKey, sizeof(g_currentKey));
        //memcpy(specialPkt.specialkey, g_currentSpecialKey, sizeof(g_currentSpecialKey));

        //send(sock, (char*)&keyPkt, sizeof(keyPkt), 0);
        //send(sock, (char*)&specialPkt, sizeof(specialPkt), 0);

        // =====================
        // (2) 패킷 수신
        // =====================
        PacketHeader header;
        int bytesReceived = 0;
        int totalReceived = 0;

        // --- 헤더(4바이트) 수신 ---
        while (totalReceived < sizeof(PacketHeader))
        {
            bytesReceived = recv(sock, ((char*)&header) + totalReceived,
                sizeof(PacketHeader) - totalReceived, 0);
            if (bytesReceived <= 0)
            {
                std::cerr << "서버 연결 종료" << std::endl;
                closesocket(sock);
                return 0;
            }
            totalReceived += bytesReceived;
        }

        uint16_t packetSize = ntohs(header.size);
        int payloadSize = packetSize - sizeof(PacketHeader);

        // --- 페이로드 수신 ---
        char payloadBuffer[2048];
        totalReceived = 0;
        while (totalReceived < payloadSize)
        {
            bytesReceived = recv(sock, payloadBuffer + totalReceived,
                payloadSize - totalReceived, 0);
            if (bytesReceived <= 0)
            {
                std::cerr << "서버 연결 종료" << std::endl;
                closesocket(sock);
                return 0;
            }
            totalReceived += bytesReceived;
        }

        // =====================
        // (3) 패킷 처리
        // =====================
        ProcessPacket(sock, header, payloadBuffer);
    }

    closesocket(sock);
    return 0;
}