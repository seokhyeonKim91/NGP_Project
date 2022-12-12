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

struct KeyInput //키 입력 on off
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

struct BombData		//폭탄 타이머 및 폭탄 위치값 저장용
{
	Point bombPoint;											//위치
	std::chrono::system_clock::time_point bombCountdown;		//타이머
	int bombExplosionLength;									//불꽃 길이
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

struct PlayerStatus  //플레이어의 소켓, 물줄기, 이동속도, 위치, 상태를 저장
{
	SOCKET					playerSocket;
	KeyInput				playerKeyInput;
	Point 					playerPosition;			// 플레이어 위치
	int						playerBombLength;		// 폭탄 불길 길이
	int 					playerBombCount;		// 폭탄 개수
	PlayerColor				playerColor;			// 클라에서 그리기위한 색상
	bool					isAlive;				// 생존 여부
	bool					isReady;				// 준비 여부
};

struct MapData
{
	bool					isRock; //막혀있는 곳
	bool 					isBomb; //폭탄이있는지
	bool					isBombFrame;	//폭탄 불길이 있는지
	PlayerColor				playerColor;
};
