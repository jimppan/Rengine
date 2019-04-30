#ifndef RENGINE_FACTORY_H
#define RENGINE_FACTORY_H

#include "PCH.h"
#include "Rengine/Render/Texture.h"
#include "Rengine/2D/TileSheet.h"
#include "Rengine/Render/RenderAPI/RAPIShader.h"

namespace Rengine
{
	namespace Factory
	{
		class Textures
		{
		public:
			static Texture* GetTexture(const std::string& path, bool keepData = false);
			static Texture* AddTexture(const std::string& path, bool keepData);
			static Texture* AddTexture(const std::string& key, Texture* texture);

			static inline std::unordered_map<std::string, Texture*>::iterator Begin() { return m_Textures.begin(); }
			static inline std::unordered_map<std::string, Texture*>::iterator End()   { return m_Textures.end(); }
		private:
			static std::unordered_map<std::string, Texture*> m_Textures;
		};

		class TileSheets
		{
		public:
			static TileSheet* GetTileSheet(const std::string& name);
			static TileSheet* AddTileSheet(TileSheet* sheet);

			static inline std::unordered_map<std::string, TileSheet*>::iterator Begin() { return m_TileSheets.begin(); }
			static inline std::unordered_map<std::string, TileSheet*>::iterator End() { return m_TileSheets.end(); }
		private:
			static std::unordered_map<std::string, TileSheet*> m_TileSheets;
		};

		class Shaders
		{
		public:
			static RAPI::Shader* GetShader(const std::string& name);
			static RAPI::Shader* AddShader(const std::string& name);

			static inline std::unordered_map<std::string, RAPI::Shader>::iterator Begin() { return m_Shaders.begin(); }
			static inline std::unordered_map<std::string, RAPI::Shader>::iterator End() { return m_Shaders.end(); }
		private:
			static std::unordered_map<std::string, RAPI::Shader> m_Shaders;
		};
	}
}

#endif // RENGINE_FACTORY_H