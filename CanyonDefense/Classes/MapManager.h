//
//  MapManager.h
//  CanyonDefense
//
//  Created by thang nguyen on 2/26/14.
//
//

#ifndef __CanyonDefense__MapManager__
#define __CanyonDefense__MapManager__

#include "cocos2d.h"

USING_NS_CC;

class MapManager : public Node {
private:
    int maptrix1[10][15] = {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
        {0,0,0,0,0,0,2,0,0,0,3,0,0,0,0,},
        {0,0,0,0,0,2,1,0,0,0,0,0,0,0,0,},
        {0,0,0,0,2,1,0,0,0,0,0,0,0,0,0,},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
        {0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,},
        {1,1,1,1,1,0,0,0,0,3,4,0,0,0,0,},
        {1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
    };
    int maptrix2[10][15] = {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
        {0,0,0,0,0,0,2,0,0,0,3,0,0,0,0,},
        {0,0,0,0,0,2,1,0,0,0,0,0,0,0,0,},
        {0,0,0,0,2,1,0,0,0,0,0,0,0,0,0,},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
        {0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,},
        {1,1,1,1,1,0,0,0,0,3,4,0,0,0,0,},
        {1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
    };

public:
    static MapManager* shareMap();
    
    TMXTiledMap* _tileMap;
    void initMapWithMapId(int mapId);
    void setMapTileForGame();
    CC_SYNTHESIZE(int, _mapId, MapId);
    
    TMXTiledMap* getTileMap();
    
};

#endif /* defined(__CanyonDefense__MapManager__) */
