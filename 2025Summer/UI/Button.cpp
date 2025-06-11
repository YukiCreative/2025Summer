#include "Button.h"

Button::Button() :
	m_pos(),
	m_updateState(&Button::NormalUpdate),
	m_downButton(),
	m_leftButton(),
	m_rightButton(),
	m_upButton()
{
}

void Button::Init(const Vector2& initPos)
{
	m_pos = initPos;

	m_upButton    = weak_from_this();
	m_downButton  = weak_from_this();
	m_leftButton  = weak_from_this();
	m_rightButton = weak_from_this();
	m_upButton    = weak_from_this();
}

void Button::Update()
{
	// ‚¢‚Â‚à‚Ì
	(this->*m_updateState)();
}

std::weak_ptr<Button> Button::RightButton() const
{
	return m_rightButton;
}

std::weak_ptr<Button> Button::LeftButton() const
{
	return m_leftButton;
}

std::weak_ptr<Button> Button::UpButton() const
{
	return m_upButton;
}

std::weak_ptr<Button> Button::DownButton() const
{
	return m_downButton;
}

void Button::InvertState()
{
	if (m_updateState == &Button::FocusedUpdate)
	{
		m_updateState = &Button::NormalUpdate;
	}
	else
	{
		m_updateState = &Button::FocusedUpdate;
	}
}