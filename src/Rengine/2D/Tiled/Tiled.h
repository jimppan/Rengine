#ifndef RENGINE_TILED_H
#define RENGINE_TILED_H

#include "Rengine/2D/TileMap.h"

namespace Rengine
{
	namespace Tiled
	{
		enum class Orientation
		{
			ORTHOGONAL,
			ISOMETRIC,
			ISOMETRIC_S,
			HEXAGONAL_S
		};

		enum class RenderOrder
		{
			RIGHT_DOWN,
			RIGHT_UP,
			LEFT_DOWN,
			LEFT_UP
		};

		enum class StaggerAxis
		{
			X,
			Y
		};

		enum class StaggerIndex
		{
			ODD,
			EVEN
		};

		class TileSet
		{
		public:
			inline std::string GetDisplayName() const { return m_szSource; }
			inline int         GlobalIdToRelativeId(int gid) const { return (gid <= 0) ? -1 : gid - m_iFirstGID; }
			inline int         GetFirstGlobalId() const { return m_iFirstGID; }
		private:
			std::string m_szSource;
			int m_iFirstGID;

			friend class TiledMap;
		};

		struct TileLayer
		{
		public:
			std::vector<int> data;

			std::string type;
			std::string name;

			int opacity;
			int width;
			int height;
			bool visible;

			Vector2i pos;
		};

		class TiledMap
		{
		public:

			bool                             Import(const std::string& path);
			TileMap*                         GenerateTileMap();

			TileLayer*                       GetLayer(const std::string& name);
			TileSet*                         GetTileSet(const std::string& name);
			TileSet*                         GetTileSetByGlobalTileIndex(int gid);
										     
			inline int                       GetVersion() const        { return m_iVersion; }
			inline std::string               GetTiledVersion() const   { return m_szTiledVersion; }
			inline int                       GetWidth() const          { return m_iWidth; }
			inline int                       GetHeight() const         { return m_iHeight; }
			inline int                       GetTileWidth() const      { return m_iTileWidth; }
			inline int                       GetTileHeight() const     { return m_iTileHeight; }
			inline int                       GetHexSideLength() const  { return m_iHexSideLength; }
			inline Orientation               GetOrientation() const    { return m_eOrientation; }
			inline RenderOrder               GetRenderOrder() const    { return m_eRenderOrder; }
			inline StaggerAxis               GetStaggerAxis() const    { return m_eStaggerAxis; }
			inline StaggerIndex              GetStaggerIndex() const   { return m_eStaggerIndex; }
			inline std::vector<TileSet>*     GetTileSets()             { return &m_TileSets; }
			inline std::vector<TileLayer>*   GetLayers()               { return &m_Layers; }
			inline int                       GetNextObjectID() const   { return m_iVersion; }
		private:
			int m_iVersion;
			std::string m_szTiledVersion;

			int m_iWidth;
			int m_iHeight;

			int m_iTileWidth;
			int m_iTileHeight;

			int m_iHexSideLength;

			Orientation m_eOrientation;
			RenderOrder m_eRenderOrder;

			StaggerAxis m_eStaggerAxis;
			StaggerIndex m_eStaggerIndex;

			std::vector<TileSet> m_TileSets;
			std::vector<TileLayer> m_Layers;

			int m_iNextObjectID;

			// Custom property
			std::string m_szTileSheetPath;
		};

		class TileSheet
		{
		public:
			Rengine::TileSheet* Import(const std::string& path);
		private:
			int m_iTileWidth;
			int m_iTileHeight;
			int m_iTileCountX;
			int m_iTileCountY;

			std::string m_szName;
		};
	};
}

#endif // RENGINE_TILED_H


