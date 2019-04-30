#include "PCH.h"
#include "Tiled.h"

#include "Rengine/IO/IO.h"
#include "Rengine/StringManip.h"
#include "Rengine/2D/TileSheet.h"
#include "Rengine/Resource/Factory.h"

namespace Rengine
{
	namespace Tiled
	{
		bool TiledMap::Import(const std::string& path)
		{
			std::string jsonData;
			if (!IO::ReadText(path, &jsonData))
			{
				RENGINE_CORE_ERROR("Could not import tiled map: {0}", path.c_str());
				return false;
			}

			Json json = Json::parse(jsonData);
			
			if (json.is_null())
			{
				RENGINE_CORE_ERROR("Could not import tiled map. Invalid json.: {0}", path.c_str());
				return false;
			}

			m_iTileWidth = json["tilewidth"].get<int>();
			m_iTileHeight = json["tileheight"].get<int>();

			if (m_iTileWidth != m_iTileHeight)
			{
				RENGINE_CORE_ERROR("Tile Width ({0}) does not match Tile Height ({1})!", m_iTileWidth, m_iTileHeight);
				return false;
			}

			m_iWidth = json["width"].get<int>();
			m_iHeight = json["height"].get<int>();

			if (m_iWidth != m_iHeight)
			{
				RENGINE_CORE_ERROR("Tile Map Width ({0}) does not match Tile Map Height ({1})!", m_iWidth, m_iHeight);
				return false;
			}

			if (m_iWidth != 16 && m_iWidth != 32 && m_iWidth != 64)
			{
				RENGINE_CORE_ERROR("Tile Map is not 2 to the power of X (Supported: 16, 32, 64)!");
				return false;
			}

			m_iNextObjectID = json["nextobjectid"].get<int>();

			m_iVersion = json["version"].get<int>();

			if (!json["hexsidelength"].is_null())
				m_iHexSideLength = json["hexsidelength"].get<int>();
			else
				m_iHexSideLength = 0;

			std::string szOrientation = json["orientation"].get<std::string>();
			if (szOrientation == "orthogonal")
				m_eOrientation = Orientation::ORTHOGONAL;
			else if (szOrientation == "isometric")
				m_eOrientation = Orientation::ISOMETRIC;
			else if (szOrientation == "staggered")
				m_eOrientation = Orientation::ISOMETRIC_S;
			else if (szOrientation == "hexagonal")
				m_eOrientation = Orientation::HEXAGONAL_S;

			std::string szRenderOrder = json["renderorder"].get<std::string>();
			if (szRenderOrder == "right-down")
				m_eRenderOrder = RenderOrder::RIGHT_DOWN;
			else if (szRenderOrder == "right-up")
				m_eRenderOrder = RenderOrder::RIGHT_UP;
			else if (szRenderOrder == "left-down")
				m_eRenderOrder = RenderOrder::LEFT_DOWN;
			else if (szRenderOrder == "left-up")
				m_eRenderOrder = RenderOrder::LEFT_UP;

			if (!json["staggeraxis"].is_null())
			{
				std::string szStaggerAxis = json["staggeraxis"].get<std::string>();
				if (szStaggerAxis == "y")
					m_eStaggerAxis = StaggerAxis::X;
				else if (szStaggerAxis == "x")
					m_eStaggerAxis = StaggerAxis::Y;
			}

			if (!json["staggerindex"].is_null())
			{
				std::string szStaggerIndex = json["staggerindex"];
				if (szStaggerIndex == "odd")
					m_eStaggerIndex = StaggerIndex::ODD;
				else if (szStaggerIndex == "even")
					m_eStaggerIndex = StaggerIndex::EVEN;
			}

			m_szTiledVersion = json["tiledversion"].get<std::string>();

			// Custom properties
			Json propertiesObj = json["properties"];

			if (!propertiesObj["tilesheetpath"].is_null())
				m_szTileSheetPath = propertiesObj["tilesheetpath"].get<std::string>();

			if (!json["tilesets"].is_null())
			{
				Json tilesetsArr = json["tilesets"];
				if (tilesetsArr.size() > 0)
				{
					m_TileSets.resize(tilesetsArr.size());

					for (unsigned int i = 0; i < tilesetsArr.size(); i++)
					{
						m_TileSets[i].m_iFirstGID = tilesetsArr[i]["firstgid"].get<int>();
						m_TileSets[i].m_szSource = tilesetsArr[i]["source"].get<std::string>();

						Tiled::TileSheet sheet;
						//TODO: Should not be up to the engine to decide the path
						sheet.Import("res/textures/" + m_szTileSheetPath + m_TileSets[i].m_szSource);
						String::Replace(m_TileSets[i].m_szSource, ".tsx", "");
					}
				}
				else
				{
					RENGINE_CORE_WARN("Loaded tiled map with 0 tile sets");
				}
			}

			if (!json["layers"].is_null())
			{
				Json layersArr = json["layers"];
				if (layersArr.size() > 0)
				{
					m_Layers.resize(layersArr.size());

					for (unsigned int i = 0; i < layersArr.size(); i++)
					{
						Json dataArr = layersArr[i]["data"];
						m_Layers[i].data.resize(dataArr.size());

						m_Layers[i].width = layersArr[i]["width"].get<int>();
						m_Layers[i].height = layersArr[i]["height"].get<int>();

						m_Layers[i].opacity = layersArr[i]["opacity"].get<int>();
						m_Layers[i].pos = Vector2i(layersArr[i]["x"].get<int>(), layersArr[i]["y"].get<int>());

						m_Layers[i].visible = layersArr[i]["visible"].get<bool>();

						m_Layers[i].type = layersArr[i]["type"].get<std::string>();
						m_Layers[i].name = layersArr[i]["name"].get<std::string>();

						// If render order is right down, then the layout is fine,
						// but is not, we have to flip this array around

						//TODO: not !=, but ==
						switch (m_eRenderOrder)
						{
						case RenderOrder::RIGHT_DOWN:
							for (unsigned int j = 0; j < dataArr.size(); j++)
								m_Layers[i].data[j] = dataArr[j].get<int>();
							break;
						case RenderOrder::RIGHT_UP:
							for (unsigned int j = 0; j < dataArr.size(); j++)
							{
								Vector2i ret;
								int ogY = (int)glm::floor(j / m_Layers[i].width);
								ret.y = glm::abs(ogY - m_Layers[i].height) - 1;
								ret.x = j - ogY * m_Layers[i].width;

								int index = ret.y * m_Layers[i].height + ret.x;

								m_Layers[i].data[j] = dataArr[index].get<int>();
							}
							break;
						}
					}
				}
				else
				{
					RENGINE_CORE_WARN("Loaded tiled map with 0 layers");
				}
			}

			return true;
		}

