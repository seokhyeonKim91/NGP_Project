#include "stdafx.h"
#include "Title.h"

Title::Title()
{
	m_renderer = new Renderer(GSE_WINDOW_WIDTH, GSE_WINDOW_HEIGHT);
}

Title::~Title()
{
}

void Title::RendererScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}
