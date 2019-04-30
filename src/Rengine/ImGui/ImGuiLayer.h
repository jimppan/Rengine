#ifndef RENGINE_IMGUI_LAYER_H
#define RENGINE_IMGUI_LAYER_H

#include "Rengine/Core.h"
#include "Rengine/Layer.h"

#include "Rengine/Events/ApplicationEvent.h"
#include "Rengine/Events/Event.h"
#include "Rengine/Events/KeyEvent.h"
#include "Rengine/Events/MouseEvent.h"

namespace Rengine
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
	};
}

#endif // RENGINE_IMGUI_LAYER_H

