#include "PCH.h"

#include "Application.h"
#include "Rengine/Input.h"

#include "Rengine/Render/RenderAPI/RAPI.h"
#include "Rengine/Time.h"
#include "Rengine/Render/Font.h"

#include <imgui.h>

namespace Rengine
{
	Application* Application::s_Instance = nullptr;

	Application::Application(const windowprops_t& props)
	{
		RENGINE_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_pWindow = std::unique_ptr<Window>(Window::Create(props));
		m_pWindow->SetEventCallback(RENGINE_BIND_FN(Application::OnEvent));

		// Init font after window (Window inits opengl)
		Font::Font::Initialize();

		m_pImGuiLayer = new ImGuiLayer();
		PushOverlay(m_pImGuiLayer);
	}


	Application::~Application()
	{
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowResizeEvent>(RENGINE_BIND_FN(Application::OnWindowResized));
		dispatcher.Dispatch<WindowCloseEvent>(RENGINE_BIND_FN(Application::OnWindowClosed));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.IsHandled())
			{
				if (m_NextActiveLayer)
				{
					SetActiveLayer(m_NextActiveLayer);
					m_NextActiveLayer = nullptr;
				}
				else
				{
					SetActiveLayer(*it);
				}
				break;
			}
		}
	}
#undef BIND_EVENT_FN
	void Application::Run()
	{
		while (m_bRunning)
		{
			Timer timer;
			
			m_pWindow->OnUpdatePre();

			for (Layer* layer : m_LayerStack)
			{
				layer->OnUpdate();
				layer->OnRender();
			}

			m_pImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_pImGuiLayer->End();

			m_pWindow->OnUpdatePost();
			Time::m_flDeltaTime = timer.GetSeconds();
			Time::m_flApplicationTime = m_ApplicationTimer.GetSeconds();
			Time::CalculateFPS();
			Time::CalculateTickCount();
		}
	}

	void Application::PushLayer(Layer* layer)
	{ 
		m_LayerStack.PushLayer(layer); 
		layer->OnAttach();
		m_ActiveLayer = layer;
		m_PreviousActiveLayer = layer;
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay); 
		overlay->OnAttach();
		m_ActiveLayer = overlay;
		m_PreviousActiveLayer = overlay;
	}

	Layer* Application::SetActiveLayer(const std::string& layer)
	{
		for (auto& it = LayerBegin(); it != LayerEnd(); it++)
		{
			if ((*it)->GetName() == layer)
			{
				if (m_ActiveLayer == *it)
					return m_ActiveLayer;

				m_PreviousActiveLayer = m_ActiveLayer;
				return (m_ActiveLayer = *it);
			}
		}
		return nullptr;
	}

	Layer* Application::SetActiveLayer(Layer* layer)
	{
		if (m_ActiveLayer == layer)
			return m_ActiveLayer;

		m_PreviousActiveLayer = m_ActiveLayer;
		return (m_ActiveLayer = layer);
	}

	Layer* Application::SetNextActiveLayer(Layer* layer)
	{
		return (m_NextActiveLayer = layer);
	}
	
	bool Application::OnWindowResized(WindowResizeEvent& e)
	{
		Rengine::RAPI::RAPI::SetViewport(Rect(0.0f, 0.0f, e.GetWidth(), e.GetHeight()));
		return true;
	}

	bool Application::OnWindowClosed(WindowCloseEvent& e)
	{
		m_bRunning = false;
		return true;
	}
}
