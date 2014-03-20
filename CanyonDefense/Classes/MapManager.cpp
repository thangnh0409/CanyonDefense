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
            _tileMap = TMXTiledMap::create("map_lv1.tmx");

            log("set tile map");
            break;
        case 2:
            _tileMap = TMXTiledMap::create("map_lv3.tmx");
            break;
        case 3:
            _tileMap = TMXTiledMap::create("map_lv2.tmx");
            break;
        default:
            _tileMap = TMXTiledMap::create("map_lv1.tmx");
            break;
    }
}
TMXTiledMap* MapManager::getTileMap()
{
    return _tileMap;
}