#include "UIBase.h"
#include "UIPositionLoader.h"

UIBase::UIBase(const std::string& className)
{
	m_name = className;

	UIPosData data = UIPositionLoader::GeInstance().GetPosData(className);
	m_pos = data.pos;
	m_scale = data.scale;
}