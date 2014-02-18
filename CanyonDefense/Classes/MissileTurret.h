//
//  MissileTurret.h
//  CanyonDefense
//
//  Created by thang nguyen on 2/18/14.
//
//

#ifndef __CanyonDefense__MissileTurret__
#define __CanyonDefense__MissileTurret__

#include "GameObject.h"

class MissileTurret : public GameObject {
    
public:
    virtual void handleCollisonWith(GameObject *obj);
    virtual void update();
};

#endif /* defined(__CanyonDefense__MissileTurret__) */
