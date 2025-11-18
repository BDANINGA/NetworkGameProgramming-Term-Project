# Network-Game-Programming
## 개발일정
(1) 임성훈 <br>
(2) 박지성 <br>
(3) 김용채 <br> <br>

**코드 구현 완료: S**<br>
**병합 완료: P** <br>
**문제 발생: N**

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
|(3) recv_input(), send_renderdata()|   |(2) MainThread()|(1) Tackle()| |(2) GameSessionLoop() (3) UI Update()|1차토의 |
| 16: P | 17 | 18 | 19 | 20 | 21 | 22 |
|(1) test환경 구축|(2) ProcessGameStateUpdate()|(1) GameoverUI()|(1) send_score() (2) LoadGameSceneUI(), Reusdaddr 소켓 옵션| |(2) UpdateInGameUI|(1) Update Score(), PlayerChat()|
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


