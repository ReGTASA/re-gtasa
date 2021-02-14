#include "../utils/patcher.h"
#include "CSimpleTransform.h"

// DONE
CSimpleTransform::CSimpleTransform()
{
	m_fHeading = 0.0f;
}

// DONE
CSimpleTransform::~CSimpleTransform() {}


// DONE
void CSimpleTransform::Invert(const CSimpleTransform& base)
{
	double c = cos(base.m_fHeading);
	double s = sin(base.m_fHeading);

	m_vPosn.x = -(c * base.m_vPosn.x) - s * base.m_vPosn.y;
	m_vPosn.y = s * base.m_vPosn.x - c * base.m_vPosn.y;
	m_vPosn.z = -base.m_vPosn.z;
	m_fHeading = -base.m_fHeading;
}

STARTPATCHES
InjectHook(0x54EF90, &CSimpleTransform::Invert, PATCH_JUMP);
ENDPATCHES