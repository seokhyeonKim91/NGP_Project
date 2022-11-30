/*
Copyright 2020 Lee Taek Hee (Korea Polytech University)

This program is free software: you can redistribute it and/or modify
it under the terms of the What The Hell License. Do it plz.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY.
*/
#pragma comment(lib, "ws2_32")

#include "stdafx.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <conio.h>
#include <iostream>
#include <gl/glew.h>
#include <gl/freeglut.h>

#include "Title.h"

#define SERVERIP "172.30.1.11"
#define SERVERPORT 9000
#define BUFSIZE    512

GSEGame* g_game = NULL;
KeyInput g_inputs;
Title* g_title = NULL;
WSADATA wsa;
SOCKET sock;
SOCKADDR_IN serveraddr;
char buf[BUFSIZE + 1];
int retval;
int RecvData(SOCKET s, char* buf, int len, int flags);
bool gamestate = false;

int g_prevTimeInMillisecond = 0;
int px = 15; int py = 15;

int bx[BOMB_MAX] = { -1 }; int by[BOMB_MAX] = { -1 };
int bombnum = 0;
int bombcount = 1;
int bombtime[BOMB_MAX] = { 0 };

MapData mapData[MAP_SIZE][MAP_SIZE];


bool collision(void)
{
	if (mapData[px][py].isRock == true)
	{
		return true;
	}

	return false;
}

void MakeBomb(void)
{
	mapData[bx[bombnum]][by[bombnum]].isBomb = true;
	bombnum++;
}

void Idle(void)
{
}

void MouseInput(int button, int state, int x, int y)
{
}

void KeyDownInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case ' ':
		g_inputs.key_Space = true;
		bx[bombcount] = px;
		by[bombcount] = py;

		MakeBomb();

		break;
	}
}

void KeyUpInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case ' ':
		g_inputs.key_Space = false;
		break;
	}
}

void SpecialKeyDownInput(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		g_inputs.key_UP = true;
		py++;
		if (collision()) py--;

		break;
	case GLUT_KEY_DOWN:
		g_inputs.key_Down = true;
		py--;
		if (collision()) py++;

		break;
	case GLUT_KEY_LEFT:
		g_inputs.key_Left = true;
		px--;
		if (collision()) px++;

		break;
	case GLUT_KEY_RIGHT:
		g_inputs.key_Right = true;
		px++;
		if (collision()) px--;

		break;
	}

}

void SpecialKeyUpInput(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		g_inputs.key_UP = false;
		break;
	case GLUT_KEY_DOWN:
		g_inputs.key_Down = false;
		break;
	case GLUT_KEY_LEFT:
		g_inputs.key_Left = false;
		break;
	case GLUT_KEY_RIGHT:
		g_inputs.key_Right = false;
		break;
	}
}

void err_quit(char* msg)
{
}

void err_display(char* msg)
{
}


int SendServer(void)
{
	retval = send(sock, (const char*)(&g_inputs), sizeof(g_inputs), 0);
	if (retval == SOCKET_ERROR) 
	{
		err_display("");
	}
}

int RecvClient(void)
{
	retval = recv(sock, (char*)(&mapData), sizeof(mapData), 0);
	if (retval == SOCKET_ERROR) 
	{
		err_display("");
	}
	g_game->SetMapData(mapData);
	g_game->RendererGameScene();
}

/*int RecvnData(SOCKET s, char* buf, int len, int flags)
{
	int data;
	char* ptr = buf;
	int left = len;

	while (left > 0) 
	{
	}

	return ();
}*/

void RenderScene(int temp)
{
	int currentTime = glutGet(GLUT_ELAPSED_TIME);
	int elapsedTime = currentTime - g_prevTimeInMillisecond;
	g_prevTimeInMillisecond = currentTime;
	float elapsedTimeInSec = (float)elapsedTime; // 1000.0f;

	if (gamestate == true)
	{
		SendServer();
		RecvClient();
	}
	else
	{
		RecvClient();
		g_title->RendererScene();
	}
	g_game->SetMapData(mapData);
	g_game->RendererGameScene();

	glutSwapBuffers();		//double buffering

	glutTimerFunc(60, RenderScene, 60);
}

int main(int argc, char** argv)
{
	// Initialize GL things
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(GSE_WINDOW_WIDTH, GSE_WINDOW_HEIGHT);
	glutCreateWindow("Net Game");


	// Initialize Renderer
	glewInit();
	if (glewIsSupported("GL_VERSION_3_0"))
	{
		std::cout << " GLEW Version is 3.0\n ";
	}
	else
	{
		std::cout << "GLEW 3.0 not supported\n ";
	}

	//윈속 초기화
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;
	//소켓 생성, 서버 연결
	// 소켓 생성
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("");

	// 서버 연결()
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	inet_pton(AF_INET, SERVERIP, &(serveraddr.sin_addr.s_addr));
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR)
	{
		err_quit("");
	}

	g_game = new GSEGame();
	g_title = new Title();
	memset(&g_inputs, 0, sizeof(KeyInput));

	glutDisplayFunc(Idle);
	glutIdleFunc(Idle);
	glutKeyboardFunc(KeyDownInput);
	glutKeyboardUpFunc(KeyUpInput);
	glutMouseFunc(MouseInput);
	glutSpecialFunc(SpecialKeyDownInput);
	glutSpecialUpFunc(SpecialKeyUpInput);

	g_prevTimeInMillisecond = glutGet(GLUT_ELAPSED_TIME);

	glutTimerFunc(50, RenderScene, 50);

	glutMainLoop();

	delete g_game;

	// 서버 연결 종료, 윈속 종료
	closesocket(sock);
	WSACleanup();

	return 0;
}

