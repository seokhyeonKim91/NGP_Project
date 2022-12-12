#pragma once
#include "ServerData.h"

// 게임에 들어갈 게임 정보
class ServerData
{
public:
	ServerData();
	~ServerData();

private:
	PlayerStatus					m_players[MAX_PLAYER];
	int						m_nPlayer = 0;
	std::vector<BombData>	m_bombManger;
	MapData					m_mapData[MAP_SIZE][MAP_SIZE];
public:
	void Update();
	void SetKeyInput(SOCKET socket, KeyInput key);
	MapData GetMapData(int n, int m) { return m_mapData[n][m]; };
	void CreatePlayer(SOCKET socket);
};