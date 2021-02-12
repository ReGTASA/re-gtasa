#pragma once

class CVector
{
public:
	CVector();
	~CVector();

public:
	float x, y, z;
};

VALIDATE_SIZE(CVector, 0xC);