#ifndef RENGINE_ENTRYPOINT_H
#define RENGINE_ENTRYPOINT_H

#ifdef RENGINE_PLATFORM_WINDOWS

#include <Rengine/Application.h>

extern Rengine::Application* Rengine::CreateApplication(int argc, char** argv);

int main(int argc, char** argv)
{
	RENGINE_LOG_INIT();
	RENGINE_CORE_INFO("Initialized Log.");
	RENGINE_INFO("Initialized Log.");

	auto application = Rengine::CreateApplication(argc, argv);
	application->Run();
	delete application;

	return 0;
}

#endif
#endif