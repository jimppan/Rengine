#ifndef RENGINE_PCH
#define RENGINE_PCH

// general
#include <memory>
#include <iostream>
#include <algorithm>
#include <functional>
#include <vector>
#include <unordered_map>
#include <chrono>
#include <string>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <cstddef>
#include <cstdarg>

#ifdef RENGINE_PLATFORM_WINDOWS
	#include <Windows.h>
#endif

#include <Rengine/Math.h>

// spdlog
#include <Rengine/Log.h>

// json
#include <Rengine/Json.h>

// xml
#include <Rengine/XML.h>

namespace Rengine
{
	typedef nlohmann::json Json;
}

#endif RENGINE_PCH