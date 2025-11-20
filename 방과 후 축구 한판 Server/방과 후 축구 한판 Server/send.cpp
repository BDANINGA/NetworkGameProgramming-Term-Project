#include "network.h"

// send_gameover - 11.8일 구현
void send_gameover(SOCKET& socket) {
    PacketGameover gameover;
    gameover.gameover = true;
    int sent = send(socket, (char*)&gameover, sizeof(PacketGameover), 0);
    if (sent == sizeof(PacketGameover))
        std::cerr << "error: send_gameover()" << std::endl;
}

void send_renderdata(SOCKET socket, PacketRenderData& renderdata) {
    int sent1 = send(socket, (char*)&renderdata, sizeof(PacketRenderData), 0);
    if (sent1 == sizeof(PacketHeader))
        printf("error send_renderdata()");
}

// send_userdata - 11.19 구현
void send_userdata(SOCKET& socket, PacketUserData& userdata) {
    int sent = send(socket, (char*)&userdata, sizeof(PacketUserData), 0);
    if (sent == sizeof(PacketUserData))
        std::cerr << "error: send_userdata()" << std::endl;
}