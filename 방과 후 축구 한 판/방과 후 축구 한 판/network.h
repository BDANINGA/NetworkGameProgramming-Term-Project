#pragma once
#include "Packet.h"

// --- connect함수 ---
bool ConnectToServer(SOCKET& g_ServerSocket, const char* ipAddress, uint16_t port);

// --- 수신된 패킷을 처리하는 함수 ---
void ProcessPacket(SOCKET socket, const PacketHeader& header);

// --- 클라이언트 네트워크 스레드 ---
DWORD WINAPI ClientNetworkThread(LPVOID lpParam);

// -----------------------------------------------------------------------------
// 조작(send_input)
void PlayerInput(int key_value, PacketInputkey& key, SOCKET sock, bool toggle);
void PlayerInput_special(int key_value, PacketInputspecialkey& key, SOCKET sock, bool toggle);

// recv_gameover() - 11.08
void recv_gameover(SOCKET socket, PacketHeader header, bool* gameover);
void recv_renderdata(SOCKET socket, PacketHeader header, PacketRenderData* renderData);