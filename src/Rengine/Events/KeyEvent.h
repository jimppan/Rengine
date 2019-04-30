#ifndef RENGINE_KEY_EVENT_H
#define RENGINE_KEY_EVENT_H

#include "Event.h"
#include "Rengine/Input.h"

namespace Rengine
{
	// Key Event
	class KeyEvent : public Event
	{
	public:
		inline KeyCode GetKeyCode() { return m_iKeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		KeyEvent(KeyCode keycode):
			m_iKeyCode(keycode)
		{

		}

		KeyCode m_iKeyCode;
	};

	// Key Pressed Event
	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(KeyCode keycode, int repeatCount) :
			KeyEvent(keycode), 
			m_iRepeatCount(repeatCount)
		{

		}

		inline int GetRepeatCount() const { return m_iRepeatCount; }

		EVENT_CLASS_TYPE(KeyPressed)
	private:
		int m_iRepeatCount;
	};

	// Key Released Event
	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(KeyCode keycode) :
			KeyEvent(keycode)
		{

		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	// Key Typed Event
	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(KeyCode keycode) :
			KeyEvent(keycode)
		{

		}

		EVENT_CLASS_TYPE(KeyTyped)
	};
}

#endif // RENGINE_KEY_EVENT_H