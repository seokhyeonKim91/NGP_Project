#include "stdafx.h"
#include "ServerScene.h"

int X1 = 1, Y1 = 1;
int X2 = 10, Y2 = 1;

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
	for (int i = 0; i < MAX_PLAYER; i++) 
	{		
		if (m_players[i].isAlive)
		{
			m_mapData[m_players[i].playerPosition.X][m_players[i].playerPosition.Y].playerColor = PlayerColor::PLAYEREMPTY;

			if (m_players[i].playerKeyInput.key_Down) //아래키 처리
			{
				if (m_mapData[m_players[i].playerPosition.X][m_players[i].playerPosition.Y - 1].isRock == false)
				{
					m_players[i].playerPosition.Y -= 1;
				}
				
				m_players[i].playerKeyInput.key_Down = false;
			}
			if (m_players[i].playerKeyInput.key_Up) //위키
			{
				if (m_mapData[m_players[i].playerPosition.X][m_players[i].playerPosition.Y + 1].isRock == false)
				{
					m_players[i].playerPosition.Y += 1;
				}
				
				m_players[i].playerKeyInput.key_Up = false;
			}
			if (m_players[i].playerKeyInput.key_Left) // 왼쪽
			{
				if (m_mapData[m_players[i].playerPosition.X - 1][m_players[i].playerPosition.Y].isRock == false)
				{
					m_players[i].playerPosition.X -= 1;
				}
				
				m_players[i].playerKeyInput.key_Left = false;
			}
			if (m_players[i].playerKeyInput.key_Right)
			{
				if (m_mapData[m_players[i].playerPosition.X + 1][m_players[i].playerPosition.Y].isRock == false)
				{
					m_players[i].playerPosition.X += 1;
				}
				
				m_players[i].playerKeyInput.key_Right = false;
			}
			if (m_players[i].playerKeyInput.key_Space)				//폭탄 처리
			{
				if (m_players[i].playerBombCount > 0)
				{
					if (m_mapData[m_players[i].playerPosition.X][m_players[i].playerPosition.Y].isBomb == false)
					{
						m_bombManger.emplace_back(m_players[i].playerPosition.X, m_players[i].playerPosition.Y, std::chrono::system_clock::now(), m_players[i].playerBombLength, i);
						m_mapData[m_players[i].playerPosition.X][m_players[i].playerPosition.Y].isBomb = true;
						m_players[i].playerBombCount--;
					}
				}
			}
			m_mapData[m_players[i].playerPosition.X][m_players[i].playerPosition.Y].playerColor = PlayerColor(i);
		}
	}

	if (!m_bombManger.empty())
	{
		int nbomb = m_bombManger.size();
		for (int k = 0; k < nbomb; k++)		//플레이어 길이로 처리하기
		{
			if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - m_bombManger[k].bombCountdown) >= std::chrono::seconds(2)
				&& std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - m_bombManger[k].bombCountdown) < std::chrono::seconds(3))
			{
				for (int l = 1; l < m_bombManger[k].bombExplosionLength + 1; l++)
				{
					if (!m_mapData[m_bombManger[k].bombPoint.X - l][m_bombManger[k].bombPoint.Y].isRock && !(m_bombManger[k].bombPoint.X - l < 0) && m_bombManger[k].left)
					{
						m_mapData[m_bombManger[k].bombPoint.X - l][m_bombManger[k].bombPoint.Y].isBombFrame = true;
					}
					else
					{
						m_bombManger[k].left = false;
					}
					if (!m_mapData[m_bombManger[k].bombPoint.X + l][m_bombManger[k].bombPoint.Y].isRock && !(m_bombManger[k].bombPoint.X + l > MAP_SIZE) && m_bombManger[k].right)
					{
						m_mapData[m_bombManger[k].bombPoint.X + l][m_bombManger[k].bombPoint.Y].isBombFrame = true;
					}
					else
					{
						m_bombManger[k].right = false;
					}
					if (!m_mapData[m_bombManger[k].bombPoint.X][m_bombManger[k].bombPoint.Y - l].isRock && !(m_bombManger[k].bombPoint.Y - l < 0) && m_bombManger[k].down)
					{
						m_mapData[m_bombManger[k].bombPoint.X][m_bombManger[k].bombPoint.Y - l].isBombFrame = true;
					}
					else
					{
						m_bombManger[k].down = false;
					}
					if (!m_mapData[m_bombManger[k].bombPoint.X][m_bombManger[k].bombPoint.Y + l].isRock && !(m_bombManger[k].bombPoint.Y + l > MAP_SIZE) && m_bombManger[k].up)
					{
						m_mapData[m_bombManger[k].bombPoint.X][m_bombManger[k].bombPoint.Y + l].isBombFrame = true;
					}
					else
					{
						m_bombManger[k].up = false;
					}
					m_mapData[m_bombManger[k].bombPoint.X][m_bombManger[k].bombPoint.Y].isBombFrame = true;//폭탄이 있던 곳
					m_mapData[m_bombManger[k].bombPoint.X][m_bombManger[k].bombPoint.Y].isBomb = false;
				}
			}
			else if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - m_bombManger[k].bombCountdown) >= std::chrono::seconds(3))
			{
				for (int l = 1; l < m_bombManger[k].bombExplosionLength + 1; l++)
				{
					if (!m_mapData[m_bombManger[k].bombPoint.X - l][m_bombManger[k].bombPoint.Y].isRock && !(m_bombManger[k].bombPoint.X - l < 0))
					{
						m_mapData[m_bombManger[k].bombPoint.X - l][m_bombManger[k].bombPoint.Y].isBombFrame = false;
					}
					if (!m_mapData[m_bombManger[k].bombPoint.X + l][m_bombManger[k].bombPoint.Y].isRock && !(m_bombManger[k].bombPoint.X + l > MAP_SIZE))
					{
						m_mapData[m_bombManger[k].bombPoint.X + l][m_bombManger[k].bombPoint.Y].isBombFrame = false;
					}
					if (!m_mapData[m_bombManger[k].bombPoint.X][m_bombManger[k].bombPoint.Y - l].isRock && !(m_bombManger[k].bombPoint.Y - l < 0))
					{
						m_mapData[m_bombManger[k].bombPoint.X][m_bombManger[k].bombPoint.Y - l].isBombFrame = false;
					}
					if (!m_mapData[m_bombManger[k].bombPoint.X][m_bombManger[k].bombPoint.Y + l].isRock && !(m_bombManger[k].bombPoint.Y + l > MAP_SIZE))
					{
						m_mapData[m_bombManger[k].bombPoint.X][m_bombManger[k].bombPoint.Y + l].isBombFrame = false;
					}
					m_mapData[m_bombManger[k].bombPoint.X][m_bombManger[k].bombPoint.Y].isBombFrame = false;//폭탄이 있던 곳

				}
				m_players[m_bombManger[k].playerID].playerBombCount++;
				m_bombManger.erase(m_bombManger.begin() + k);
				k--; nbomb--;
			}
		}
	}

	for (int i = 0; i < MAX_PLAYER; i++)
	{
		if (m_mapData[m_players[i].playerPosition.X][m_players[i].playerPosition.Y].isBombFrame)
		{
			m_players[i].isAlive = false;
			m_mapData[m_players[i].playerPosition.X][m_players[i].playerPosition.Y].playerColor = PlayerColor::PLAYEREMPTY;
		}
	}
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

	if (m_nPlayer == 0)
	{
		m_players[0].playerPosition.X = X1;
		m_players[0].playerPosition.Y = Y1;
	}

	else
	{
		m_players[1].playerPosition.X = X2;
		m_players[1].playerPosition.Y = Y2;
	}

	m_players[m_nPlayer].playerBombCount = 3;
	m_players[m_nPlayer].playerBombLength = 3;
	m_players[m_nPlayer].playerColor = PlayerColor(m_nPlayer);

	++m_nPlayer;
}