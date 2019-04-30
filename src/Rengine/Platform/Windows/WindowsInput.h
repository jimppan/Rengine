#ifndef RENGINE_WINDOWS_INPUT_H
#define RENGINE_WINDOWS_INPUT_H

#include "Rengine/Core.h"
#include "Rengine/Input.h"

namespace Rengine
{
	class WindowsInput : public Input
	{
	protected:
		virtual bool IsKeyDownImpl(KeyCode keycode) override;
		virtual bool IsMouseButtonDownImpl(MouseButton button) override;
		virtual Vector2f GetMousePositionImpl() override;
	};
}

#endif // RENGINE_WINDOWS_INPUT_H