#include "Network.h"



treatedByServerInfo *totalServerInfo;
ClientInfo clientInfo;

// 소켓 함수 오류 출력 후 종료
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

// 소켓 함수 오류 출력
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
	// 윈속 초기화
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

	printf("[알림] %s:%d 정상적으로 연결 되었습니다..!\n", SERVERIP, SERVERPORT);
	return sock;
}

// 서버에 데이터를 보낸다.
bool sendToServer(SOCKET s, ClientInfo clientInfo, int flag) {
	int retval;
	// 데이터 보내기( 구조체 크기를 먼저 보낸다. )
	int len = sizeof(clientInfo);
	retval = send(s, (char *)&len, sizeof(int), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
		exit(1);
	}

	// 데이터 보내기( 구조체 데이터를 보낸다. )
	retval = send(s, (char*)&clientInfo, sizeof(ClientInfo), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
		exit(1);
	}

	return 0;
}

// 서버에게 데이터를 받는다.
bool recvFromServer(SOCKET s, treatedByServerInfo tbServerInfo, int flags) {
	int retval;
	int len;
	retval = recvn(s, (char *)&len, sizeof(int), 0); // 데이터 받기(고정 길이)
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
	}
	else if (retval == 0) {
	}

	//retval = recv(s, (char*)&tbServerInfo, len, 0);

	int GetSize;
	char tempBuffer[BUFSIZE];  // 임시저장
	// 크기만큼 구조체 받기
	GetSize = recv(s, (char*)&tbServerInfo, len, 0);
	
	// 초기화 및 데이터값 저장
	tempBuffer[GetSize] = '\0';
	totalServerInfo = (treatedByServerInfo*)tempBuffer;

	return 0;
}



//// 서버에게 데이터를 받는다.
//bool recvFromServer(SOCKET s, treatedByServerInfo tbServerInfo, int len, int flags) {
//	int retval;
//	int len;
//	retval = recvn(s, (char *)&len, sizeof(int), 0); // 데이터 받기(고정 길이)
//	if (retval == SOCKET_ERROR) {
//		err_display("recv()");
//	}
//	else if (retval == 0) {
//	}
//
//
//	retval=recv(s, (char*)&tbServerInfo, len, 0);
//
//	//printf( "데이터 크기 : %d\n", len );
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

