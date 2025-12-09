#include "network.h"

// send_gameover - 11.8일 구현
void send_gameover(SOCKET& socket) {
    PacketGameover gameover;
    gameover.gameover = true;
    int sent = send(socket, (char*)&gameover, sizeof(PacketGameover), 0);
    if (sent == SOCKET_ERROR)
        std::cerr << "error: send_gameover()" << std::endl;
}

// send_userdata - 11.19 구현
void send_userdata(SOCKET& socket, PacketUserData& userdata) {
    int sent = send(socket, (char*)&userdata, sizeof(PacketUserData), 0);
    if (sent == SOCKET_ERROR)
        std::cerr << "error: send_userdata()" << std::endl;
}

void send_loginresult(SOCKET& socket, PacketLoginResult& loginresult, bool result, char* message) {
	loginresult.success = result;
	strcpy_s(loginresult.message, message);
    int sent = send(socket, (char*)&loginresult, sizeof(PacketLoginResult), 0);
    if (sent == SOCKET_ERROR)
        std::cerr << "error: send_loginresult()" << std::endl;
}