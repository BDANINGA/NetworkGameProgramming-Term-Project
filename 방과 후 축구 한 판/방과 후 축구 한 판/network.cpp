#include "network.h"
#include "Keeper.h"

extern bool gameover;
extern PacketRenderData renderData;
extern Player player[3];
extern Ball ball;
extern Keeper keeper;

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
void ProcessPacket(SOCKET socket, const PacketHeader& header)
{
    uint16_t type = ntohs(header.type);

    switch (type)
    {
    case PKT_RENDER_DATA:
    {
        recv_renderdata(socket, header, &renderData);
        for (int i = 0; i < 3; ++i) {
            player[i].setPosition(renderData.p_data->position);
            player[i].setRotation(renderData.p_data->rotation);
        }
        ball.setPosition(renderData.b_data.position);
        ball.setRotation(renderData.b_data.rotation);
        ball.setRotationAngle(renderData.b_data.rotationAngle);
        keeper.setPosition(renderData.k_data.position);
        keeper.setRotation(renderData.k_data.rotation);
        break;
    }
    case PKT_LOGIN_RESULT:
    {
        
        break;
    }
    case PKT_GAMEOVER:                                 
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
        // =====================
        // (1) 패킷 수신
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

        // =====================
        // (2) 패킷 처리
        // =====================
        ProcessPacket(sock, header);
    }

    closesocket(sock);
    return 0;
}