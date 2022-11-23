#pragma once

#include "GSEGlobal.h"

class GSEObject
{
public:
	GSEObject();
	~GSEObject();

	void Update(float elapsedTimeInSec, GSEUpdateParams* param);

	void SetPosition(float x, float y, float depth);
	void SetSize(float sx, float sy);

	void SetVel(float sx, float sy);
	void SetAcc(float sx, float sy);
	void SetMass(float sx);
	
	void GetPosition(float* x, float* y, float * depth);
	void GetSize(float* sx, float* sy);
	
	void GetVel(float* sx, float* sy);
	void GetAcc(float* sx, float *sy);
	void GetMass(float* sx);


private:

	float m_positionX, m_positionY;
	float m_Depth;
	float m_SizeX, m_SizeY;
	float m_VelX, m_VelY;
	float m_AccX, m_AccY;
	float m_Mass;
};

