//
//  Missile.h
//  CanyonDefense
//
//  Created by thang nguyen on 2/18/14.
//
//

#ifndef __CanyonDefense__Missile__
#define __CanyonDefense__Missile__

#include "GameObject.h"

class Missile : public GameObject {
    
public:
    virtual void handleCollisonWith(GameObject *obj);
    virtual void update();
};

#endif /* defined(__CanyonDefense__Missile__) */
