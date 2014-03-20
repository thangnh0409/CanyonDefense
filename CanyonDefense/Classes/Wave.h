//
//  Wave.h
//  CanyonDefense
//
//  Created by thang nguyen on 2/25/14.
//
//

#ifndef __CanyonDefense__Wave__
#define __CanyonDefense__Wave__

#include "cocos2d.h"

USING_NS_CC;

class Wave: public Node {
    
public:
    static Wave* create(int spawnRate, int smallDragon, int mediumDragon, int bigDragon, int smallFlyDrg, int mediumFlyDrg);
    bool initWith(int spawnRate, int smallDragon, int mediumDragon, int bigDragon, int smallFlyDrg, int mediumFlyDrg);

    CC_SYNTHESIZE(int, _spawnRate, SpawnRate);
    /**
     water dragon
     */
    CC_SYNTHESIZE(int, _numSmallDragon, NumSmallDragon);
    CC_SYNTHESIZE(int, _numMediumDragon, NumMediumDragon);
    CC_SYNTHESIZE(int, _numBigDragon, NumBigDragon);
    /**
     fly dragon
     */
    CC_SYNTHESIZE(int, _numSmallFlyDragon, NumSmallFlyDragon);
    CC_SYNTHESIZE(int, _numMediumFlyDragon, NumMediumFlyDragon);
};

#endif /* defined(__CanyonDefense__Wave__) */
