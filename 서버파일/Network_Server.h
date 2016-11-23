#pragma once
#pragma comment(lib, "ws2_32")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <stdlib.h>
#include <atlstr.h> // cstring
#include <Windows.h>
#include <stdio.h>
#include <queue>

using namespace std;

#define SERVERPORT 9000
#define BUFSIZE    1024
#define MAX_Client 4

class Player {
	float xpos;
	float ypos;
};

// 서버에서 처리한 데이터의 정보를 담는 구조체.
struct TreatedByServerInfo {
	Player *player; //다른 플레이어의 정보
	//Boss *boss; //보스의 정보
	//queue<Projectiles> projectiles; //스킬들의 정보
	//queue<Effect> effect; //이펙트의 정보
};
TreatedByServerInfo totalServerInfo;



// 클라이언트로부터 받을 데이터의 정보를 담는 구조체.
struct ClientInfo {
	Player player; //플레이어의 정보
	//queue <Skill> skill; //내 스킬들의 정보
};

ClientInfo *clientInfo;

// 클라이언트에 처음에 보낼 맵의 정보를 담는 구조체
struct mapInfo {
	int  **first_mapInfo;
	int  **second_mapInfo;
};

// 클라이언트로부터 받은 데이터로 처리해야 할 데이터를 처리한다.
TreatedByServerInfo processClientData(ClientInfo *p_clientInfo);

// 클라이언트로부터 데이터를 받는다
bool recvFromClient(SOCKET s, ClientInfo clientInfo, int len, int flags);

// 고정-가변 길이의 데이터 전송방식으로 데이터를 보낸다
bool sentToClient(SOCKET s, int len, TreatedByServerInfo tbServerInfo, int flag);

// 인자 - 총 받을 수 있는 클라이언트 개수 ( 관련 속성 : g_Client )
bool IsClientPull(int Client);

// 클라이언트에게 방장 권한을 준다. 
bool sendChiefClient(SOCKET s, BOOL isChief, int len, int flags);

// 게임 씬으로 넘어간 정보를 변경하고 성공여부를 리턴
bool StartGameMessageFromClient(int sceneNum);

// 멀티스레드 작업을 할 때, 각 스레드를 모두 처리했는지 단순 체크 함수
// 인자 – 스레드 개수,  if( g_Client == 처리한 스레드 개수 ) return true
bool SyncThread(int nThread);