# Network-Game-Programming
## 개발일정
(1) 임성훈 <br>
(2) 박지성 <br>
(3) 김용채 <br> <br>
<span style="color:blue">●</span> **코드 구현 완료** <br>
<span style="color:red">●</span> **병합 준비중** <br>
<span style="color:green">●</span> **병합 완료** <br>
### 2025년 10월 일정
| 일 | 월 | 화 | 수 | 목 | 금 | 토 |
|---|---|---|---|---|---|---|
| 26 | 27 | 28 | 29 | 30 | 31 |  |
|||||<span style="color:green">(1) 기존Code Client/Server 나누기| <span style="color:green"> (1) packet 선언|
---

### 2025년 11월 일정

| 일 | 월 | 화 | 수 | 목 | 금 | 토 |
|---|---|---|---|---|---|---|
|   |    |   |  |  |  | 1 |
|   |   |    |    | | | <span style="color:green">(1) PlayerInput()|
| 2 | 3 | 4 | 5 | 6 | 7 | 8 |
|<span style="color:green">(3) connect(), listening(), accept() |   |<span style="color:green">(1) Gameover()|<span style="color:green">(3) ClientReveiveThread(), ServerReceiveThread()|<span style="color:green"> (2) ProcessClient(), ProcessServer() |   |<span style="color:green">(1) send_gameover(), recv_gameover()|
| 9 | 10 | 11 | 12 | 13 | 14 | 15 |
|<span style="color:green">(3) recv_input(), send_renderdata()|   |<span style="color:green">(2) MainThread()|<span style="color:green">(1) Tackle()| |<span style="color:green">(2) GameSessionLoop() (3) UI Update()| <span style="color:green">1차토의 |
| 16 | 17 | 18 | 19 | 20 | 21 | 22 |
|<span style="color:green">(1) test환경 구축|(2) ProcessGameStateUpdate()|(1) GameoverUI()|(1) send_score() (2) LoadGameSceneUI(), Reusdaddr 소켓 옵션| |(2) UpdateInGameUI|(1) Update Score(), PlayerChat()|
| 23 | 24 | 25 | 26 | 27 | 28 | 29 |
|(3) send_IDPW(), recv_IDPW(), send_login(), recv_login()|(2) ValidateLogin()|    |(1) send_chat() (2) LoadLoginScene()| |(1) BroadcastChat() (2) RegisterUser()| 2차토의  |
| 30 |  
| (1) ChatUI() (3) send_ready(), recv_ready(), 2ready?  |
---

### 2025년 12월 일정

| 일 | 월 | 화 | 수 | 목 | 금 | 토 |
|---|---|---|---|---|---|---|
|   | 1 | 2 | 3 | 4 | 5 | 6 |
|   |(2) OnClick_LoginScene()|    |    | |   | 3차토의  |
| 7 | 8 | 9 | 10 | 11 | 12 | 13 |
|   |   |    |  최종 테스트 및 결과물 제출  | |   |   |
---


