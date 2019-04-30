#ifndef RENGINE_APPLICATION_EVENT_H
#define RENGINE_APPLICATION_EVENT_H

#include "Event.h"

namespace Rengine
{
	// Window Resize
	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(unsigned int width, unsigned int height) :
			m_iWidth(width),
			m_iHeight(height)
		{

		}

		inline unsigned int GetWidth() const { return m_iWidth; }
		inline unsigned int GetHeight() const { return m_iHeight; }

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:
		unsigned int m_iWidth, m_iHeight;
	};

	// Window Close
	class  WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent()
		{

		}

		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};
}

#endif // RENGINE_APPLICATION_EVENT_H