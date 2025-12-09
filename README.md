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
|ㅣ|담당자|ㅣ|주요 기능|ㅣ|완성 여부|ㅣ|
|---|:---:|---|:---:|---|:---:|---|
|ㅣ|임성훈|ㅣ|기존 코드 Client/Server 분할|ㅣ|O|ㅣ|
|ㅣ|임성훈|ㅣ|Packet 설계 및 구현|ㅣ|O|ㅣ|
|ㅣ|임성훈|ㅣ|Input IP Address|ㅣ|O|ㅣ|
|ㅣ|김용채|ㅣ|ConnectToServer()|ㅣ|O|ㅣ|
|ㅣ|김용채|ㅣ|ListenForClients()|ㅣ|O|ㅣ|
|ㅣ|김용채|ㅣ|AcceptClient()|ㅣ|O|ㅣ|

### 2. Client/Server Thread
|ㅣ|담당자|ㅣ|주요 기능|ㅣ|완성 여부|ㅣ|
|---|:---:|---|:---:|---|:---:|---|
|ㅣ|박지성|ㅣ|ServerReceiveThread|ㅣ|O|ㅣ|
|ㅣ|박지성|ㅣ|ClientNetworkThread|ㅣ|O|ㅣ|
|ㅣ|박지성|ㅣ|GameSessionLoop(ServerMainThread)|ㅣ|O|ㅣ|
|ㅣ|김용채|ㅣ|CreateServerReceiveThread|ㅣ|O|ㅣ|
|ㅣ|김용채|ㅣ|CreateClientNetworkThread|ㅣ|O|ㅣ|

### 3. Send/Recv/Process Packet
|ㅣ|담당자|ㅣ|주요 기능|ㅣ|완성 여부|ㅣ|
|---|:---:|---|:---:|---|:---:|---|
|ㅣ|임성훈|ㅣ|PacketInputKey/PacketInputspecialKey|ㅣ|O|ㅣ|
|ㅣ|임성훈|ㅣ|PacketChatMessage, BroadCastChatMessage|ㅣ|O|ㅣ|
|ㅣ|임성훈|ㅣ|PacketGameover|ㅣ|O|ㅣ|
|ㅣ|박지성|ㅣ|PacketRenderData|ㅣ|O|ㅣ|
|ㅣ|김용채|ㅣ|PacketLogin/PacketLoginResult/PacketGameReady|ㅣ|O|ㅣ|

### 4. Additional Content
|ㅣ|담당자|ㅣ|주요 기능|ㅣ|완성 여부|ㅣ|
|---|:---:|---|:---:|---|:---:|---|
|ㅣ|임성훈|ㅣ|Chat|ㅣ|O|ㅣ|
|ㅣ|임성훈|ㅣ|Player_Tackle|ㅣ|O|ㅣ|
|ㅣ|김용채|ㅣ|Login|ㅣ|O|ㅣ|
