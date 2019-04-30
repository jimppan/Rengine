#ifndef RENGINE_TILE_MAP_H
#define RENGINE_TILE_MAP_H

#include "Rengine/Render/Renderer/TileMapRenderer.h"
#include "Rengine/2D/Tile.h"
#include "Rengine/2D/TileSheet.h"

namespace Rengine
{
	static const int TILE_WIDTH    = 16;
	static const int TILE_HEIGHT   = 16;
	static const int TILE_MAP_SIZE = 32;

	class TileMapLayer;
	typedef std::pair<TileMapLayer*, Tile*> TilePair;

	class TileMap
	{
	public:
		TileMap(int width = TILE_MAP_SIZE, int height = TILE_MAP_SIZE, int tilewidth = TILE_WIDTH, int tileheight = TILE_HEIGHT);
		~TileMap();

		inline int              GetWidth() const                                           { return m_iWidth; }
		inline int              GetHeight() const                                          { return m_iHeight; }
		inline int              GetTileWidth() const                                       { return m_iTileWidth; }
		inline int              GetTileHeight() const                                      { return m_iTileHeight; }
																				           
		inline TileMapRenderer* GetRenderer()                                              { return &m_TileMapRenderer; }
		inline Rect             GetRect()                                                  { return Rect(0.0f, 0.0f, m_iWidth * m_iTileWidth, m_iHeight * m_iTileHeight); }
																				           
		inline void             SetWidth(const int& width)                                 { m_iWidth = width; }
		inline void             SetHeight(const int& height)                               { m_iHeight = height; }
		inline void             SetTileWidth(const int& tilewidth)                         { m_iTileWidth = tilewidth; }
		inline void             SetTileHeight(const int& tileheight)                       { m_iTileHeight = tileheight; }

		void                    Apply();

		void                    Render(Camera* cam, const Vector3f& pos);

		TileMapLayer*           GetLayer(const std::string& layer);
		TileMapLayer*           AddLayer(const std::string& layer, int width, int height);

		TilePair                GetFirstTile(int x, int y);
		TilePair                GetFirstTile(int id);

		TilePair                GetFirstTileByPixel(int x, int y, const Vector2f& relativePos);
		TilePair                GetFirstTileByPixel(int id, const Vector2f& relativePos);

		inline std::vector<TileMapLayer>::iterator LayerBegin() { return m_TileMapLayers.begin(); };
		inline std::vector<TileMapLayer>::iterator LayerEnd()   { return m_TileMapLayers.end(); }
	private:
		void                    InitTiles();
		void                    ClearTiles();

	public:
		std::vector<TileSheet*> m_TileSheets;
	private:
		int m_iWidth = 0;
		int m_iHeight = 0;

		int m_iTileWidth = 0;
		int m_iTileHeight = 0;
		TileMapRenderer m_TileMapRenderer;
		std::vector<TileMapLayer> m_TileMapLayers;
	};

	class TileMapLayer
	{
	public:
		TileMapLayer(TileMap* tilemap, const std::string& layer, int width, int height, int layerId);

		inline int              GetWidth() const { return m_iWidth; }
		inline int              GetHeight() const { return m_iHeight; }
		inline std::string      GetLayerName() const { return m_szLayerName; }

		inline void             SetWidth(const int& width) { m_iWidth = width; }
		inline void             SetHeight(const int& height) { m_iHeight = height; }
		inline void             SetLayerName(const std::string& name) { m_szLayerName = name; }

		Tile*                   GetTile(int x, int y);
		inline Tile*            GetTile(int id) { return &m_Tiles[id]; }

		void                    SetTile(int x, int y, const Tile& tile);
		inline void             SetTile(int id, const Tile& tile) { m_Tiles[id] = Tile(tile); }

		void                    UpdateTile(int x, int y);
		void                    UpdateTile(int id);

	private:
		void                    InitTiles();
		void                    ClearTiles();
	private:
		int m_iWidth = 0;
		int m_iHeight = 0;
		std::string m_szLayerName;

		std::vector<Tile> m_Tiles;

		TileMap* m_TileMap;
		int m_iLayerID = 0;

		friend class TileMap;
	};
}


#endif // RENGINE_TILE_MAP_H