		TileMap* TiledMap::GenerateTileMap()
		{
			//TODO ALL LAYERS
			TileMap* tilemap = new TileMap(m_iWidth, m_iHeight, m_iTileWidth, m_iTileHeight);

			for (unsigned int i = 0; i < m_Layers.size(); i++)
			{
				TileLayer* layer = &m_Layers[i];
				TileMapLayer* tileMapLayer = tilemap->AddLayer(layer->name, layer->width, layer->height);

				for (unsigned int j = 0; j < layer->data.size(); j++)
				{
					int gid = layer->data[j];
					if (gid > 0)
					{
						TileSet* tileset = GetTileSetByGlobalTileIndex(gid);

						int rid = tileset->GlobalIdToRelativeId(gid);
						if (rid >= 0)
						{
							std::string tileSetName = tileset->GetDisplayName();
							Rengine::TileSheet* sheet = Rengine::Factory::TileSheets::GetTileSheet(tileSetName);
							if (sheet)
							{
								Vector2i pos;
								pos.y = (int)glm::floor((float)j / (float)m_iWidth);
								pos.x = j - pos.y * m_iWidth;

								Tile* tile = tileMapLayer->GetTile(j);

								tile->SetTileSheet(sheet);
								tile->SetUV(sheet->GetTileRect(rid));
								tile->SetColor({ 255, 255, 255, 255 });
								tile->SetRect({ pos.x * m_iTileWidth, pos.y * m_iTileHeight, m_iTileWidth, m_iTileHeight });
								tile->SetAnimation(sheet->GetTileAnimation(rid));

								if (std::find(tilemap->m_TileSheets.begin(), tilemap->m_TileSheets.end(), sheet) == tilemap->m_TileSheets.end())
									tilemap->m_TileSheets.push_back(sheet);
							}
						}
					}
				}
			}

			return tilemap;
		}

