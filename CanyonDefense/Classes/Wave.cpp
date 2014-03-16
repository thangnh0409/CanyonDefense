//
//  Wave.cpp
//  CanyonDefense
//
//  Created by thang nguyen on 2/25/14.
//
//

#include "Wave.h"

Wave* Wave::create(int spawnRate, int smallCar, int mediumCar, int bigCar)
{
    Wave* wave = new Wave();
    if (wave && wave->initWith(spawnRate, smallCar, mediumCar, bigCar)) {
        wave->autorelease();
        return wave;
    }
    return NULL;
}
bool Wave::initWith(int spawnRate, int smallCar, int mediumCar, int bigCar)
{
    this->setSpawnRate(spawnRate);
    this->setNumSmallCar(smallCar);
    this->setNumMediumCar(mediumCar);
    this->setNumBigCar(bigCar);
    return true;
}
