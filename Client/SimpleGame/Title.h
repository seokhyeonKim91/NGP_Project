#pragma once
#include "Renderer.h"


class Title
{
public:
	Title();
	~Title();

	void RendererScene();

private:
	Renderer* m_renderer = NULL;
};

