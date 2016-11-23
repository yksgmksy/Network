#ifndef __NETWORK_H
#define __NETWORK_H
#include <winsock2.h>
#include <windows.h>
#include <cstdio>
#include <queue>
#include "Player.h"
#include "SkillOne.h"
#include "Boss.h"


//-----------------------------------------------------------

static SOCKET sock;

//-----------------------------------------------------------
using namespace std;

void err_quit(char *msg);
void err_display(char *msg);
int recvn(SOCKET s, char *buf, int len, int flags);
SOCKET init_sock();

struct ClientInfo{
	Player player; //플레이어의 정보
	queue <ArcherSkillOne> skill; //내 스킬들의 정보
};

struct treatedByServerInfo{
	Player *player; //다른 플레이어의 정보
	Boss *boss; //보스의 정보
	queue <ArcherSkillOne> skill; // 스킬들의 정보
	//queue<Projectiles> projectiles; //스킬들의 정보
	//queue<Effect> effect; //이펙트의 정보
};

bool recvFromServer(SOCKET s, treatedByServerInfo tbServerInfo, int flags);
// bool recvFromServer(SOCKET s, treatedByServerInfo tbServerInfo, int len, int flags);
bool sendToServer(SOCKET s, ClientInfo clientInfo, int flag);


#endif