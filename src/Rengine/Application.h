#ifndef RENGINE_APPLICATION_H
#define RENGINE_APPLICATION_H

#include "Core.h"

#include "Rengine/Window.h"
#include "Rengine/LayerStack.h"
#include "Rengine/Events/ApplicationEvent.h"

#include "Rengine/ImGui/ImGuiLayer.h"
#include "Rengine/Time.h"

namespace Rengine
{
	class Application
	{
	public:
		Application(const windowprops_t& props);
		virtual ~Application();
		
		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline static Application* GetInstance() { return s_Instance; }

		inline Window* GetWindow() const { return m_pWindow.get(); }

		inline std::vector<Layer*>::iterator LayerBegin() { return m_LayerStack.begin(); }
		inline std::vector<Layer*>::iterator LayerEnd() { return m_LayerStack.end(); }

		Layer* SetActiveLayer(const std::string& layer);
		Layer* SetActiveLayer(Layer* layer);
		inline Layer* GetActiveLayer() { return m_ActiveLayer; }

		inline Layer* GetPreviousActiveLayer() { return m_PreviousActiveLayer; }

		// Stops the layer stack from automatically assigning the next layer, and sets it to this one instead
		// Useful in even callbacks
		Layer* SetNextActiveLayer(Layer* layer);

	private:
		bool OnWindowResized(WindowResizeEvent& e);
		bool OnWindowClosed(WindowCloseEvent& e);

		std::unique_ptr<Window> m_pWindow;
		ImGuiLayer* m_pImGuiLayer;
		bool m_bRunning = true;
		LayerStack m_LayerStack;
		Timer m_ApplicationTimer;
		Layer* m_ActiveLayer = nullptr;
		Layer* m_PreviousActiveLayer = nullptr;
		Layer* m_NextActiveLayer = nullptr;
	private:
		static Application* s_Instance;
	};

	// Entry point for every application
	Application* CreateApplication();
}


#endif // RENGINE_APPLICATION_H
