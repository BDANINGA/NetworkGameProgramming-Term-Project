#pragma once
#include "Packet.h"

// --- connect함수 ---
bool ConnectToServer(SOCKET& g_ServerSocket, const char* ipAddress, uint16_t port);

// --- 수신된 패킷을 처리하는 함수 ---
void ProcessPacket(SOCKET socket, const PacketHeader& header, char* payload);

// --- 클라이언트 네트워크 스레드 ---
DWORD WINAPI ClientNetworkThread(LPVOID lpParam);













// recv_gameover() - 11.08
void recv_gameover(SOCKET socket, PacketHeader header, bool* gameover);