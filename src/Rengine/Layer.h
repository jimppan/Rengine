#ifndef RENGINE_LAYER_H
#define RENGINE_LAYER_H

#include "Rengine/Core.h"
#include "Rengine/Events/Event.h"

namespace Rengine
{
	class Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& e) {}

		inline const std::string& GetName() const { return m_szName; }
	protected:
		std::string m_szName;

	};
}

#endif // RENGINE_LAYER_H
