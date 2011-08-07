/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011  The ManaPlus Developers
 *
 *  This file is part of The ManaPlus Client.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MAP_H
#define MAP_H

#include "actor.h"
#include "configlistener.h"
#include "position.h"
#include "properties.h"

#include <list>
#include <string>
#include <vector>

#ifdef __GNUC__
#define A_UNUSED  __attribute__ ((unused))
#else
#define A_UNUSED
#endif

class Animation;
class AmbientLayer;
class Graphics;
class GraphicsVertexes;
class MapLayer;
class Particle;
class SimpleAnimation;
class Tileset;
class SpecialLayer;
class MapItem;
class ObjectsLayer;

class ImageVertexes;

typedef std::vector<Tileset*> Tilesets;
typedef std::vector<MapLayer*> Layers;
typedef std::vector<ImageVertexes*> MepRowImages;

class MapRowVertexes
{
    public:
        MapRowVertexes()
        {
            images.reserve(30);
        }

        ~MapRowVertexes();

        MepRowImages images;
};

/**
 * A meta tile stores additional information about a location on a tile map.
 * This is information that doesn't need to be repeated for each tile in each
 * layer of the map.
 */
struct MetaTile
{
    /**
     * Constructor.
     */
    MetaTile() : Fcost(0), Gcost(0), Hcost(0), whichList(0),
            parentX(0), parentY(0), blockmask(0)
    {}

    // Pathfinding members
    int Fcost;               /**< Estimation of total path cost */
    int Gcost;               /**< Cost from start to this location */
    int Hcost;               /**< Estimated cost to goal */
    unsigned whichList;      /**< No list, open list or closed list */
    int parentX;             /**< X coordinate of parent tile */
    int parentY;             /**< Y coordinate of parent tile */
    unsigned char blockmask; /**< Blocking properties of this tile */
};

class MapObject
{
    public:
        MapObject(int type, std::string data)
        {
            this->type = type;
            this->data = data;
        }

        int type;
        std::string data;
};

class MapObjectList
{
    public:
        std::vector<MapObject> objects;
};

/**
 * Animation cycle of a tile image which changes the map accordingly.
 */
class TileAnimation
{
    public:
        TileAnimation(Animation *ani);
        ~TileAnimation();
        void update(int ticks = 1);
        void addAffectedTile(MapLayer *layer, int index)
        { mAffected.push_back(std::make_pair(layer, index)); }
    private:
        std::vector<std::pair<MapLayer*, int> > mAffected;
        SimpleAnimation *mAnimation;
        Image *mLastImage;
};

/**
 * A map layer. Stores a grid of tiles and their offset, and implements layer
 * rendering.
 */
class MapLayer: public ConfigListener
{
    public:
        friend class Map;

        /**
         * Constructor, taking layer origin, size and whether this layer is the
         * fringe layer. The fringe layer is the layer that draws the actors.
         * There can be only one fringe layer per map.
         */
        MapLayer(int x, int y, int width, int height, bool isFringeLayer);

        /**
         * Destructor.
         */
        ~MapLayer();

        /**
         * Set tile image, with x and y in layer coordinates.
         */
        void setTile(int x, int y, Image *img);

        /**
         * Set tile image with x + y * width already known.
         */
        void setTile(int index, Image *img) { mTiles[index] = img; }

        /**
         * Get tile image, with x and y in layer coordinates.
         */
        Image *getTile(int x, int y) const;

        /**
         * Draws this layer to the given graphics context. The coordinates are
         * expected to be in map range and will be translated to local layer
         * coordinates and clipped to the layer's dimensions.
         *
         * The given actors are only drawn when this layer is the fringe
         * layer.
         */
        void draw(Graphics *graphics,
                  int startX, int startY,
                  int endX, int endY,
                  int scrollX, int scrollY,
                  int mDebugFlags) const;

        void drawOGL(Graphics *graphics);

        void drawSDL(Graphics *graphics);

        void updateOGL(Graphics *graphics,
                       int startX, int startY,
                       int endX, int endY,
                       int scrollX, int scrollY,
                       int mDebugFlags);

        void updateSDL(Graphics *graphics,
                       int startX, int startY,
                       int endX, int endY,
                       int scrollX, int scrollY,
                       int mDebugFlags);

