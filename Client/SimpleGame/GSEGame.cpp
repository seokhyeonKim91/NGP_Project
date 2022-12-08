#include "stdafx.h"
#include "Title.h"

GSEGame::GSEGame()
{
	//Renderer 생성
	m_renderer = new Renderer(GSE_WINDOW_WIDTH, GSE_WINDOW_HEIGHT);

	for (int i = 0; i < MAP_SIZE; i++)
	{
		for (int j = 0; j < MAP_SIZE; j++)
		{
			m_pMapdata[i][j].isBomb = false;
			m_pMapdata[i][j].isRock = false;
			m_pMapdata[i][j].item = Item::EMPTY;
			m_pMapdata[i][j].playerColor = PlayerColor::PLAYEREMPTY;
		}
	}
}

GSEGame::~GSEGame()
{
	//Renderer 삭제
}

void GSEGame::RendererGameScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.25f, 0.25f, 0.25f, 1.0f);


	//Draw all objects
	for (int i = 0; i < MAP_SIZE; i++)
	{
		for (int j = 0; j < MAP_SIZE; j++)
		{

			if (m_pMapdata[i][j].isBomb == true)		//폭탄이 있을 경우
			{
				m_renderer->DrawSolidRect((i - MAP_SIZE / 2) * 20, (j - MAP_SIZE / 2) * 20, 0, 20, 0.4f, 0.4f, 0.4f, 1.0f);		//어두운 회색
			}
			if (m_pMapdata[i][j].isRock == true)		//벽이 있을 경우
			{
				m_renderer->DrawSolidRect((i - MAP_SIZE / 2) * 20, (j - MAP_SIZE / 2) * 20, 0, 20, 0.8f, 0.8f, 0.8f, 1.0f);		//밝은 회색
			}
			if (m_pMapdata[i][j].isBombFrame == true)		//벽이 있을 경우
			{
				m_renderer->DrawSolidRect((i - MAP_SIZE / 2) * 20, (j - MAP_SIZE / 2) * 20, 0, 20, 0.5f, 0.3f, 0.0f, 1.0f);		//
			}
			if (m_pMapdata[i][j].item != Item::EMPTY)	//아이템이 있을 경우
			{
				switch (m_pMapdata[i][j].item)
				{
				case Item::BALLON:
					m_renderer->DrawSolidRect((i - MAP_SIZE / 2) * 20, (j - MAP_SIZE / 2) * 20, 0, 20, 0.0f, 1.0f, 1.0f, 1.0f);	//하늘색
					break;
				}
			}
			if (m_pMapdata[i][j].playerColor != PlayerColor::PLAYEREMPTY)
			{
				switch (m_pMapdata[i][j].playerColor)
				{
				case PlayerColor::RED:
					m_renderer->DrawSolidRect((i - MAP_SIZE / 2) * 20, (j - MAP_SIZE / 2) * 20, 0, 16, 1.0f, 0.0f, 0.0f, 1.0f);	//빨강
					break;
				case PlayerColor::BLUE:
					m_renderer->DrawSolidRect((i - MAP_SIZE / 2) * 20, (j - MAP_SIZE / 2) * 20, 0, 16, 0.0f, 0.0f, 1.0f, 1.0f);	//파랑
					break;
				}
			}


		}
	}
}


void GSEGame::SetMapData(MapData(*map_data)[MAP_SIZE])
{
	for (int i = 0; i < MAP_SIZE; i++)
	{
		for (int j = 0; j < MAP_SIZE; j++)
		{
			m_pMapdata[i][j].isBomb = map_data[i][j].isBomb;
			m_pMapdata[i][j].isRock = map_data[i][j].isRock;
			m_pMapdata[i][j].item = map_data[i][j].item;
			m_pMapdata[i][j].isBombFrame = map_data[i][j].isBombFrame;
			m_pMapdata[i][j].playerColor = map_data[i][j].playerColor;
		}
	}
}
