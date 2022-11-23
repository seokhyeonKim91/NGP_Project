#pragma once
#include "Renderer.h"
#include "GSEGlobals.h"

class Title
{
public:
	Title();
	~Title();

	void RendererScene();

private:
	Renderer* m_renderer = NULL;
};

