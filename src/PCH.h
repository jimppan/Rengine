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
#include <cstdint>
#include <cstddef>
#include <cstdarg>
#include <filesystem>

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

typedef std::int8_t         int8;
typedef std::uint8_t        uint8;
typedef std::int16_t        int16;
typedef std::uint16_t       uint16;
typedef std::int32_t        int32;
typedef std::uint32_t       uint32;
typedef std::int64_t        int64;
typedef std::uint64_t       uint64;

#endif RENGINE_PCH