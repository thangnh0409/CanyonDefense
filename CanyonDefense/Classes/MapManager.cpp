//
//  MapManager.cpp
//  CanyonDefense
//
//  Created by thang nguyen on 2/26/14.
//
//

#include "MapManager.h"

static MapManager* _sharedContext;

MapManager* MapManager::shareMap()
{
    if (!_sharedContext) {
        _sharedContext = new MapManager;
        _sharedContext->initMapWithMapId(1);
    }
    return _sharedContext;
}

MapManager* MapManager::createMap()
{
    MapManager* map = new MapManager;
    if (map) {
        map->initMapWithMapId(1);
        map->autorelease();
        return map;
    }
    CC_SAFE_DELETE(map);
    return NULL;
}

void MapManager::initMapWithMapId(int mapId)
{
    setMapId(mapId);
    setMapTileForGame();
}

void MapManager::setMapTileForGame()
{
    TMXTiledMap* mapTile = NULL;
    switch (getMapId()) {
        case 1:
            mapTile = TMXTiledMap::create("TileMap.tmx");
            setTileMap(mapTile);
            log("set tile map");
            break;
        case 2:
            mapTile = TMXTiledMap::create("TileMap2.tmx");
            setTileMap(mapTile);
            break;
            
        default:
            break;
    }
}