		TileLayer* TiledMap::GetLayer(const std::string& name)
		{
			for (unsigned int i = 0; i < m_Layers.size(); i++)
			{
				if (m_Layers[i].name == name)
					return &m_Layers[i];
			}
			return nullptr;
		}

		TileSet* TiledMap::GetTileSet(const std::string& name)
		{
			for (unsigned int i = 0; i < m_TileSets.size(); i++)
			{
				if (m_TileSets[i].m_szSource == name)
					return &m_TileSets[i];
			}
			return nullptr;
		}

		TileSet* TiledMap::GetTileSetByGlobalTileIndex(int gid)
		{
			for (unsigned int i = m_TileSets.size() - 1; i >= 0; i--)
			{
				if (gid >= m_TileSets[i].m_iFirstGID)
					return &m_TileSets[i];
			}
			return nullptr;
		}

		Rengine::TileSheet* TileSheet::Import(const std::string& path)
		{
			XMLDocument doc;

			if (doc.LoadFile(path.c_str()) != XML_SUCCESS)
			{
				RENGINE_CORE_ERROR("Could not import tiled tilset: {0}", path.c_str());
				return nullptr;
			}

			XMLElement* xmlTileset = doc.FirstChildElement("tileset");
			if (!xmlTileset)
				return nullptr;

			m_szName = xmlTileset->Attribute("name");
			Rengine::TileSheet* newSheet = Factory::TileSheets::GetTileSheet(m_szName);
			if (newSheet)
				return newSheet;

			m_iTileWidth = xmlTileset->IntAttribute("tilewidth");
			RENGINE_CORE_ASSERT(m_iTileWidth, "Tiled map width must be greater than 0", path.c_str());

			m_iTileHeight = xmlTileset->IntAttribute("tileheight");
			RENGINE_CORE_ASSERT(m_iTileHeight, "Tiled map height must be greater than 0", path.c_str());

			m_iTileCountX = xmlTileset->IntAttribute("columns");
			RENGINE_CORE_ASSERT(m_iTileCountX, "Tiled map tile count X must be greater than 0", path.c_str());

			m_iTileCountY = xmlTileset->IntAttribute("tilecount") / m_iTileCountX;
			RENGINE_CORE_ASSERT(m_iTileCountY, "Tiled map tile count Y must be greater than 0", path.c_str());

			newSheet = new Rengine::TileSheet();
			XMLElement* xmlTile = xmlTileset->FirstChildElement("tile");
			if (xmlTile)
			{
				do
				{
					Rengine::TileAnimation anim(newSheet, xmlTile->IntAttribute("id"));

					XMLElement* xmlAnimation = xmlTile->FirstChildElement("animation");
					if (xmlAnimation)
					{
						XMLElement* xmlFrame = xmlAnimation->FirstChildElement("frame");
						if (xmlFrame)
						{
							do
							{
								anim.AddFrame(xmlFrame->IntAttribute("tileid"), xmlFrame->IntAttribute("duration"));
							} while ((xmlFrame = xmlFrame->NextSiblingElement()));
						}
					}
					newSheet->AddTileAnimation(anim);
					
				} while ((xmlTile = xmlTile->NextSiblingElement()));
			}

			std::string texturePath = path;
			String::Replace(texturePath, ".tsx", ".png");

			
			newSheet->SetName(m_szName);
			newSheet->SetTexture(Rengine::Factory::Textures::AddTexture(texturePath, true)); // Always keep tilesheet data for sampling pixels
			newSheet->SetTileWidth(m_iTileWidth);
			newSheet->SetTileHeight(m_iTileHeight);
			newSheet->SetTileCountX(m_iTileCountX);
			newSheet->SetTileCountY(m_iTileCountY);
			return Rengine::Factory::TileSheets::AddTileSheet(newSheet);
		}
	}
}