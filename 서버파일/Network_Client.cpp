#include "Network.h"



treatedByServerInfo *totalServerInfo;
ClientInfo clientInfo;

// ���� �Լ� ���� ��� �� ����
void err_quit(char *msg) {
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

// ���� �Լ� ���� ���
void err_display(char *msg) {
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char *)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

int recvn(SOCKET s, char *buf, int len, int flags) {
	int received;
	char *ptr = buf;
	int left = len;

	while (left > 0) {
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (received == 0)
			break;
		left -= received;
		ptr += received;
	}

	return (len - left);
}

SOCKET init_sock() {
	int retval;
	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
	}
	// socket()
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	// connect()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect()");

	printf("[�˸�] %s:%d ���������� ���� �Ǿ����ϴ�..!\n", SERVERIP, SERVERPORT);
	return sock;
}

// ������ �����͸� ������.
bool sendToServer(SOCKET s, ClientInfo clientInfo, int flag) {
	int retval;
	// ������ ������( ����ü ũ�⸦ ���� ������. )
	int len = sizeof(clientInfo);
	retval = send(s, (char *)&len, sizeof(int), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
		exit(1);
	}

	// ������ ������( ����ü �����͸� ������. )
	retval = send(s, (char*)&clientInfo, sizeof(ClientInfo), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
		exit(1);
	}

	return 0;
}

// �������� �����͸� �޴´�.
bool recvFromServer(SOCKET s, treatedByServerInfo tbServerInfo, int flags) {
	int retval;
	int len;
	retval = recvn(s, (char *)&len, sizeof(int), 0); // ������ �ޱ�(���� ����)
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
	}
	else if (retval == 0) {
	}

	//retval = recv(s, (char*)&tbServerInfo, len, 0);

	int GetSize;
	char tempBuffer[BUFSIZE];  // �ӽ�����
	// ũ�⸸ŭ ����ü �ޱ�
	GetSize = recv(s, (char*)&tbServerInfo, len, 0);
	
	// �ʱ�ȭ �� �����Ͱ� ����
	tempBuffer[GetSize] = '\0';
	totalServerInfo = (treatedByServerInfo*)tempBuffer;

	return 0;
}



//// �������� �����͸� �޴´�.
//bool recvFromServer(SOCKET s, treatedByServerInfo tbServerInfo, int len, int flags) {
//	int retval;
//	int len;
//	retval = recvn(s, (char *)&len, sizeof(int), 0); // ������ �ޱ�(���� ����)
//	if (retval == SOCKET_ERROR) {
//		err_display("recv()");
//	}
//	else if (retval == 0) {
//	}
//
//
//	retval=recv(s, (char*)&tbServerInfo, len, 0);
//
//	//printf( "������ ũ�� : %d\n", len );
//
//	int GetSize;
//	char suBuffer[BUFSIZE];
//	Server_Player *player;
//	GetSize = recv(s, (char*)&tbServerInfo, len, 0);
//
//	suBuffer[GetSize] = '\0';
//	player = (Server_Player*)suBuffer;
//
//
//
//	return (Server_Player&)player;
//}

