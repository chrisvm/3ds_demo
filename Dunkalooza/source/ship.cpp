#include "ship.h"

void Ship::Load()
{
	LoadImage(spaceship_png, spaceship_png_size);
	vboIndex = 0;
    imageDimension = {0.0f, 1.0f, 0.0f, 1.0f};
    this->width = 32;
    this->height = 32;
}

void Ship::CreatePhysicsBody(C3D_Scene* phy_scene)
{
	// create body
	C3D_BodyParameters bodyParameters = {};
	bodyParameters.axis = FVec3_New(0, 0, 0);
	bodyParameters.linearVelocity = FVec3_New(0, 0, 0);
	bodyParameters.position = FVec3_New(pos.x, pos.y, 0.0f);
	bodyParameters.radianAngle = 0;
	bodyParameters.angularVelocity = FVec3_New(1.0f, 0.0f, 0.0f);
	bodyParameters.bodyType = BodyType_Dynamic;
	m_body = Scene_CreateBody(phy_scene, &bodyParameters);

	//Initialize box parameters.
	C3D_BoxParameters boxParameters = {};
	boxParameters.restitution = 0.0f;

	C3D_Transform transform = {};
	Mtx_Identity(&transform.rotation);
    transform.position = FVec3_New(width/2.0f, height/2.0f, 0.0f);
	C3D_FVec box_size = FVec3_New(width, height, 1.0f);
	BoxParameters_Init(&boxParameters, transform, box_size);

	//Add box.
	Body_AddBox(m_body, &boxParameters);
}

void Ship::AddForceToFacing(int forceMagnitude)
{
    // todo: finish this method
}

void Ship::Update(float deltaTime)
{
	printf("\x1b[6;1HPosition: x = %f y = %f", m_body->transform.position.x, m_body->transform.position.y);
    rotation = Quat_FromMtx(&m_body->transform.rotation);
}