        void drawFringe(Graphics *graphics,
                        int startX, int startY,
                        int endX, int endY,
                        int scrollX, int scrollY,
                        const Actors *actors,
                        int mDebugFlags, int yFix) const;

        bool isFringeLayer()
        { return mIsFringeLayer; }

        void setSpecialLayer(SpecialLayer *val)
        { mSpecialLayer = val; }

        void setTempLayer(SpecialLayer *val)
        { mTempLayer = val; }

        int getWidth()
        { return mWidth; }

        int getHeight()
        { return mHeight; }

//        void setTileInfo(int x, int y, int width, int cnt);

//        void getTileInfo(int x, int y, int &width, int &cnt) const;

        void optionChanged(const std::string &value);

        int getTileDrawWidth(int tilePtr, int endX, int &width) const;

//        void initTileInfo();

    private:
        int mX, mY;
        int mWidth, mHeight;
        bool mIsFringeLayer;    /**< Whether the actors are drawn. */
        bool mHighlightAttackRange;
        Image **mTiles;
//        int *mTilesWidth;
//        int *mTilesCount;
        SpecialLayer *mSpecialLayer;
        SpecialLayer *mTempLayer;
        typedef std::vector<MapRowVertexes*> MapRows;
        MapRows mTempRows;
};

/**
 * A tile map.
 */
class Map : public Properties, public ConfigListener
{
    public:
        enum BlockType
        {
            BLOCKTYPE_NONE = -1,
            BLOCKTYPE_WALL,
            BLOCKTYPE_CHARACTER,
            BLOCKTYPE_MONSTER,
            BLOCKTYPE_AIR,
            BLOCKTYPE_WATER,
            BLOCKTYPE_GROUND,
            NB_BLOCKTYPES
        };

        enum CollisionTypes
        {
            COLLISION_EMPTY = 0,
            COLLISION_WALL = 1,
            COLLISION_AIR = 2,
            COLLISION_WATER = 3,
            COLLISION_MAX = 4
        };

        enum BlockMask
        {
            BLOCKMASK_WALL      = 0x80, // 1000 0000
            BLOCKMASK_CHARACTER = 0x01, // 0000 0001
            BLOCKMASK_MONSTER   = 0x02, // 0000 0010
            BLOCKMASK_AIR       = 0x04, // 0000 0100
            BLOCKMASK_WATER     = 0x08, // 0000 1000
            BLOCKMASK_GROUND    = 0x16  // 0001 0000
        };

        enum DebugType
        {
            MAP_NORMAL  = 0,
            MAP_DEBUG   = 1,
            MAP_SPECIAL = 2,
            MAP_SPECIAL2 = 3,
            MAP_SPECIAL3 = 4,
            MAP_BLACKWHITE = 5
        };

        /**
         * Constructor, taking map and tile size as parameters.
         */
        Map(int width, int height, int tileWidth, int tileHeight);

        /**
         * Destructor.
         */
        ~Map();

        /**
         * Initialize ambient layers. Has to be called after all the properties
         * are set.
         */
        void initializeAmbientLayers();

        /**
         * Updates animations. Called as needed.
         */
        void update(int ticks = 1);

        /**
         * Draws the map to the given graphics output. This method draws all
         * layers, actors and overlay effects.
         *
         * TODO: For efficiency reasons, this method could take into account
         * the clipping rectangle set on the Graphics object. However,
         * currently the map is always drawn full-screen.
         */
        void draw(Graphics *graphics, int scrollX, int scrollY);

        /**
         * Visualizes collision layer for debugging
         */
        void drawCollision(Graphics *graphics, int scrollX, int scrollY,
                           int debugFlags);

        /**
         * Adds a layer to this map. The map takes ownership of the layer.
         */
        void addLayer(MapLayer *layer);

        /**
         * Adds a tileset to this map. The map takes ownership of the tileset.
         */
        void addTileset(Tileset *tileset);

        /**
         * Finds the tile set that a tile with the given global id is part of.
         */
        Tileset *getTilesetWithGid(int gid) const;

        /**
         * Get tile reference.
         */
        MetaTile *getMetaTile(int x, int y) const;

        /**
         * Marks a tile as occupied.
         */
        void blockTile(int x, int y, BlockType type);

        /**
         * Gets walkability for a tile with a blocking bitmask. When called
         * without walkmask, only blocks against colliding tiles.
         */
        bool getWalk(int x, int y,
                     unsigned char walkmask = BLOCKMASK_WALL | BLOCKMASK_AIR
                     | BLOCKMASK_WATER) const;

