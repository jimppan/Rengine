#include "PCH.h"
#include "WindowsInput.h"

#include <GLFW/glfw3.h>
#include "Rengine/Application.h"

namespace Rengine
{
	Input* Input::s_Instance = new WindowsInput();

	bool WindowsInput::IsKeyDownImpl(KeyCode keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::GetInstance()->GetWindow()->GetNativeWindow());
		int state = glfwGetKey(window, (int)keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool WindowsInput::IsMouseButtonDownImpl(MouseButton button)
	{
		auto window = static_cast<GLFWwindow*>(Application::GetInstance()->GetWindow()->GetNativeWindow());
		int state = glfwGetMouseButton(window, (int)button);
		return state == GLFW_PRESS;
	}

	Vector2f WindowsInput::GetMousePositionImpl()
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::GetInstance()->GetWindow()->GetNativeWindow());;
		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);
		yPos = glm::abs((yPos - Application::GetInstance()->GetWindow()->GetHeight()) - 1);
		return Vector2f((float)xPos, (float)yPos);
	}
}