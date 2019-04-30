#include "PCH.h"
#include "Factory.h"

#include "Rengine/IO/IO.h"

namespace Rengine
{
	namespace Factory
	{
		std::unordered_map<std::string, Texture*> Textures::m_Textures;
		std::unordered_map<std::string, TileSheet*> TileSheets::m_TileSheets;
		std::unordered_map<std::string, RAPI::Shader> Shaders::m_Shaders;

		/* TEXTURES */
		Texture* Textures::GetTexture(const std::string& path, bool keepData)
		{
			auto& it = m_Textures.find(path);
			if (it == m_Textures.end())
				return AddTexture(path, keepData);
			return it->second;
		}

		Texture* Textures::AddTexture(const std::string& path, bool keepData)
		{
			auto& it = m_Textures.find(path);
			if (it != m_Textures.end())
				return it->second;

			Texture* newTexture = new Texture();

			if (!newTexture->LoadPNG(path, keepData))
				return nullptr;

			RENGINE_CORE_INFO("Loaded texture \"{0}\"", path.c_str());
			return newTexture;
		}

		Texture* Textures::AddTexture(const std::string& key, Texture* texture)
		{
			auto& it = m_Textures.find(key);
			if (it != m_Textures.end())
				return it->second;

			m_Textures[key] = texture;

			RENGINE_CORE_INFO("Added texture \"{0}\"", key.c_str());
			return texture;
		}

		/* TILE SHEETS*/
		TileSheet* TileSheets::GetTileSheet(const std::string& name)
		{
			auto& it = m_TileSheets.find(name);
			if (it == m_TileSheets.end())
				return nullptr;
			return it->second;
		}

		TileSheet* TileSheets::AddTileSheet(TileSheet* sheet)
		{
			auto& it = m_TileSheets.find(sheet->GetName());
			if (it != m_TileSheets.end())
				return it->second;

			m_TileSheets[sheet->GetName()] = sheet;
			TileSheet* newSheet = m_TileSheets[sheet->GetName()];
			RENGINE_CORE_INFO("Added tilesheet \"{0}\"", sheet->GetName().c_str());
			return newSheet;
		}

		/* SHADERS */
		RAPI::Shader* Shaders::GetShader(const std::string& path)
		{
			auto& it = m_Shaders.find(path);
			if (it == m_Shaders.end())
				return AddShader(path);
			return &it->second;
		}

		RAPI::Shader* Shaders::AddShader(const std::string& path)
		{
			auto& it = m_Shaders.find(path);
			if (it != m_Shaders.end())
				return &it->second;

			m_Shaders[path] = RAPI::Shader();
			RAPI::Shader* newShader = &m_Shaders[path];
			newShader->CreateShader(path);

			RENGINE_CORE_INFO("Loaded shader \"{0}\"", path.c_str());
			return newShader;
		}
	}
}
