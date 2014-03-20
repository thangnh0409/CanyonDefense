//
//  Wave.cpp
//  CanyonDefense
//
//  Created by thang nguyen on 2/25/14.
//
//

#include "Wave.h"

Wave* Wave::create(int spawnRate, int smallDragon, int mediumDragon, int bigDragon, int smallFlyDrg, int mediumFlyDrg)
{
    Wave* wave = new Wave();
    if (wave && wave->initWith(spawnRate, smallDragon, mediumDragon, bigDragon, smallFlyDrg, mediumFlyDrg)) {
        wave->autorelease();
        return wave;
    }
    return NULL;
}
bool Wave::initWith(int spawnRate, int smallDragon, int mediumDragon, int bigDragon, int smallFlyDrg, int mediumFlyDrg)
{
    this->setSpawnRate(spawnRate);

    this->setNumSmallDragon(smallDragon);
    this->setNumMediumDragon(mediumDragon);
    this->setNumBigDragon(bigDragon);
    this->setNumSmallFlyDragon(smallFlyDrg);
    this->setNumMediumFlyDragon(mediumFlyDrg);
    
    return true;
}
