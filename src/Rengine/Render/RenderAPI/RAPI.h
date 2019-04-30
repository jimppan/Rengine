#ifndef RENGINE_RAPI_CORE_H
#define RENGINE_RAPI_CORE_H

#include "Rengine/Core.h"
#include <glad/glad.h>

#if defined RENGINE_DEBUG
#define GLCall(x) Rengine::RAPI::RAPI::GLClearError();\
	x;\
	Rengine::RAPI::RAPI::GLLogCall(#x, __FILE__, __LINE__);
#else
#define GLCall(x) x
#endif

/*
#define BENCHMARK(x)\
	double startTime = glfwGetTime();\
	x;\
	std::cout << "Elapsed time: " << glfwGetTime() - startTime;

#define BENCHMARK_TIMES(x, times)\
	double* results = new double[times];\
	for(int i = 0; i < times; i++)\
	{\
		double startTime = glfwGetTime();\
		x;\
		double result = glfwGetTime() - startTime;\
		results[i] = result;\
	}\
	for(int i = 0; i < times; i++)\
	{\
		std::cout << "Run " << i << " - Elapsed time: " << results[i] << std::endl;\
	}\
	std::cout << "Fastest time: " << GetMinArray(results, times) << std::endl;\
	std::cout << "Slowest time: " << GetMaxArray(results, times) << std::endl;\
	std::cout << "Average time: " << GetAverageArray(results, times) << std::endl;\
	delete[] results;


template<typename T>
static T GetMinArray(T* arr, const int& size)
{
	T min = arr[0];
	for (int i = 1; i < size; i++)
	{
		if (arr[i] < min)
			min = arr[i];
		//(arr[i] < min) ? min = arr[i];
	}

	return min;
}

template<typename T>
static T GetMaxArray(T* arr, const int& size)
{
	T max = arr[0];
	for (int i = 1; i < size; i++)
	{
		if (arr[i] > max)
			max = arr[i];
		//(arr[i] > max) ? max = arr[i];
	}

	return max;
}

template<typename T>
static T GetAverageArray(T* arr, const int& size)
{
	T added = arr[0];
	for (int i = 1; i < size; i++)
		added += arr[i];

	return added / size;
}
*/

namespace Rengine
{
	namespace RAPI
	{
		enum DrawType
		{
			STATIC = GL_STATIC_DRAW,
			DYNAMIC = GL_DYNAMIC_DRAW,
			STREAM = GL_STREAM_DRAW
		};

		enum ClearBits
		{
			COLOR = GL_COLOR_BUFFER_BIT,
			DEPTH = GL_DEPTH_BUFFER_BIT,
			STENCIL = GL_STENCIL_BUFFER_BIT
		};

		class RAPI
		{
		public:
			static void ClearColor(const ColorRGBA32& color);
			static void ClearWindow(const unsigned int& bitfield);
			static void SetViewport(const Rect& rect);

			static void GLClearError();
			static bool GLLogCall(const char* function, const char* file, int line);
		};
	}
}
#endif // RENGINE_RAPI_CORE_H