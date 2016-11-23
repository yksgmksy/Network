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
	Player player; //�÷��̾��� ����
	queue <ArcherSkillOne> skill; //�� ��ų���� ����
};

struct treatedByServerInfo{
	Player *player; //�ٸ� �÷��̾��� ����
	Boss *boss; //������ ����
	queue <ArcherSkillOne> skill; // ��ų���� ����
	//queue<Projectiles> projectiles; //��ų���� ����
	//queue<Effect> effect; //����Ʈ�� ����
};

bool recvFromServer(SOCKET s, treatedByServerInfo tbServerInfo, int flags);
// bool recvFromServer(SOCKET s, treatedByServerInfo tbServerInfo, int len, int flags);
bool sendToServer(SOCKET s, ClientInfo clientInfo, int flag);


#endif