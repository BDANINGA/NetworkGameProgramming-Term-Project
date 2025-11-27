#include "network.h"

// 조작(send_input)
void PlayerInput(int key_value, PacketInputkey& key, SOCKET sock, bool toggle) {
    if (key.key[key_value] != toggle) {
        key.key[key_value] = toggle;
        int sent = send(sock, (char*)&key, sizeof(PacketInputkey), 0);
        if (sent == SOCKET_ERROR)
            std::cerr << "error: PlayerInput()" << std::endl;
        std::cout << "키 입력: " << key_value << std::endl;
    }
}
void PlayerInput_special(int key_value, PacketInputspecialkey& key, SOCKET sock, bool toggle) {
    if (key.key[key_value] != toggle) {
        key.key[key_value] = toggle;
        int sent = send(sock, (char*)&key, sizeof(PacketInputspecialkey), 0);
        if (sent == SOCKET_ERROR)
            std::cerr << "error: PlayerInput_special()" << std::endl;
        std::cout << "키 입력: " << key_value << std::endl;
    }
}