#include "network.h"

// send_gameover - 11.8일 구현
void send_gameover(SOCKET& socket) {
    PacketGameover gameover;
    gameover.gameover = true;
    int sent = send(socket, (char*)&gameover, sizeof(PacketGameover), 0);
    if (sent == sizeof(PacketGameover))
        std::cerr << "error: send_gameover()" << std::endl;
}

// send_userdata - 11.19 구현
void send_userdata(SOCKET& socket, PacketUserData& userdata) {
    int sent = send(socket, (char*)&userdata, sizeof(PacketUserData), 0);
    if (sent == sizeof(PacketUserData))
        std::cerr << "error: send_userdata()" << std::endl;
}