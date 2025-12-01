#pragma once
#include "include.h"
#pragma pack(push, 1)
#define PKT_LOGIN_RESULT 100
#define PKT_USER_DATA 101
#define PKT_RENDER_DATA 102
#define PKT_GAMEOVER 103
#define PKT_INPUT_KEY 104
#define PKT_INPUT_SPECIALKEY 105
#define PKT_LOGIN 106
#define PKT_GAME_READY 107
#define PKT_CHAT_MESSAGE 108

struct PacketHeader {
	uint16_t type;    // 패킷 종류(ex.로그인, 유저데이터, 키입력 등)
	uint16_t size;    // 데이터 크기
};

// Server -> Client
struct PacketLoginResult {
	PacketHeader header{ htons(PKT_LOGIN_RESULT), htons(sizeof(PacketLoginResult) - sizeof(PacketHeader)) };
	uint8_t success;
	int myPlayerID;
	char message[64];      // 실패 시 이유 or 성공 메시지
};

struct UserData {
	int totalMatch;
	int win;
	int lose;
	float winRate;
};

struct PacketUserData {
	PacketHeader header{ htons(PKT_USER_DATA), htons(sizeof(PacketUserData) - sizeof(PacketHeader)) };
	UserData data;
};

struct PlayerData {
	int id;
	glm::vec3 position;
	glm::vec3 rotation;
};

struct BallData {
	glm::vec3 position;
	glm::vec3 rotation;
	float rotationAngle;
};

struct KeeperData {
	glm::vec3 position;
	glm::vec3 rotation;
};

struct PacketRenderData {
	PacketHeader header{ htons(PKT_RENDER_DATA), htons(sizeof(PacketRenderData) - sizeof(PacketHeader)) };
	PlayerData p_data[3];
	BallData b_data;
	KeeperData k_data;

	uint8_t playerScore[3];
	time_t remainingTime;
};

struct PacketGameover {
	PacketHeader header{ htons(PKT_GAMEOVER), htons(sizeof(PacketGameover) - sizeof(PacketHeader)) };
	uint8_t gameover;
};

// Client -> Server
struct PacketInputkey {
	PacketHeader header{ htons(PKT_INPUT_KEY), htons(sizeof(PacketInputkey) - sizeof(PacketHeader)) };
	uint8_t key[256];
};
struct PacketInputspecialkey {
	PacketHeader header{ htons(PKT_INPUT_SPECIALKEY), htons(sizeof(PacketInputkey) - sizeof(PacketHeader)) };
	uint8_t key[256];
};

struct PacketLogin {
	PacketHeader header{ htons(PKT_LOGIN), htons(sizeof(PacketLogin) - sizeof(PacketHeader)) };
	char userID[32];
	char userPW[32];
};

struct PacketGameReady {
	PacketHeader header{ htons(PKT_GAME_READY), htons(sizeof(PacketGameReady) - sizeof(PacketHeader)) };
	uint8_t ready;
};

struct PacketChatMessage {
	PacketHeader header{ htons(PKT_CHAT_MESSAGE), htons(sizeof(PacketChatMessage) - sizeof(PacketHeader)) };
	char message[256];
};

#pragma pack(pop)