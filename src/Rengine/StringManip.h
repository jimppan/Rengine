#ifndef RENGINE_STRING_MANIP_H
#define RENGINE_STRING_MANIP_H

#include "PCH.h"

namespace Rengine
{
	namespace String
	{
		static bool Replace(std::string& buffer, const std::string& find, const std::string& replace)
		{
			size_t startPos = buffer.find(find);
			if (startPos == std::string::npos)
				return false;

			buffer.replace(startPos, find.length(), replace);
			return true;
		}

		static void ReplaceAll(std::string& buffer, const std::string& find, const std::string& replace)
		{
			size_t startPos = 0;
			while ((startPos = buffer.find(find, startPos)) != std::string::npos)
			{
				buffer.replace(startPos, find.length(), replace);
				startPos += replace.length();
			}
		}
	}
}

#endif // RENGINE_STRING_MANIP_H
