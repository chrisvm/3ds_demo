#ifndef _SHIP_H_
#define _SHIP_H_
#include "sprite.h"
#include "spaceship_png.h"
#include "types.h"

class Ship : public Sprite {
	C3D_Body* m_body;

public:
	void CreatePhysicsBody(C3D_Scene* phy_scene);
    void AddForceToFacing(int forceMagnitude);
	void Load();
    void Update(float deltaTime);
};

#endif