        /**
         * Tells whether a tile is occupied by a being.
         */
        bool occupied(int x, int y) const;

        /**
         * Returns the width of this map in tiles.
         */
        int getWidth() const { return mWidth; }

        /**
         * Returns the height of this map in tiles.
         */
        int getHeight() const { return mHeight; }

        /**
         * Returns the tile width of this map.
         */
        int getTileWidth() const
        { return mTileWidth; }

        /**
         * Returns the tile height used by this map.
         */
        int getTileHeight() const
        { return mTileHeight; }

        const std::string getMusicFile() const;
        const std::string getName() const;

        /**
         * Gives the map id based on filepath (ex: 009-1)
         */
        const std::string getFilename() const;

        /**
         * Check the current position against surrounding blocking tiles, and
         * correct the position offset within tile when needed.
         */
        Position checkNodeOffsets(int radius, unsigned char walkMask,
                                  const Position &position) const;

        Position checkNodeOffsets(int radius, unsigned char walkMask,
                                  int x, int y) const
        { return checkNodeOffsets(radius, walkMask, Position(x, y)); }

        /**
         * Find a pixel path from one location to the next.
         */
        Path findPixelPath(int startPixelX, int startPixelY,
                           int destPixelX, int destPixelY,
                           int radius, unsigned char walkmask,
                           int maxCost = 20);

        /**
         * Find a path from one location to the next.
         */
        Path findPath(int startX, int startY, int destX, int destY,
                      unsigned char walkmask, int maxCost = 20);

        /**
         * Adds a particle effect
         */
        void addParticleEffect(const std::string &effectFile,
                               int x, int y, int w = 0, int h = 0);

        /**
         * Initializes all added particle effects
         */
        void initializeParticleEffects(Particle* particleEngine);

        /**
         * Adds a tile animation to the map
         */
        void addAnimation(int gid, TileAnimation *animation)
        { mTileAnimations[gid] = animation; }

        void setDebugFlags(int n)
        { mDebugFlags = n; }

        int getDebugFlags() const
        { return mDebugFlags; }

        void addExtraLayer();

        void saveExtraLayer();

        SpecialLayer *getTempLayer()
        { return mTempLayer; }

        SpecialLayer *getSpecialLayer()
        { return mSpecialLayer; }

        void setHasWarps(bool n)
        { mHasWarps = n; }

        bool getHasWarps()
        { return mHasWarps; }

        std::string getUserMapDirectory() const;

        void addPortal(const std::string &name, int type,
                       int x, int y, int dx, int dy);

        void addRange(const std::string &name, int type,
                      int x, int y, int dx, int dy);

        void addPortalTile(const std::string &name, int type, int x, int y);

        void updatePortalTile(const std::string &name, int type,
                              int x, int y, bool addNew = true);

        std::vector<MapItem*> &getPortals()
        { return mMapPortals; }

        /**
         * Gets the tile animation for a specific gid
         */
        TileAnimation *getAnimationForGid(int gid) const;

        void optionChanged(const std::string &value);

        MapItem *findPortalXY(int x, int y);

        int getActorsCount() const
        { return static_cast<int>(mActors.size()); }

        void setPvpMode(int mode);

        ObjectsLayer* getObjectsLayer()
        { return mObjects; }

        std::string getObjectData(unsigned x, unsigned y, int type);

        void indexTilesets();

        void clearIndexedTilesets();

        void setActorsFix(int x, int y)
        { mActorFixX = x; mActorFixY = y; }

        int getVersion()
        { return mVersion; }

        void setVersion(int n)
        { mVersion = n; }

        void reduce();

        void redrawMap();

    protected:
        friend class Actor;
        friend class Minimap;

        /**
         * Adds an actor to the map.
         */
        Actors::iterator addActor(Actor *actor);

        /**
         * Removes an actor from the map.
         */
        void removeActor(Actors::iterator iterator);

    private:

        enum LayerType
        {
            FOREGROUND_LAYERS = 0,
            BACKGROUND_LAYERS
        };

        /**
         * Updates scrolling of ambient layers. Has to be called each game tick.
         */
        void updateAmbientLayers(float scrollX, float scrollY);

