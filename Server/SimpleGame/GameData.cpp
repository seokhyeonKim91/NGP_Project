#include "stdafx.h"
#include "ServerScene.h"

// 맵 초기화
ServerData::ServerData()
{
	for (int i = 0; i < MAP_SIZE; i++)		//세로
	{
		for (int j = 0; j < MAP_SIZE; j++)	//가로
		{
			m_mapData[i][j].isBomb = false;
			if (i == 0 || i == MAP_SIZE - 1 || j == 0 || j == MAP_SIZE - 1)
				m_mapData[i][j].isRock = true;
			else if (i % 3 == 2 && j % 3 == 2)
				m_mapData[i][j].isRock = true;
			else
				m_mapData[i][j].isRock = false;

			if ((m_mapData[i][j].isRock == false) && (i % 6 == 2 && j % 5 == 1)) //아이템두기
				m_mapData[i][j].item = Item::POTION;
			else if ((m_mapData[i][j].isRock == false) && (i % 8 == 3 && j % 6 == 3))
				m_mapData[i][j].item = Item::BALLON;
			else
				m_mapData[i][j].item = Item::EMPTY;
			m_mapData[i][j].playerColor = PlayerColor::PLAYEREMPTY;
		}
	}
}

ServerData::~ServerData()
{

}

void ServerData::Update()
{
	///////////////////////
	// 업데이트 파트 추가
	///////////////////////
}

void ServerData::SetKeyInput(SOCKET socket, KeyInput key)
{
	for (int i = 0; i < MAX_PLAYER; i++)
	{
		if (m_players[i].playerSocket == socket)
		{
			m_players[i].playerKeyInput = key;
			break;
		}
	}
}

void ServerData::CreatePlayer(SOCKET socket)
{
	m_players[m_nPlayer].playerSocket = socket;
	m_players[m_nPlayer].isAlive = true;
	m_players[m_nPlayer].playerPosition = { (m_nPlayer * 6) + 1, (m_nPlayer * 6) + 1 };
	m_players[m_nPlayer].playerBombCount = 1;
	m_players[m_nPlayer].playerBombLength = 1;
	m_players[m_nPlayer].playerColor = PlayerColor(m_nPlayer);
	++m_nPlayer;
}