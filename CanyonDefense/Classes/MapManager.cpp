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


void MapManager::initMapWithMapId(int mapId)
{
    setMapId(mapId);
    setMapTileForGame();
}

void MapManager::setMapTileForGame()
{

    switch (getMapId()) {
        case 1:
            _tileMap = TMXTiledMap::create("TileMap.tmx");

            log("set tile map");
            break;
        case 2:
            _tileMap = TMXTiledMap::create("TileMap.tmx");
            break;
            
        default:
            break;
    }
}
TMXTiledMap* MapManager::getTileMap()
{
    return _tileMap;
}