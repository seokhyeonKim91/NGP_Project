#pragma once
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <WS2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <chrono>
#include <queue>

#define MAX_PLAYER 2
#define MAP_SIZE 30
#define SERVERPORT 9000

#define GSE_GRAVITY 9.8f

#define GSE_WINDOW_WIDTH 800
#define GSE_WINDOW_HEIGHT 800
#define BOMB_TIME 30

struct KeyInput //Ű �Է� on off
{
	bool key_Up;
	bool key_Down;
	bool key_Left;
	bool key_Right;
	bool key_Space;
};

struct Point
{
	int X;
	int Y;
};

struct BombData		//��ź Ÿ�̸� �� ��ź ��ġ�� �����
{
	Point bombPoint;											//��ġ
	std::chrono::system_clock::time_point bombCountdown;		//Ÿ�̸�
	int bombExplosionLength;									//�Ҳ� ����
	int playerID;
	bool up = true;
	bool down = true;
	bool right = true;
	bool left = true;
	BombData(int px, int py, std::chrono::system_clock::time_point count, int length, int id) { bombPoint.X = px; bombPoint.Y = py; bombCountdown = count; bombExplosionLength = length; playerID = id; }
};

enum PlayerColor
{
	RED,
	BLUE,	
	PLAYEREMPTY,
};

struct PlayerStatus  //�÷��̾��� ����, ���ٱ�, �̵��ӵ�, ��ġ, ���¸� ����
{
	SOCKET					playerSocket;
	KeyInput				playerKeyInput;
	Point 					playerPosition;			// �÷��̾� ��ġ
	int						playerBombLength;		// ��ź �ұ� ����
	int 					playerBombCount;		// ��ź ����
	PlayerColor				playerColor;			// Ŭ�󿡼� �׸������� ����
	bool					isAlive;				// ���� ����
	bool					isReady;				// �غ� ����
};

struct MapData
{
	bool					isRock; //�����ִ� ��
	bool 					isBomb; //��ź���ִ���
	bool					isBombFrame;	//��ź �ұ��� �ִ���
	PlayerColor				playerColor;
};
