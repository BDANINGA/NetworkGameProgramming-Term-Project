#include "network.h"

// recv_gameover() - 11.08
void recv_gameover(SOCKET socket, PacketHeader header, bool& gameover) {
    int received = recv(socket, (char*)&gameover + sizeof(PacketHeader), header.size, MSG_WAITALL);
    if (received == SOCKET_ERROR)
        std::cout << "error: recv_gameover" << std::endl;
}

void recv_renderdata(SOCKET socket, PacketHeader header, PacketRenderData& renderData) {
    int received = recv(socket, (char*)&renderData + sizeof(PacketHeader), header.size, MSG_WAITALL);
    if (received == SOCKET_ERROR)
        std::cout << "error: recv_renderdata" << std::endl;
}