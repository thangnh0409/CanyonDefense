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
    static Wave* create(int spawnRate, int smallCar, int mediumCar, int bigCar);
    bool initWith(int spawnRate, int smallCar, int mediumCar, int bigCar);

    CC_SYNTHESIZE(int, _spawnRate, SpawnRate);
    CC_SYNTHESIZE(int, _numSmallCar, NumSmallCar);
    CC_SYNTHESIZE(int, _numMediumCar, NumMediumCar);
    CC_SYNTHESIZE(int, _numBigCar, NumBigCar);
};

#endif /* defined(__CanyonDefense__Wave__) */
