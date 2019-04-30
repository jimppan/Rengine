#ifndef RENGINE_TILESHEET_H
#define RENGINE_TILESHEET_H

#include "Rengine/Render/Texture.h"

namespace Rengine
{
	struct tile_animation_frame_t
	{
		tile_animation_frame_t()
		{

		}

		tile_animation_frame_t(int tileId, int duration) :
			tile_id(tileId),
			duration(duration)
		{

		}

		tile_animation_frame_t(const tile_animation_frame_t& other) :
			tile_id(other.tile_id),
			duration(other.duration)
		{

		}

		int tile_id = -1;
		int duration = -1;
	};

	class TileSheet;
	class TileAnimation
	{
	public:
		TileAnimation(
			TileSheet* sheet, 
			int startId) :
			m_TileSheet(sheet),
			m_iStartTileID(startId),
			m_iCurrentFrame(0),
			m_iFramesToSkip(1),
			m_bBounce(false),
			m_iLastUpdate(0)
		{

		}

		TileAnimation(const TileAnimation& other) :
			m_TileSheet(other.m_TileSheet),
			m_iStartTileID(other.m_iStartTileID),
			m_iCurrentFrame(other.m_iCurrentFrame),
			m_iFramesToSkip(other.m_iFramesToSkip),
			m_bBounce(other.m_bBounce),
			m_iLastUpdate(other.m_iLastUpdate)
		{
			m_Frames.resize(other.m_Frames.size());
			for (unsigned int i = 0; i < other.m_Frames.size(); i++)
				m_Frames[i] = other.m_Frames[i];
		}

		int NextFrame(int currentTick);

		inline TileSheet*              GetTileSheet()                     { return m_TileSheet; }

		inline void                    AddFrame(int tileId, int duration) { m_Frames.emplace_back(tileId, duration); }
		inline tile_animation_frame_t* GetFrame(int index)                { return &m_Frames[index]; }
		inline tile_animation_frame_t* GetCurrentFrame()                  { return &m_Frames[m_iCurrentFrame]; }

		inline unsigned int            GetFrameCount() const              { return m_Frames.size(); }
		inline int                     GetStartTileID()                   { return m_iStartTileID; }
		inline int                     GetLastUpdate()                    { return m_iLastUpdate; }
		
		inline bool                    GetBounce() const                  { return m_bBounce; }
		inline void                    SetBounce(bool value)              { m_bBounce = value; }
	private:
		TileSheet* m_TileSheet;
		int m_iStartTileID;
		std::vector<tile_animation_frame_t> m_Frames;

		int m_iCurrentFrame;
		int m_iFramesToSkip;
		bool m_bBounce;
		int m_iLastUpdate;
	};

	/**
	 * gid = global tile id from tile map
	 * rid = relative tile id from this sprite sheet
	 */
	class TileSheet
	{
	public:
		TileSheet();
		~TileSheet();

		inline void        SetTexture(Texture* texture)      { m_Texture = texture; }
		inline void        SetName(const std::string& name)  { m_szName = name; }
					       
		inline void        SetTileWidth(int tilewidth)       { m_iTileWidth = tilewidth; }
		inline void        SetTileHeight(int tileheight)     { m_iTileHeight = tileheight; }
		inline void        SetTileCountX(int tilecountx)     { m_iTileCountX = tilecountx; }
		inline void        SetTileCountY(int tilecounty)     { m_iTileCountY = tilecounty; }
						   
		inline Texture*    GetTexture()                      { return m_Texture; }
		inline std::string GetName() const                   { return m_szName; }
		inline int         GetTileWidth() const              { return m_iTileWidth; }
		inline int         GetTileHeight() const             { return m_iTileHeight; }
		inline int         GetTileCountX() const             { return m_iTileCountX; }
		inline int         GetTileCountY() const             { return m_iTileCountY; }

		Recti              GetTileRect(int x, int y);
		Recti              GetTileRect(int rid);

		void               AddTileAnimation(const TileAnimation& animation);
	    TileAnimation*     GetTileAnimation(int rid);

		Recti              GetTileAnimationRect(int rid);

	private:
		Vector2i           IndexToCoordinate(int id);
	private:
		Texture* m_Texture;
		std::string m_szName;

		int m_iTileWidth = 0;
		int m_iTileHeight = 0;
		int m_iTileCountX = 0;
		int m_iTileCountY = 0;

		std::vector<TileAnimation> m_Animations;

		friend class TileAnimator;
	};
}

#endif // RENGINE_TILESHEET_H