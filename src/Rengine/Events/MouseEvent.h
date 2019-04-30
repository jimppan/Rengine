#ifndef RENGINE_MOUSE_EVENT_H
#define RENGINE_MOUSE_EVENT_H

#include "Event.h"

namespace Rengine
{
	// Mouse Moved
	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(float x, float y) :
			m_flMouseX(x),
			m_flMouseY(y)
		{

		}

		inline float GetX() const { return m_flMouseX; }
		inline float GetY() const { return m_flMouseY; }

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	private:
		float m_flMouseX, m_flMouseY;
	};

	// Mouse Scrolled
	class MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(float xOffset, float yOffset) :
			m_flOffsetX(xOffset),
			m_flOffsetY(yOffset)
		{

		}

		inline float GetOffsetX() const { return m_flOffsetX; }
		inline float GetOffsetY() const { return m_flOffsetY; }

		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	private:
		float m_flOffsetX, m_flOffsetY;
	};

	// Mouse Button Base
	class MouseButtonEvent : public Event
	{
	public:
		inline int GetMouseButton() const { return m_iButton; }

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	protected:
		MouseButtonEvent(int button) :
			m_iButton(button)
		{

		}

		int m_iButton;
	};

	// Mouse Button Pressed
	class MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(int button) :
			MouseButtonEvent(button)
		{

		}

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	// Mouse Button Released
	class MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(int button) :
			MouseButtonEvent(button)
		{

		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};
}

#endif // RENGINE_MOUSE_EVENT_H