#ifndef RENGINE_TILE_MAP_RENDERER_H
#define RENGINE_TILE_MAP_RENDERER_H

#include "Rengine/Render/RenderAPI/RAPIBuffers.h"
#include "Rengine/Render/RenderAPI/RAPIShader.h"
#include "Rengine/Render/Camera.h"
#include "Rengine/Render/Renderer/Renderer.h"
#include "Rengine/2D/TileSheet.h"

//#include "Rengine/2D/Tile.h"

namespace Rengine
{
	class TileBatch
	{
	public:
		TileBatch(int offset, int vertCount, Texture* texture, TileAnimation* anim) :
			m_iOffset(offset),
			m_iNumVertices(vertCount),
			m_Texture(texture),
			m_Animation(anim)
		{

		}

		unsigned int m_iOffset;
		unsigned int m_iNumVertices;
		Texture* m_Texture;
		TileAnimation* m_Animation;
	};

	class Tile;
	class TileMapRenderer
	{
	public:
		TileMapRenderer();
		~TileMapRenderer();

		void                 Initialize();
							 
		void                 Begin(GlyphSortType sortType = GlyphSortType::BACK_TO_FRONT);
		void                 End();
							 
		void                 Draw(Tile* tile, int layerIndex, int tileIndex, TileAnimation* anim);
							 
		inline void          Render(Camera* cam, Vector3f pos) { Render(cam, ColorRGBA8{ 255, 255, 255, 255 }, pos); }
		void                 Render(Camera* cam, ColorRGBA8 color, Vector3f pos);
		void                 Render(Camera* cam, ColorRGBA32 color, Vector3f pos);
							 
		inline RAPI::Shader* GetShader() { return m_Shader; }
		inline void          SetShader(RAPI::Shader* shader) { m_Shader = shader; }
							 
		void                 UpdateTile(int layerId, int tileid, Tile* tile);

	private:				 
		void                 SortGlyphsTextures();
		void                 SortGlyphLayers();
		void                 GenerateTileBatches();
		int                  GetGlyphIndexByTileId(int layerId, int tileId);

		static bool          LayerFrontToBackSort(TileGlyph* first, TileGlyph* second);
		static bool          LayerBackToFrontSort(TileGlyph* first, TileGlyph* second);
		static bool          AnimationSort(TileGlyph* first, TileGlyph* second);
	public:
		std::vector<TileBatch> m_TileBatches;
	private:
		RAPI::VertexBuffer m_VBO;
		RAPI::VertexArray m_VAO;

		std::vector<TileGlyph*> m_GlyphPointers;
		std::vector<TileGlyph> m_Glyphs;
		GlyphSortType m_eGlyphSortType;

		RAPI::Shader* m_Shader = nullptr;
	};
}


#endif // RENGINE_TILE_MAP_RENDERER_H

