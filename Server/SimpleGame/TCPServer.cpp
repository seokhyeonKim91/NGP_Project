#pragma comment(lib, "ws2_32")
#include "stdafx.h"
#include "ServerScene.h"
bool startGame = false;

//SceneData �����!
ServerData gameData;
HANDLE Event;
HANDLE TThread;

// ����� ���� ����
std::vector<SOCKET> MatchingQueue;


/////////////////////////////////////////////////////////////////
// ���� ��� �κ�
/////////////////////////////////////////////////////////////////
// ���� �Լ� ���� ��� �� ����
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

// ���� �Լ� ���� ���
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



// Ÿ��Ʋ ������ ����
DWORD WINAPI TitleThread(LPVOID arg)
{   
    int retval;

    // ���� �ʱ�ȭ
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

    //�ʱ� ������ ��� �÷��̾ �غ� �ȵ� �����̹Ƿ� false
    //��, �ƹ� Ű�� �Է� �޾Ƽ� �ȵȴ�.
    bool playerReady = false;
    KeyInput Input{ 0 };

    // ������ ��ſ� ����� ����
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

        // ����� Ŭ���̾�Ʈ�� ������ �������
        printf("\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
            ip_addr, ntohs(clientaddr.sin_port));

        // ����Ǿ����Ƿ� ���� �󿡼� �÷��̾ ���������Ѵ�.
        gameData.CreatePlayer(client_sock);

        ////////////////////////////////////////////////////////////////
        if (MatchingQueue.size() == MAX_PLAYER)
        {
            startGame = true;

            for (int i = 0; i < 3; ++i)
            {
                retval = send(MatchingQueue[i], (char*)&startGame, sizeof(startGame), 0);

                TThread = CreateThread(NULL, 0, TitleThread, (LPVOID)MatchingQueue[i], 0, NULL);
                if (TThread == NULL) { closesocket(client_sock); }
            }          

            ExitThread(0);
        }
        else
        {
            for (int i = 0; i < MatchingQueue.size(); ++i)
            {
                retval = send(MatchingQueue[i], (char*)&startGame, sizeof(startGame), 0);
            }
        }
        //////////////////////////////////////////////////////////////// 

    }
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