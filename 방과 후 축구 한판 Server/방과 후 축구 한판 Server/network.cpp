#include "network.h"

// --- listen함수 ---
bool ListenForClients(ClientContext& context, uint16_t port) {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cerr << "WSAStartup failed" << std::endl;
        return false;
    }

    context.socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (context.socket == INVALID_SOCKET)
    {
        std::cerr << "Socket creation failed" << std::endl;
        WSACleanup();
        return false;
    }

    DWORD optval = 1;
    int retval = setsockopt(context.socket, SOL_SOCKET, SO_REUSEADDR, (const char*)&optval, sizeof(optval));
    if (retval == SOCKET_ERROR) std::cerr << "Socket Option Error" << std::endl;

    // bind()
    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(port);
    if (bind(context.socket, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) != 0)
    {
        std::cerr << "Bind failed" << std::endl;
        closesocket(context.socket);
        WSACleanup();
        return false;
    }

    // listen()
    if (listen(context.socket, SOMAXCONN) == SOCKET_ERROR)
    {
        std::cerr << "Listen failed" << std::endl;
        closesocket(context.socket);
        WSACleanup();
        return false;
    }

    return true;
}

// --- accept 함수 ---
SOCKET AcceptClient(SOCKET listenSocket) {
    SOCKET clientSocket = accept(listenSocket, NULL, NULL);
    if (clientSocket == INVALID_SOCKET)
    {
        std::cerr << "Accept failed" << std::endl;
        return INVALID_SOCKET;
    }

    int flag = 1;
    int result = setsockopt(clientSocket, IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(flag));

    if (result == SOCKET_ERROR) {
        printf("setsockopt failed: %d\n", WSAGetLastError());
    }

    return clientSocket;
}

// -- 쓰레드 생성 --
HANDLE CreateServerReceiveThread(ClientContext* context) {
    return CreateThread(NULL, 0, ServerReceiveThread, (LPVOID)context, 0, NULL);
}

// --- 서버 측 수신 스레드 ---
DWORD WINAPI ServerReceiveThread(LPVOID lpParam) {
    ClientContext* context = (ClientContext*)lpParam;
    SOCKET sock = context->socket;
    int playerID = context->playerID;

    PacketHeader header;
    while (true) {
        // 헤더 수신
        if (!RecvTCP(sock, (char*)&header, sizeof(PacketHeader))) {
            break;
        }
        // 패킷 정보 변환 
        header.size = ntohs(header.size);
        header.type = ntohs(header.type);

        // 패킷 타입에 따라 분기
        switch (header.type) {
            // 입력값 처리
        case PKT_INPUT_KEY: {
            if (!RecvTCP(sock, (char*)&g_LatestInputKey[playerID] + sizeof(PacketHeader), header.size)) {
                std::cout << "error_recv: key" << std::endl;
                break;
            }
            break;
        }
        case PKT_INPUT_SPECIALKEY: {
            if (!RecvTCP(sock, (char*)&g_LatestInputSpecialKey[playerID] + sizeof(PacketHeader), header.size)) {
                std::cout << "error_recv: skey" << std::endl;
                break;
            }
            break;
        }     
        case PKT_LOGIN: {
            if (header.size != sizeof(PacketLogin) - sizeof(PacketHeader)) {
                std::cerr << "Error: Invalid packet size for PKT_LOGIN. Expected "
                          << sizeof(PacketLogin) - sizeof(PacketHeader) << ", got " << header.size << std::endl;
                break;
            }

            PacketLogin loginPkt;
            // 헤더 이후 페이로드 수신
            if (!RecvTCP(sock, (char*)(&loginPkt) + sizeof(PacketHeader), header.size)) {
                std::cout << "error_recv: login" << std::endl;
                break;
            }

            // 수신된 ID/PW를 std::string으로 변환 (널 종료를 대비)
            std::string recvID(loginPkt.userID, strnlen(loginPkt.userID, sizeof(loginPkt.userID)));
            std::string recvPW(loginPkt.userPW, strnlen(loginPkt.userPW, sizeof(loginPkt.userPW)));

            bool found = false;

            if(loginPkt.isRegister) {
                // 회원가입 처리
                std::ofstream file("IDPW.txt", std::ios::app);
                if (file.is_open()) {
                    file << std::endl << recvID << " " << recvPW;
                    file.close();
                    std::cout << "Player " << playerID << " Registered with ID: " << recvID << std::endl;
                } else {
                    std::cerr << "Could not open IDPW.txt for registration." << std::endl;
                }

                std::ofstream file2("UserData.txt", std::ios::app);
                if (file2.is_open()) {
                    file2 << std::endl << recvID << " 0" << " 0";
                    file2.close();
                }
                else {
                    std::cerr << "Could not open UserData.txt for registration." << std::endl;
                }

                break;
			}

            std::ifstream file("IDPW.txt");
            if (file.is_open()) {
                std::string line;
                while (std::getline(file, line)) {
                    if (line.empty()) continue;
                    // 공백으로 구분된 id pw 예상
                    auto pos = line.find(' ');
                    if (pos == std::string::npos) continue;

                    std::string file_id = line.substr(0, pos);
                    std::string file_pw = line.substr(pos + 1);

                    // 윈도우 CR 제거 가능성 처리
                    if (!file_pw.empty() && file_pw.back() == '\r') file_pw.pop_back();

                    if (file_id == recvID && file_pw == recvPW) {
                        found = true;
                        break;
                    }
                }
                file.close();
            } else {
                std::cerr << "Could not open IDPW.txt for login verification." << std::endl;
            }

            // 응답 패킷 생성 및 전송 (성공이면 success=1, 실패면 0)
            PacketLoginResult resPkt;
            resPkt.success = found ? 1 : 0;
            resPkt.myPlayerID = playerID;
            if (found) {
                strcpy_s(resPkt.message, "Login Success");
                std::cout << "Player " << playerID << " Logged in." << std::endl;
                LoginID[playerID] = recvID;
            } else {
                strcpy_s(resPkt.message, "Login Failed");
                std::cout << "Player " << playerID << " login failed." << std::endl;
            }

            // 전체 구조체를 그대로 전송 (header는 이미 네트워크 바이트 오더로 초기화되어 있음)
            int sent = send(sock, (char*)&resPkt, sizeof(PacketLoginResult), 0);
            if (sent == SOCKET_ERROR)
                std::cerr << "error: send_LoginResult" << std::endl;

            break;
        }

            // 3명의 플레이어가 들어왔는지 확인
        case PKT_GAME_READY: {
            if (!RecvTCP(sock, (char*)&g_GameReady[playerID] + sizeof(PacketHeader), header.size)) {
                std::cout << "error_recv: gameready" << std::endl;
                break;
            }
            break;
        }
        case PKT_CHAT_MESSAGE:
            if (!RecvTCP(sock, (char*)&g_ChatMessage[playerID] + sizeof(PacketHeader), header.size)) {
                std::cout << "error_recv: chatmessage" << std::endl;
                break;
            }
            for (int i = 8; i > 0; i--) {
                strcpy_s(BroadCastChatMessage[i], BroadCastChatMessage[i - 1]);
            }
            strcpy_s(BroadCastChatMessage[0], g_ChatMessage[playerID].message);
            
            break;

        default: {
            // 패킷 헤더 식별 실패
            std::cout << "Unknown packet type: " << ntohs(header.type) << std::endl;
            break;
        }

        }
    }

    // (접속 종료 처리...)
    closesocket(sock);
    delete context;
    return 0;
}

// --- TCP 수신 함수 ---
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