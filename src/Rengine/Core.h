#ifndef RENGINE_CORE_H
#define RENGINE_CORE_H

#ifdef RENGINE_DEBUG
#define RENGINE_ENABLE_ASSERTS
#endif

#ifdef RENGINE_ENABLE_ASSERTS
#define RENGINE_ASSERT(x, ...) { if(!(x)) { RENGINE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#define RENGINE_CORE_ASSERT(x, ...) { if(!(x)) { RENGINE_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
#define RENGINE_ASSERT(x, ...)
#define RENGINE_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define RENGINE_BIND_FN(fn) std::bind(&fn, this, std::placeholders::_1)

#endif // RENGINE_CORE_H