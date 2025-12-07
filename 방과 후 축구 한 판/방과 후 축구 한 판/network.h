#pragma once
#include "Packet.h"

#define MAX_PLAYERS 3

extern int g_CurrentScores[MAX_PLAYERS];
extern int g_SecondsRemaining;
extern char BroadCastChatMessage[9][256];

// --- connect함수 ---
bool ConnectToServer(SOCKET& g_ServerSocket, const char* ipAddress, uint16_t port);


// --- 클라이언트 네트워크 스레드 ---
DWORD WINAPI ClientNetworkThread(LPVOID lpParam);

// -----------------------------------------------------------------------------
// 조작(send_input)
void PlayerInput(int key_value, PacketInputkey& key, SOCKET sock, bool toggle);
void PlayerInput_special(int key_value, PacketInputspecialkey& key, SOCKET sock, bool toggle);

bool RecvTCP(SOCKET sock, char* buffer, int size);

bool RecvTCP(SOCKET sock, char* buffer, int size);

void PlayerLogin(PacketLogin& logindata, SOCKET sock, char* id, char* pw, bool regist);

void PlayerReady(PacketGameReady& readydata, SOCKET sock, bool ready);

void PlayerChat(PacketChatMessage& chatmessage, SOCKET sock);