        /**
         * Draws the foreground or background layers to the given graphics output.
         */
        void drawAmbientLayers(Graphics *graphics, LayerType type,
                               int detail);

        /**
         * Tells whether the given coordinates fall within the map boundaries.
         */
        bool contains(int x, int y) const;

        /**
         * Blockmasks for different entities
         */
        unsigned *mOccupation[NB_BLOCKTYPES];

        int mWidth, mHeight;
        int mTileWidth, mTileHeight;
        int mMaxTileHeight;
        MetaTile *mMetaTiles;
        Layers mLayers;
        Tilesets mTilesets;
        Actors mActors;
        bool mHasWarps;

        // debug flags
        int mDebugFlags;

        // Pathfinding members
        unsigned mOnClosedList, mOnOpenList;

        // Overlay data
        std::vector<AmbientLayer*> mBackgrounds;
        std::vector<AmbientLayer*> mForegrounds;
        float mLastAScrollX;
        float mLastAScrollY;
//        bool mSpritesUpdated;

        // Particle effect data
        struct ParticleEffectData
        {
            std::string file;
            int x;
            int y;
            int w;
            int h;
        };
        std::vector<ParticleEffectData> particleEffects;

        std::vector<MapItem*> mMapPortals;

        std::map<int, TileAnimation*> mTileAnimations;

        int mOverlayDetail;
        float mOpacity;
        int mOpenGL;
        int mPvp;
        bool mTilesetsIndexed;
        Tileset** mIndexedTilesets;
        int mIndexedTilesetsSize;
        int mActorFixX;
        int mActorFixY;
        int mVersion;

        SpecialLayer *mSpecialLayer;
        SpecialLayer *mTempLayer;
        ObjectsLayer *mObjects;
        MapLayer *mFringeLayer;

        int mLastX;
        int mLastY;
        int mLastScrollX;
        int mLastScrollY;

        int mDrawX;
        int mDrawY;
        int mDrawScrollX;
        int mDrawScrollY;
        bool mRedrawMap;
        bool mBeingOpacity;
};


class SpecialLayer
{
    public:
        friend class Map;
        friend class MapLayer;

        SpecialLayer(int width, int height, bool drawSprites = false);

        ~SpecialLayer();

        void draw(Graphics *graphics, int startX, int startY,
                  int endX, int endY, int scrollX, int scrollY);

        MapItem* getTile(int x, int y) const;

        void setTile(int x, int y, MapItem* item);

        void setTile(int x, int y, int type);

        void addRoad(Path road);

        void clean();

        void itemDraw(Graphics *graphics, int x, int y,
                      int scrollX, int scrollY);

    private:
        int mWidth, mHeight;
        bool mDrawSprites;
        MapItem **mTiles;
};

class MapItem
{
    public:
        friend class Map;
        friend class MapLayer;

        enum ItemType
        {
            EMPTY = 0,
            HOME = 1,
            ROAD = 2,
            CROSS = 3,
            ARROW_UP = 4,
            ARROW_DOWN = 5,
            ARROW_LEFT = 6,
            ARROW_RIGHT = 7,
            PORTAL = 8,
            MUSIC = 9,
            ATTACK = 10,
            PRIORITY = 11,
            IGNORE_ = 12,
            SEPARATOR = 13
        };

        MapItem();

        MapItem(int type);

        MapItem(int type, std::string comment);

        MapItem(int type, std::string comment, int x, int y);

        ~MapItem();

        int getType() const
        { return mType; }

        void setType(int type);

        void setPos(int x, int y);

        int getX() const
        { return mX; }

        int getY() const
        { return mY; }

        std::string &getComment()
        { return mComment; }

        void setComment(std::string comment)
        { mComment = comment; }

        std::string &getName()
        { return mName; }

        void setName(std::string name)
        { mName = name; }

        void draw(Graphics *graphics, int x, int y, int dx, int dy);

    private:
        int mType;
        Image *mImage;
        std::string mComment;
        std::string mName;
        int mX;
        int mY;
};

class ObjectsLayer
{
    public:
        ObjectsLayer(unsigned width, unsigned height);
        ~ObjectsLayer();

        void addObject(std::string name, int type, unsigned x, unsigned y,
                       unsigned dx, unsigned dy);

        MapObjectList *getAt(unsigned x, unsigned y);
    private:
        MapObjectList **mTiles;
        unsigned mWidth;
        unsigned mHeight;
};

#endif
