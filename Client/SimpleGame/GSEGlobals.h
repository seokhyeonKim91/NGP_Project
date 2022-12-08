#pragma once
#define MAP_SIZE 30

#define GSE_GRAVITY 9.8f

#define GSE_WINDOW_WIDTH 800
#define GSE_WINDOW_HEIGHT 800
#define BOMB_TIME 30
#define BOMB_MAX 6


enum Item
{
	EMPTY,
	BALLON, //폭탄 개수 증가
	SHOES, //플레이어 이동 속도 증가
	POTION, //폭탄 길이 증가
};

enum PlayerColor
{
	RED,
	BLUE,
	PLAYEREMPTY,
};

struct Point
{
	short x;
	short y;
};

struct MapData
{
	Item 	item; // 아이템이 있는지
	bool	isRock; //막혀있는 곳
	bool 	isBomb; //폭탄이있는지
	bool	isBombFrame;	//폭탄 불길이 있는지
	PlayerColor playerColor;
};

struct KeyInput //키 입력 on off
{
	bool key_UP;
	bool key_Down;
	bool key_Left;
	bool key_Right;
	bool key_Space;
};

struct PlayerStatus  //플레이어의 소켓, 물줄기, 이동속도, 위치, 상태를 저장
{
	KeyInput	key;
	char 		speed; //플레이어 속도
	char 		power; // 폭탄 길이
	Point 		position; // 플레이어 위치
	bool		isAlive; // 생존 여부
};