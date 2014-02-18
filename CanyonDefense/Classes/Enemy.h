//
//  Enemy.h
//  CanyonDefense
//
//  Created by thang nguyen on 2/18/14.
//
//

#ifndef __CanyonDefense__Enemy__
#define __CanyonDefense__Enemy__

#include "GameObject.h"

class Enemy : public GameObject {
    
public:
    virtual void handleCollisonWith(GameObject *obj);
    virtual void update();
};

#endif /* defined(__CanyonDefense__Enemy__) */
