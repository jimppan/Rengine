#include "PCH.h"

#include "WindowsWindow.h"

#include "Rengine/Events/ApplicationEvent.h"
#include "Rengine/Events/MouseEvent.h"
#include "Rengine/Events/KeyEvent.h"

#include "Rengine/Render/RenderAPI/RAPI.h"
#include "Rengine/Input.h"

namespace Rengine {

	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description)
	{
		RENGINE_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	Window* Window::Create(const windowprops_t& props)
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const windowprops_t& props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::Init(const windowprops_t& props)
	{
		m_Data.title = props.title;
		m_Data.width = props.width;
		m_Data.height = props.height;

		RENGINE_CORE_INFO("Creating window {0} ({1}, {2})", props.title, props.width, props.height);

		if (!s_GLFWInitialized)
		{
			// TODO: glfwTerminate on system shutdown
			int success = glfwInit();
			RENGINE_CORE_ASSERT(success, "Could not intialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);

			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

			s_GLFWInitialized = true;
		}

		m_pWindow = glfwCreateWindow((int)props.width, (int)props.height, m_Data.title.c_str(), nullptr, nullptr);
		RENGINE_CORE_ASSERT(m_pWindow, "Failed to create WindowsWindow!");
		glfwMakeContextCurrent(m_pWindow);
		
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		RENGINE_CORE_ASSERT(status, "Failed to initialize Glad!");

		glfwSetWindowUserPointer(m_pWindow, &m_Data);
		 SetVSync(false);

		glViewport(0, 0, (int)props.width, (int)props.height);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//glEnable(GL_DEPTH_TEST);
		//glDepthFunc(GL_LESS);

		// Set GLFW callbacks
		glfwSetWindowSizeCallback(m_pWindow, [](GLFWwindow* window, int width, int height)
		{
			windowdata_t& data = *(windowdata_t*)glfwGetWindowUserPointer(window);
			data.width = width;
			data.height = height;

			WindowResizeEvent event(width, height);
			data.event_callback(event);
		});

		glfwSetWindowCloseCallback(m_pWindow, [](GLFWwindow* window)
		{
			windowdata_t& data = *(windowdata_t*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.event_callback(event);
		});

		glfwSetKeyCallback(m_pWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			windowdata_t& data = *(windowdata_t*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent event((KeyCode)key, 0);
					data.event_callback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event((KeyCode)key);
					data.event_callback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event((KeyCode)key, 1);
					data.event_callback(event);
					break;
				}
			}
		});

		glfwSetCharCallback(m_pWindow, [](GLFWwindow* window, unsigned int keycode)
		{
			windowdata_t& data = *(windowdata_t*)glfwGetWindowUserPointer(window);

			KeyTypedEvent event((KeyCode)keycode);
			data.event_callback(event);
		});
		
		glfwSetMouseButtonCallback(m_pWindow, [](GLFWwindow* window, int button, int action, int mods)
		{
			windowdata_t& data = *(windowdata_t*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					data.event_callback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					data.event_callback(event);
					break;
				}
			}
		});

		glfwSetScrollCallback(m_pWindow, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			windowdata_t& data = *(windowdata_t*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.event_callback(event);
		});

		glfwSetCursorPosCallback(m_pWindow, [](GLFWwindow* window, double xPos, double yPos)
		{
			windowdata_t& data = *(windowdata_t*)glfwGetWindowUserPointer(window);

			// invert mouse position, so bottom is 0, and top is window size
			MouseMovedEvent event((float)xPos, glm::abs(((float)yPos - data.height) - 1));
			data.event_callback(event);
		});
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_pWindow);
	}

	void WindowsWindow::OnUpdatePre()
	{
		Rengine::RAPI::RAPI::ClearWindow(Rengine::RAPI::ClearBits::COLOR | Rengine::RAPI::ClearBits::DEPTH);
		glfwPollEvents();
	}

	void WindowsWindow::OnUpdatePost()
	{
		glfwSwapBuffers(m_pWindow);
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.vsync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.vsync;
	}

}