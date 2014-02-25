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
    static MissileTurret* create(const char *filename);
    virtual void handleCollisionWith(GameObject* obj);
    virtual void update();
    void prepare();
    /*
     Ham xac dinh Enemy co di vao pham vi anh huong cua MissileTurret ko
     @param: pos - toa do cua Enemy
     @return: true, neu thuoc pham vi anh huong, nguoc lai tra ve false
     **/
    bool detectIntrusion(Point pos);
    /*
     Ham ban enemy
     **/
    void fire(float dt);
};

#endif /* defined(__CanyonDefense__MissileTurret__) */
