#include "UIBase.h"
#include "UIPositionStore.h"

UIBase::UIBase(const UIKind& kind) :
	m_kind(kind)
{
	// À•Wæ“¾
	m_positionData = UIPositionStore::GetInstance().GetPositionData(kind);
}
