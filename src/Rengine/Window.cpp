#include "PCH.h"
#include "Window.h"

#include "Rengine/Render/RenderAPI/RAPI.h"

namespace Rengine
{
	void Window::SetClearColor(const ColorRGBA32& color)
	{
		m_clrClearColor = color;
		Rengine::RAPI::RAPI::ClearColor(color);
	}
}