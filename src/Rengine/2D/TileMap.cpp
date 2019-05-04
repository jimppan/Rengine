#include "PCH.h"
#include "TileMap.h"

namespace Rengine
{
	TileMap::TileMap(int width,
		int height,
		int tilewidth,
		int tileheight) :
		m_iWidth(width),
		m_iHeight(height),
		m_iTileWidth(tilewidth),
		m_iTileHeight(tileheight)
	{
		InitTiles();
	}

	TileMap::~TileMap()
	{
	
	}

	void TileMap::Apply()
	{
		m_TileMapRenderer.Begin();
		for (unsigned int i = 0; i < m_TileMapLayers.size(); i++)
		{
			TileMapLayer* layer = &m_TileMapLayers[i];

			for (unsigned int j = 0; j < layer->m_Tiles.size(); j++)
			{
				Tile* tile = layer->GetTile(j);
				if (tile && tile->GetTileSheet())
				{
					m_TileMapRenderer.Draw(tile, i, j, tile->GetAnimation());
				}
			}
		}
		m_TileMapRenderer.End();
	}

	void TileMap::Render(Camera* cam, const Vector3f& pos)
	{
		m_TileMapRenderer.Render(cam, pos);
	}

	TileMapLayer* TileMap::GetLayer(const std::string& layer)
	{
		for (unsigned int i = 0; i < m_TileMapLayers.size(); i++)
		{
			if (m_TileMapLayers[i].m_szLayerName == layer)
				return &m_TileMapLayers[i];
		}
		return nullptr;
	}

	TileMapLayer* TileMap::AddLayer(const std::string& layer, int width, int height)
	{
		TileMapLayer* ret = nullptr;
		if ((ret = GetLayer(layer)))
			return ret;

		m_TileMapLayers.emplace_back(this, layer, width, height, m_TileMapLayers.size());
		return &m_TileMapLayers[m_TileMapLayers.size() - 1];
	}

	TilePair TileMap::GetFirstTile(int x, int y)
	{
		if (x >= m_iWidth || x < 0 || y >= m_iHeight || y < 0)
			return TilePair(nullptr, nullptr);

		int id = y * m_iWidth + x;
		return GetFirstTile(id);
	}

	TilePair TileMap::GetFirstTile(int id)
	{
		// Loop from front to back
		for (int i = (int)m_TileMapLayers.size() - 1; i >= 0; i--)
		{
			if (id < 0 || id >= (int)m_TileMapLayers[i].m_Tiles.size())
				continue;

			Tile* tile = m_TileMapLayers[i].GetTile(id);
			if (tile && tile->GetTileSheet())
				return TilePair(&m_TileMapLayers[i], tile);
		}

		return TilePair(nullptr, nullptr);
	}

	TilePair TileMap::GetFirstTileByPixel(int x, int y, const Vector2f& relativePos)
	{
		if (x >= m_iWidth || x < 0 || y >= m_iHeight || y < 0)
			return TilePair(nullptr, nullptr);

		int id = y * m_iWidth + x;
		return GetFirstTileByPixel(id, relativePos);
	}

	// Pixel pos is value from 0 to TILE_WIDTH (0 - 16 by default)
	// up is 1.0, right is 1.0
	TilePair TileMap::GetFirstTileByPixel(int id, const Vector2f& pixelPos)
	{
		// Loop from front to back
		for (int i = (int)m_TileMapLayers.size() - 1; i >= 0; i--)
		{
			if (id < 0 || id >= (int)m_TileMapLayers[i].m_Tiles.size())
				continue;

			// Check if the pixels alpha value greater than 0, then its a valid tile
			Tile* tile = m_TileMapLayers[i].GetTile(id);
			if (tile && tile->GetTileSheet())
			{
				if(tile->GetPixel((int)pixelPos.x, (int)pixelPos.y).a > 0)
					return TilePair(&m_TileMapLayers[i], tile);
			}
		}

		return TilePair(nullptr, nullptr);
	}

	void TileMap::InitTiles()
	{
		for (unsigned int i = 0; i < m_TileMapLayers.size(); i++)
			m_TileMapLayers[i].InitTiles();
	}

	void TileMap::ClearTiles()
	{
		for (unsigned int i = 0; i < m_TileMapLayers.size(); i++)
			m_TileMapLayers[i].ClearTiles();
	}

	/* TILE MAP LAYER */
	TileMapLayer::TileMapLayer(TileMap* tilemap,
		const std::string& layer,
		int width,
		int height,
		int layerId) :
		m_iWidth(width),
		m_iHeight(height),
		m_szLayerName(layer),
		m_TileMap(tilemap),
		m_iLayerID(layerId)
	{
		InitTiles();
	}

	void TileMapLayer::InitTiles()
	{
		m_Tiles.resize(m_iWidth * m_iHeight);
	}

	void TileMapLayer::ClearTiles()
	{
		m_Tiles.clear();
	}

	Tile* TileMapLayer::GetTile(int x, int y)
	{
		if (x >= m_iWidth || x < 0 || y >= m_iHeight || y < 0)
			return nullptr;

		return GetTile(y * m_iWidth + x);
	}

	void TileMapLayer::SetTile(int x, int y, const Tile& tile)
	{
		if (x >= m_iWidth || x < 0 || y >= m_iHeight || y < 0)
			return;

		SetTile(y * m_iWidth + x, tile);
	}

	void TileMapLayer::UpdateTile(int x, int y)
	{
		if (x >= m_iWidth || x < 0 || y >= m_iHeight || y < 0)
			return;

		UpdateTile(y * m_iWidth + x);
	}

	void TileMapLayer::UpdateTile(int id)
	{
		Tile* tile = GetTile(id);
		if(tile)
			m_TileMap->GetRenderer()->UpdateTile(m_iLayerID, id, GetTile(id));
	}
}