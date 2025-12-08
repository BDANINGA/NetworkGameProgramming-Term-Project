#include "network.h"
#include "Keeper.h"
#include "방과후 축구한판_Client.h"

extern bool gameover;
extern PacketRenderData renderData;
extern Player player[MAX_PLAYERS];
extern Ball ball;
extern Keeper keeper;

int g_MyPlayerID = 0;

PacketLogin MyLogin;
bool loginResult;

extern GameState g_GameState; // 초기 상태는 로그인


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

    BOOL optval = TRUE;
    if (setsockopt(g_ServerSocket, IPPROTO_TCP, TCP_NODELAY, (const char*)&optval, sizeof(optval)) == SOCKET_ERROR) {
        std::cerr << "setsockopt TCP_NODELAY failed" << std::endl;
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

    Sleep(3000);


    //--- 로그인 처리 ---
    bool loginSuccess = false;
    do
    {
        PacketLoginResult loginResult;

        recv(sock, (char*)&loginResult, sizeof(PacketLoginResult), 0);
        std::cout << loginResult.message << std::endl;
        if (loginResult.success) {
            loginSuccess = true;
        }
        else {
            WCHAR msgText[32];
			MultiByteToWideChar(CP_ACP, 0, loginResult.message, strlen(loginResult.message), msgText, 32);
            MessageBox(NULL, msgText, L"Login Info", MB_OK);
        }

        g_MyPlayerID = loginResult.myPlayerID;

    } while (!loginSuccess);

    PacketGameReady readyData;
    PlayerReady(readyData, sock, true);

    g_GameState = STATE_GAME;


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
                player[i].setPosition(renderData.p_data[i].position);
                player[i].setRotation(renderData.p_data[i].rotation);
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

            for (int i = 0; i < 9; i++)
                strcpy_s(BroadCastChatMessage[i], renderData.BroadCastChatMessage[i]);

            break;
        }
        case PKT_LOGIN_RESULT:
        {
            break;
        }
        case PKT_GAMEOVER:
        {
            PacketGameover gameoverPkt;
            if (!RecvTCP(sock, (char*)&gameoverPkt + sizeof(PacketHeader), header.size)) {
                std::cout << "error_recv: gameover" << std::endl;
                break;
            }
            gameover = gameoverPkt.gameover;
            std::cout << gameover << std::endl;
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