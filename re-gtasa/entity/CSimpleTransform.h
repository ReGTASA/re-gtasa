#pragma once

#include "../math/CVector.h"

class CSimpleTransform
{
public:
	CSimpleTransform();
	~CSimpleTransform();

	void Invert(const CSimpleTransform& base);

public:
	CVector m_vPosn;
	float m_fHeading;
};

VALIDATE_SIZE(CSimpleTransform, 0x10);