#include "include.h" 
#include "network.h"        
#include "Player.h"           
#include "Keeper.h"     

#define PORT 9000

// --- 사용할 전역 변수 ---
PacketInputkey g_LatestInputKey[MAX_PLAYERS];
PacketInputspecialkey g_LatestInputSpecialKey[MAX_PLAYERS];

// --- 전역 변수 보호용 Lock ---
CRITICAL_SECTION g_InputLock;

uint16_t currentScore[3]{ 0,0,0 };


void GameSessionLoop(SOCKET clientSockets[]) {

    // --- 게임 월드(World) 초기화 (수정 필요)---
    Player players[MAX_PLAYERS];
    Ball ball(0, 0, 0);
    Keeper keeper(0, 0, -32);

    // 플레이어 초기 위치
    for (int i = 0; i < MAX_PLAYERS; ++i) {
        glm::vec3 random_DirVec = randomDir_xz();
        random_DirVec.y = 0;
        players[i].setPosition(random_DirVec * 10.0f);
    }

    // 시작 시간 설정
    time_t startTime = time(NULL);

    // 게임 스코어 초기화

    //PacketRenderData 준비
    PacketRenderData statePkt;

    std::cout << "--- Game Loop Started ---" << std::endl;

    // --- 메인 루프 시작 ---
    while (true) {
        // 저장된 최신 데이터를 지역 변수로 복사 
        PacketInputkey playerKeys[MAX_PLAYERS];
        PacketInputspecialkey playerSpecialKeys[MAX_PLAYERS];

        memcpy(playerKeys, g_LatestInputKey, sizeof(g_LatestInputKey));
        memcpy(playerSpecialKeys, g_LatestInputSpecialKey, sizeof(g_LatestInputSpecialKey));


        // 저장된 최신 데이터로 연산 (게임 로직)

        // 입력값 적용
        for (int i = 0; i < MAX_PLAYERS; i++) {
            for (int k = 0; k < 256; k++) {
                if (playerKeys[i].key[k]) players[i].keyDown2(k);
                else players[i].keyUp2(k);

                if (playerSpecialKeys[i].key[k]) players[i].keyDown(k);
                else players[i].keyUp(k);
            }
        }
        keeper.Move(ball.getPosition());
        ball.Move(keeper.getPosition(), keeper.ishasBall());

        // 객체 Move() 호출
        for (int i = 0; i < MAX_PLAYERS; i++) {
            players[i].changeSprint();
            players[i].changeShooting();
            players[i].changeCurve();
            players[i].changeStrong();
            players[i].TackleCool();
            players[i].DoTackle();
            

            if (players[i].ishasBall() && players[i].isShooting()) {
                ball.setLastOwner(i);
                players[i].ShootInProgress(ball);
            }
            if (players[i].isShoot())
                players[i].Shoot(ball);
            players[i].Move(ball, keeper.ishasBall());
        }

        TackleEvent(players, MAX_PLAYERS, ball);

        // 충돌 및 골, 시간 종료 등
        for (int i = 0; i < MAX_PLAYERS; i++) {
            Gameover(time(NULL) - startTime, clientSockets[i]);
        }


        // --- 데이터 채우기/보내기 ---

        // 3명의 플레이어 데이터 채우기
        for (int i = 0; i < MAX_PLAYERS; i++) {
            statePkt.p_data[i].id = i; // 플레이어 ID
            statePkt.p_data[i].position = players[i].getPosition();
            statePkt.p_data[i].rotation = players[i].getRotation();
        }

        // 공, 키퍼 데이터 채우기
        statePkt.b_data.position = ball.getPosition();
        statePkt.b_data.rotation = ball.getRotation();
        statePkt.b_data.rotationAngle = ball.getRotationAngle();

        statePkt.k_data.position = keeper.getPosition();
        statePkt.k_data.rotation = keeper.getRotation();

        // 현재 스코어 데이터 채우기
        for (int i = 0; i < 3; i++) {
            statePkt.playerScore[i] = currentScore[i];
        }

        // 남은 시간 데이터 채우기
        statePkt.remainingTime = 300 - (time(NULL) - startTime);

        // 모든 클라이언트에게 'send' 
        for (int i = 0; i < MAX_PLAYERS; i++) {
            if (send(clientSockets[i], (char*)&statePkt, sizeof(PacketRenderData), 0) == SOCKET_ERROR) {
                std::cerr << "Send failed to player " << i << std::endl;
            }
        }
        
        Sleep(1);
    }
}


int main() {

    // --- 리슨 소켓 설정 ---
    ClientContext listenContext;
    if (!ListenForClients(listenContext, PORT)) {
        return -1; // 서버 시작 실패
    }
    SOCKET listenSocket = listenContext.socket;


    // --- 클라이언트 접속 대기 (3명) ---
    SOCKET clientSockets[MAX_PLAYERS];
    HANDLE hRecvThreads[MAX_PLAYERS];

    for (int i = 0; i < MAX_PLAYERS; i++) {
        std::cout << "Waiting for player " << i << "..." << std::endl;

        // 클라이언트 접속 수락
        clientSockets[i] = AcceptClient(listenSocket);
        if (clientSockets[i] == INVALID_SOCKET) {
            i--; // 해당 클라이언트 접속 재시도
            continue;
        }
        std::cout << "Player " << i + 1 << " connected" << std::endl;

        // 수신 스레드용 컨텍스트 생성
        ClientContext* pClientContext = new ClientContext();
        pClientContext->socket = clientSockets[i];
        pClientContext->playerID = i;

        // 수신 스레드 생성
        hRecvThreads[i] = CreateServerReceiveThread(pClientContext);
        if (hRecvThreads[i] == NULL) {
            std::cerr << "Failed to create thread for player " << i << std::endl;
        }
    }

    std::cout << "All players connected. Game Start..." << std::endl;
    closesocket(listenSocket);

    // --- 메인 스레드 안에서 게임 루프 실행 ---
    GameSessionLoop(clientSockets);

    // --- 종료 처리 ---
    std::cout << "Game loop ended. GameOver..." << std::endl;

    for (int i = 0; i < MAX_PLAYERS; i++) {
        closesocket(clientSockets[i]);
        WaitForSingleObject(hRecvThreads[i], INFINITE);
        CloseHandle(hRecvThreads[i]);
    }

    WSACleanup();
    return 0;
}