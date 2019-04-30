#ifndef RENGINE_WINDOWS_WINDOW_H
#define RENGINE_WINDOWS_WINDOW_H

#include "Rengine/Window.h"

#include <GLFW/glfw3.h>

namespace Rengine
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const windowprops_t& props);
		virtual ~WindowsWindow();

		virtual void OnUpdatePre() override;
		virtual void OnUpdatePost() override;

		virtual inline unsigned int GetWidth() const override { return m_Data.width; }
		virtual inline unsigned int GetHeight() const override { return m_Data.height; }

		// Window attributes
		virtual inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.event_callback = callback; }
		virtual void SetVSync(bool enabled) override;
		virtual bool IsVSync() const override;

		inline virtual void* GetNativeWindow() const override { return m_pWindow; }
	private:
		virtual void Init(const windowprops_t& props);
		virtual void Shutdown();
	private:
		struct windowdata_t
		{
			std::string title;
			unsigned int width;
			unsigned int height;
			bool vsync;

			EventCallbackFn event_callback;
		};

		GLFWwindow* m_pWindow;
		windowdata_t m_Data;
	};
}

#endif // RENGINE_WINDOWS_WINDOW_H