#include "Network_Server.h"

bool client[MAX_Client] = { false }; //클라이언트가 들어오는 대로 처리한다.


//typedef struct Player_Socket {
//	float x = 0, y = 0, z = 0;
//}Player_Socket;
//
//typedef struct Player {
//	float camxrotate = 0.0f, camyrotate = -90.0f;
//	Player_Socket socket;
//}Player;
//
//typedef struct Server_Player {
//	Player player[MAX_Client];
//}Server_Player;
//
//
//Server_Player server_data;


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

// 사용자 정의 데이터 수신 함수
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


DWORD WINAPI ProcessClient(LPVOID arg) {
	SOCKET client_sock = (SOCKET)arg;
	SOCKADDR_IN clientaddr;
	int addrlen;
	int retval;
	int len;
	
	// 일단 걍 만듬.. 클라 접속시 0번부터 고유번호를 준다.
	int g_Client = -1;

	char buf[BUFSIZE + 1];

	// 클라이언트 정보 얻기
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (SOCKADDR *)&clientaddr, &addrlen);
	for (int i = 0; i < MAX_Client; ++i) {
		if (client[i] == false) {
			client[i] = true;
			g_Client = i;
			printf("Client ID : %d\n", g_Client);
			break;
		}
	}
	if (g_Client == -1) {
		// 인원이 꽉참 더이상 못들어 온다.
		closesocket(client_sock);
		return 0;
	}

	while (1) {
		retval = recvn(client_sock, (char *)&len, sizeof(int), 0); // 데이터 받기(고정 길이)
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		else if (retval == 0)
			break;
		//printf( "데이터 크기 : %d\n", len );

		int GetSize;
		char tempBuffer[BUFSIZE];
		
		GetSize = recv(client_sock, tempBuffer, len, 0);

		tempBuffer[GetSize] = '\0';
		clientInfo = (ClientInfo*)tempBuffer;

		// 데이터 받기(가변 길이)
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		else if (retval == 0)
			break;


		int retval;
		// 데이터 보내기( 구조체 크기를 먼저 보낸다. )
		int len = sizeof(totalServerInfo);
		retval = send(client_sock, (char *)&len, sizeof(int), 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
			exit(1);
		}

		// 데이터 보내기( 구조체 데이터를 보낸다. )
		retval = send(client_sock, (char*)&totalServerInfo, sizeof(totalServerInfo), 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
			exit(1);
		}


	}
	closesocket(client_sock);
	client[g_Client] = false;

	return 0;
}

int main(int argc, char *argv[]) {
	int retval;
	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// socket()
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	// bind()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	// 데이터 통신에 사용할 변수
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	HANDLE hThread;

	printf("Server is Ready..!\n\n");

	while (1) {
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}

		// 접속한 클라이언트 정보 출력
		printf("\n클라이언트 접속: IP 주소=%s, 포트 번호=%d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

		// 스레드 생성
		hThread = CreateThread(NULL, 0, ProcessClient, (LPVOID)client_sock, 0, NULL);
		if (hThread == NULL) {
			closesocket(client_sock);
		}
		else {
			CloseHandle(hThread);
		}

	}
	// closesocket()
	closesocket(listen_sock);

	// 윈속 종료
	WSACleanup();
	return 0;
}