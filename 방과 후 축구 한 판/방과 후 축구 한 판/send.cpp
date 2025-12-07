#include "network.h"

// ¡∂¿€(send_input)
void PlayerInput(int key_value, PacketInputkey& key, SOCKET sock, bool toggle) {
    if (key.key[key_value] != toggle) {
        key.key[key_value] = toggle;
        int sent = send(sock, (char*)&key, sizeof(PacketInputkey), 0);
        if (sent == SOCKET_ERROR)
            std::cerr << "error: PlayerInput()" << std::endl;
    }
}
void PlayerInput_special(int key_value, PacketInputspecialkey& key, SOCKET sock, bool toggle) {
    if (key.key[key_value] != toggle) {
        key.key[key_value] = toggle;
        int sent = send(sock, (char*)&key, sizeof(PacketInputspecialkey), 0);
        if (sent == SOCKET_ERROR)
            std::cerr << "error: PlayerInput_special()" << std::endl;
    }
}

void PlayerLogin(PacketLogin& logindata, SOCKET sock, char* id, char* pw, bool regist) {
	strcpy_s(logindata.userID, id);
	strcpy_s(logindata.userPW, pw);
	logindata.isRegister = regist;
    int sent = send(sock, (char*)&logindata, sizeof(PacketLogin), 0);
    if (sent == SOCKET_ERROR)
        std::cerr << "error: PlayerLogin()" << std::endl;
}

void PlayerReady(PacketGameReady& readydata, SOCKET sock, bool ready) {
	readydata.ready = ready;
    int sent = send(sock, (char*)&readydata, sizeof(PacketGameReady), 0);
    if (sent == SOCKET_ERROR)
        std::cerr << "error: PlayerReady()" << std::endl;
}