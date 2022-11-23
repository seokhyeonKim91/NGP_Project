/*
Copyright 2020 Lee Taek Hee (Korea Polytech University)

This program is free software: you can redistribute it and/or modify
it under the terms of the What The Hell License. Do it plz.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY.
*/

#include "stdafx.h"
#include <iostream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include "GSEGame.h"
#include "GSEGlobals.h"

GSEGame* g_game = NULL;
KeyInput g_inputs;

int g_prevTimeInMillisecond = 0;
int px = 15; int py = 15;

int bx[BOMB_MAX] = { -1 }; int by[BOMB_MAX] = { -1 };
int bombnum = 0;
int bombcount = 1;
int bombtime[BOMB_MAX] = { 0 };

MapData mapData[MAP_SIZE][MAP_SIZE];

void RenderScene(int temp)
{
	int currentTime = glutGet(GLUT_ELAPSED_TIME);
	int elapsedTime = currentTime - g_prevTimeInMillisecond;
	g_prevTimeInMillisecond = currentTime;
	float elapsedTimeInSec = (float)elapsedTime; // 1000.0f;


	for (int i = 0; i < MAP_SIZE; i++)		//세로
	{
		for (int j = 0; j < MAP_SIZE; j++)	//가로
		{
			mapData[i][j].isBomb = false;
			mapData[i][j].isBombFrame = false;
			if (i == 0 || i == MAP_SIZE - 1 || j == 0 || j == MAP_SIZE - 1)
				mapData[i][j].isRock = true;
			else if (i % 3 == 2 && j % 3 == 2)
				mapData[i][j].isRock = true;
			else
				mapData[i][j].isRock = false;

			if ((mapData[i][j].isRock == false) && (i % 6 == 2 && j % 5 == 1)) //아이템두기
				mapData[i][j].item = Item::EMPTY;
			else if ((mapData[i][j].isRock == false) && (i % 8 == 3 && j % 6 == 3))
				mapData[i][j].item = Item::EMPTY;
			else
				mapData[i][j].item = Item::EMPTY;
			mapData[i][j].playerColor = PlayerColor::PLAYEREMPTY;
		}
	}

	mapData[px][py].playerColor = PlayerColor::RED;

	for (int i = 0; i < BOMB_MAX; i++)
	{
		if (bx[i] >= 0 && by[i] >= 0)
		{
			mapData[bx[i]][by[i]].isBomb = true;
			bombtime[i]++;

			if (bombtime[i] == BOMB_TIME)
			{
				bombtime[i] = 0;

				mapData[bx[i]][by[i]].isBomb = false;

				bx[i] = -1;
				by[i] = -1;
			}
		}
	}


	g_game->SetMapData(mapData);
	g_game->RendererGameScene();

	glutSwapBuffers();		//double buffering

	glutTimerFunc(60, RenderScene, 60);
}

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

int main(int argc, char** argv)
{
	// Initialize GL things
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(GSE_WINDOW_WIDTH, GSE_WINDOW_HEIGHT);
	glutCreateWindow("Game Software Engineering KPU");

	glewInit();
	if (glewIsSupported("GL_VERSION_3_0"))
	{
		std::cout << " GLEW Version is 3.0\n ";
	}
	else
	{
		std::cout << "GLEW 3.0 not supported\n ";
	}

	// Initialize Renderer

	g_game = new GSEGame();
	memset(&g_inputs, 0, sizeof(KeyInput));

	glutDisplayFunc(Idle);
	glutIdleFunc(Idle);
	glutKeyboardFunc(KeyDownInput);
	glutKeyboardUpFunc(KeyUpInput);
	glutMouseFunc(MouseInput);
	glutSpecialFunc(SpecialKeyDownInput);
	glutSpecialUpFunc(SpecialKeyUpInput);

	g_prevTimeInMillisecond = glutGet(GLUT_ELAPSED_TIME);

	glutTimerFunc(16, RenderScene, 16);

	glutMainLoop();

	delete g_game;

	return 0;
}

