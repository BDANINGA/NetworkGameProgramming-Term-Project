#include "network.h"
#include "Keeper.h"

extern bool gameover;
extern bool loginResult;
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

// --- 클라이언트 네트워크 스레드 ---
DWORD WINAPI ClientNetworkThread(LPVOID lpParam)
{
    SOCKET sock = (SOCKET)lpParam;
    PacketHeader header;
    while (true)
    {
        // 헤더 수신
        if (!RecvTCP(sock, (char*)&header, sizeof(PacketHeader))) {
            break;
        }
        // 패킷 정보 변환 
        header.size = ntohs(header.size);
        header.type = ntohs(header.type);
        // =====================
        // (2) 패킷 처리
        // =====================
        switch (header.type)
        {
        case PKT_RENDER_DATA:
        {
            if (!RecvTCP(sock, (char*)&renderData + sizeof(PacketHeader), header.size)) {
                std::cout << "error_recv: renderdata" << std::endl;
                break;
            }
            for (int i = 0; i < MAX_PLAYERS; ++i) {
                player[i].setPosition(renderData.p_data->position);
                player[i].setRotation(renderData.p_data->rotation);
            }
            ball.setPosition(renderData.b_data.position);
            ball.setRotation(renderData.b_data.rotation);
            ball.setRotationAngle(renderData.b_data.rotationAngle);
            keeper.setPosition(renderData.k_data.position);
            keeper.setRotation(renderData.k_data.rotation);

            for (int i = 0; i < MAX_PLAYERS; ++i) {
                g_CurrentScores[i] = renderData.playerScore[i];
            }
            g_SecondsRemaining = renderData.remainingTime;

            break;
        }
        case PKT_LOGIN_RESULT:
        {
            if (!RecvTCP(sock, (char*)&loginResult + sizeof(PacketHeader), header.size)) {
                std::cout << "error_recv: loginresult" << std::endl;
                break;
			}
            break;
        }
        case PKT_GAMEOVER:
        {
            if (!RecvTCP(sock, (char*)&gameover + sizeof(PacketHeader), header.size)) {
                std::cout << "error_recv: gameover" << std::endl;
                break;
            }
            break;
        }
        default:
            std::cout << "Unknown packet type: " << header.type << std::endl;
            break;
        }
    }

    closesocket(sock);
    return 0;
}

bool RecvTCP(SOCKET sock, char* buffer, int size) {
    int bytesRead = 0;
    while (bytesRead < size) {
        int result = recv(sock, buffer + bytesRead, size - bytesRead, 0);
        if (result == SOCKET_ERROR || result == 0) {
            std::cerr << "Client disconnected (ID: " << (int)sock << ")" << std::endl;
            return false;
        }
        bytesRead += result;
    }
    return true;
}