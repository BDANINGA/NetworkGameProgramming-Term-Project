#pragma once
#include "Packet.h"

#define MAX_PLAYERS 3
#define ENDTIME 300

extern PacketInputkey g_LatestInputKey[MAX_PLAYERS];
extern PacketInputspecialkey g_LatestInputSpecialKey[MAX_PLAYERS];

// --- 스레드에 넘겨줄 인수 ---
struct ClientContext {
    SOCKET socket;
    int playerID;
};
    
// --- listen함수 ---
bool ListenForClients(ClientContext& context, uint16_t port);

// --- accept 함수 ---
SOCKET AcceptClient(SOCKET listenSocket);

// -- 쓰레드 생성 --
HANDLE CreateServerReceiveThread(ClientContext* context);

// --- 서버 측 수신 스레드 ---
DWORD WINAPI ServerReceiveThread(LPVOID lpParam);

// --- TCP 수신 함수 ---
bool RecvTCP(SOCKET sock, char* buffer, int size);








// send_gameover - 11.8일 구현
void send_gameover(SOCKET& socket);

// send_userdata - 11.19 구현
void send_userdata(SOCKET& socket, PacketUserData& userdata);