#pragma comment(lib, "ws2_32")
#include "stdafx.h"
#include "ServerScene.h"
bool GameState = false;

//SceneData 만들기!
ServerData gameData;
HANDLE Event;
HANDLE TThread;

// 연결된 소켓 저장
std::vector<SOCKET> MatchingQueue;

/////////////////////////////////////////////////////////////////
// 오류 출력 부분
/////////////////////////////////////////////////////////////////
// 소켓 함수 오류 출력 후 종료
void err_quit(char* msg)
{
    LPVOID lpMsgBuf;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, WSAGetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf, 0, NULL);
    MessageBox(NULL, (LPCTSTR)lpMsgBuf, (LPCWSTR)msg, MB_ICONERROR);
    LocalFree(lpMsgBuf);
    exit(1);
}

// 소켓 함수 오류 출력
void err_display(char* msg)
{
    LPVOID lpMsgBuf;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, WSAGetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf, 0, NULL);
    printf("[%s] %s", msg, (char*)lpMsgBuf);
    LocalFree(lpMsgBuf);
}
////////////////////////////////////////////////////////////////////



// 타이틀 쓰레드 구현
DWORD WINAPI TitleThread(LPVOID arg)
{   
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
    retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR) err_quit("bind()");

    // listen()
    retval = listen(listen_sock, SOMAXCONN);
    if (retval == SOCKET_ERROR) err_quit("listen()");

    //초기 시작은 모든 플레이어가 준비 안된 상태이므로 false
    //또, 아무 키도 입력 받아선 안된다.   
    KeyInput Input{ 0 };

    // 데이터 통신에 사용할 변수
    SOCKET client_sock;
    SOCKADDR_IN clientaddr;
    int addrlen;


    while (1) {
        // accept()
        addrlen = sizeof(clientaddr);
        client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
        if (client_sock == INVALID_SOCKET) {
            err_display("accept()");
            break;
        }

        MatchingQueue.push_back(client_sock);

        char ip_addr[100];
        inet_ntop(AF_INET, &clientaddr.sin_addr, ip_addr, 100);

        // 연결된 클라이언트의 정보를 출력하자
        printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
            ip_addr, ntohs(clientaddr.sin_port));

        // 연결되었으므로 서버 상에서 플레이어를 만들어줘야한다.
        gameData.CreatePlayer(client_sock);

        ////////////////////////////////////////////////////////////////
        if (MatchingQueue.size() == MAX_PLAYER)
        {
            GameState = true;

            for (int i = 0; i < MAX_PLAYER; ++i)
            {
                retval = send(MatchingQueue[i], (char*)&GameState, sizeof(GameState), 0);

                TThread = CreateThread(NULL, 0, TitleThread, (LPVOID)MatchingQueue[i], 0, NULL);
                if (TThread == NULL) { closesocket(client_sock); }
            }          

            ExitThread(0);
        }
        else
        {
            for (int i = 0; i < MatchingQueue.size(); ++i)
            {
                retval = send(MatchingQueue[i], (char*)&GameState, sizeof(GameState), 0);
            }
        }
        //////////////////////////////////////////////////////////////// 

    }
}

DWORD WINAPI ProcessThread(LPVOID arg)
{
    printf("Running ProcessThread\n");

    SOCKET client_sock = (SOCKET)arg;
    SOCKADDR_IN clientaddr;

    int retval;
    Point Pos{ 0 };
    Point Bomb = Pos;
    KeyInput Input{ 0 };
    int addrlen;

    // 클라이언트 정보 얻기
    addrlen = sizeof(clientaddr);
    getpeername(client_sock, (SOCKADDR*)&clientaddr, &addrlen);

    // 클라이언트와 데이터 통신
    while (1) {

        retval = WaitForSingleObject(Event, INFINITE);

        // 데이터 받기 (recv())
        retval = recv(client_sock, (char*)&Input, sizeof(KeyInput), 0);    // char : 1byte
        if (retval == SOCKET_ERROR) {
            err_display("recv()");
            break;
        }
        else if (retval == 0)
            break;

        gameData.SetKeyInput(client_sock, Input);

        // 데이터 보내기 (send())

        MapData md[MAP_SIZE][MAP_SIZE];

        for (int i = 0; i < MAP_SIZE; i++)
            for (int j = 0; j < MAP_SIZE; j++)
                md[i][j] = gameData.GetMapData(i, j);
        retval = send(client_sock, (char*)&md, sizeof(md), 0);
        if (retval == SOCKET_ERROR)
        {
            err_display("XY send()");
            break;
        }
        SetEvent(Event);
    }

    // 윈속 종료
    WSACleanup();

    return 0;
}


int main(int argc, char* argv[])
{
    Event = CreateEvent(NULL, FALSE, TRUE, NULL);
    if (Event == NULL) return 1;

    TThread = CreateThread(NULL, 0, TitleThread, NULL, 0, NULL);
    if (TThread == NULL)
        printf("Create TThread Error\n");

    while (1)
    {
        printf("Running main \n");
        Sleep(10000);
    }
}