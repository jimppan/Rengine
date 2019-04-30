#ifndef RENGINE_WINDOW_H
#define RENGINE_WINDOW_H

#include "PCH.h"

#include "Rengine/Core.h"
#include "Rengine/Events/Event.h"

namespace Rengine
{
	struct windowprops_t
	{
		windowprops_t(const std::string& title = "Rengine",
                      unsigned int width = 1280,
                      unsigned int height = 720) :
			title(title),
			width(width),
			height(height)
		{

		}

		std::string title;
		unsigned int width;
		unsigned int height;
	};

	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window(){}

		virtual void OnUpdatePre() = 0;
		virtual void OnUpdatePost() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		void SetClearColor(const ColorRGBA32& color);
		inline ColorRGBA32 GetClearColor() const { return m_clrClearColor; }

		static Window* Create(const windowprops_t& props = windowprops_t());
	protected:
		ColorRGBA32 m_clrClearColor = { 0.0f, 0.0f, 0.0f, 0.0f };
	};
}

#endif // RENGINE_C_WINDOW_H