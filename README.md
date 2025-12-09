# Network-Game-Programming
## 개발일정
(1) 임성훈 <br>
(2) 박지성 <br>
(3) 김용채 <br> <br>

**코드 구현 완료: S(병합 준비 완료)**<br>
**병합 완료: P(최종 통과)** <br>
**문제 발생: N(병합 실패, 수정 고려)**

Commit Rule: 구현이 완료된 코드는 팀장이 테스트가 되는 지 확인 후 병합 후 테스트까지 합니다. 

### 2025년 10월 일정
| 일 | 월 | 화 | 수 | 목 | 금 | 토 |
|---|---|---|---|---|---|---|
| 26 | 27 | 28 | 29 | 30: P | 31: P |  |
|||||(1) 기존Code Client/Server 나누기|(1) packet 선언|
---

### 2025년 11월 일정

| 일 | 월 | 화 | 수 | 목 | 금 | 토 |
|---|---|---|---|---|---|---|
|   |    |   |  |  |  | 1 |
|   |   |    |    | | | (1) PlayerInput()|
| 2: P | 3 | 4: P | 5: P | 6: P | 7: P | 8: P |
|(3) connect(), listening(), accept() |   |(1) Gameover()|(3) ClientReveiveThread(), ServerReceiveThread()|(2) ProcessClient(), ProcessServer() |   |(1) send_gameover(), recv_gameover()|
| 9: P | 10 | 11: P | 12: P | 13 | 14: P | 15: P |
|(3) recv_input(), send_renderdata()|   |(2) MainThread()|(1) Tackle()| |(2) GameSessionLoop() (1) UI Update()|1차토의 |
| 16: P | 17: P | 18: P | 19: P | 20 | 21: P | 22 |
|(1) test환경 구축|(2) ProcessGameStateUpdate()|(1) GameoverUI()|(1) send_userdata() (2) LoadGameSceneUI(), Reusdaddr 소켓 옵션| |(2) UpdateInGameUI||
| 23 | 24 | 25 | 26: P | 27: P | 28: P | 29: P |
||||(1) Error:키 값 중복 해결 (3) send_IDPW(), recv_IDPW(), send_login(), recv_login()|(2) Players_Camera()  |(1) Error: 키 값에 대한 행동처리문제 해결| 2차토의 (온라인)  |
| 30: P |  
|(2) Error: 렌더링 지연 해결 1(네이글 알고리즘) (3) send_ready(), recv_ready()|
---

### 2025년 12월 일정

| 일 | 월 | 화 | 수 | 목 | 금 | 토 |
|---|---|---|---|---|---|---|
|| 1: P | 2: P | 3: P | 4: P| 5: P| 6: P|
| |2차토의(오프라인)|(2) 새 게임 규칙 설정 및 구현|(1) Error: 기존 드리블 수정 (2) Error: 렌더링 지연 해결 2(클라이언트의 카메라 문제)|(1) Error: 기존 슛 수정, PlayerChat(), ChatUI() (3) LoadLoginScene()|(1) send_chat(), BroadcastChat() (2) 준비 시작 단계 UI (3) RegisterUser()|3차토의 (온라인) (1) Update_userdata(), |
| 7: P| 8: P| 9: P| 10| 11| 12 | 13 |
|(2) 사운드 적용|마지막 토의(오프라인)|Demo Test(오프라인)|  최종 테스트 및 결과물 제출  | |   |   |


## Self Check List
### 1. Client/Server Connect 
|담당자|주요 기능|완성 여부|
|:---:|:---:|:---:|
|임성훈|기존 코드 Client/Server 분할|O|
|임성훈|Packet 설계 및 구현|O|
|임성훈|Input IP Address|O|
|김용채|ConnectToServer()|O|
|김용채|ListenForClients()|O|
|김용채|AcceptClient()|O|

### 2. Client/Server Thread
|담당자|주요 기능|완성 여부|
|:---:|:---:|:---:|
|박지성|ServerReceiveThread|O|
|박지성|ClientNetworkThread|O|
|박지성|GameSessionLoop(ServerMainThread)|O|
|김용채|CreateServerReceiveThread|O|
|김용채|CreateClientNetworkThread|O|

### 3. Send/Recv/Process Packet
|담당자|주요 기능|완성 여부|
|:---:|:---:|:---:|
|임성훈|PacketInputKey/PacketInputspecialKey|O|
|임성훈|PacketChatMessage, BroadCastChatMessage|O|
|임성훈|PacketGameover|O|
|박지성|PacketRenderData|O|
|김용채|PacketLogin/PacketLoginResult/PacketGameReady|O|

### 4. Additional Content
|담당자|주요 기능|완성 여부|
|:---:|:---:|:---:|
|임성훈|Chat/ChatUI|O|
|임성훈|GameOver/GameOverScene|O|
|임성훈|Player_Tackle|O|
|박지성|LoginScene|O|
|김용채|Login/LoginUI|O|
