#include "stdafx.h"
#include "GSEObject.h"

GSEObject::GSEObject()
{
	m_positionX = -1000000;
	m_positionY = -1000000;
	m_Depth = -1000000;
	m_SizeX = -1000000;
	m_SizeY = -1000000;
	m_AccX = -1000000;
	m_AccY = -1000000;
	m_VelX = -1000000;
	m_VelY = -1000000;
	m_Mass = -1000000;
}

GSEObject::~GSEObject()
{
}

void GSEObject::Update(float elapsedTimeInSec, GSEUpdateParams* param)
{
	//Update position
	//pos = prevPosition + Vel * elapsedTime
	//m_positionX = m_positionX + (m_VelX * elapsedTimeInSec);
	//m_positionY = m_positionY + (m_VelY * elapsedTimeInSec);
	float t = elapsedTimeInSec;
	float tt = elapsedTimeInSec * elapsedTimeInSec;

	//calc temporary
	float accX = param->forceX / m_Mass;
	float accY = param->forceY / m_Mass;

	//sum with object'acc
	accX += m_AccX;
	accY += m_AccY;

	//update position
	m_positionX = m_positionX + m_VelX * t + 0.5f * accX * tt;
	m_positionY = m_positionY + m_VelY * t + 0.5f * accY * tt;

	//update velocity
	m_VelX = m_VelX + accX * t;
	m_VelY = m_VelY + accY * t;
}

void GSEObject::SetPosition(float x, float y, float depth)
{
	m_positionX = x;
	m_positionY = y;
	m_Depth = depth;
}

void GSEObject::SetSize(float sx, float sy)
{
	m_SizeX = sx;
	m_SizeY = sy;
}

void GSEObject::SetVel(float sx, float sy)
{
	m_VelX = sx;
	m_VelY = sy;
}

void GSEObject::SetAcc(float sx, float sy)
{
	m_AccX = sx;
	m_AccY = sy;
}

void GSEObject::SetMass(float sx)
{
	m_Mass = sx;
}

void GSEObject::GetPosition(float* x, float* y, float* depth)
{
	*x = m_positionX;
	*y = m_positionY;
	*depth = m_Depth;
}

void GSEObject::GetSize(float* sx, float* sy)
{
	*sx = m_SizeX;
	*sy = m_SizeY;
}

void GSEObject::GetVel(float* sx, float* sy)
{
	*sx = m_VelX;
	*sy = m_VelY;
}

void GSEObject::GetAcc(float* sx, float* sy)
{
	*sx = m_AccX;
	*sy = m_AccY;
}

void GSEObject::GetMass(float* sx)
{
	*sx = m_Mass;
}
