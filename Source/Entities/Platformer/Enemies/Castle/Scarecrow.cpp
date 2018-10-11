#include "Scarecrow.h"

const std::string Scarecrow::KeyEnemyScarecrow = "scarecrow";

Scarecrow* Scarecrow::deserialize(ValueMap* initProperties)
{
	Scarecrow* instance = new Scarecrow(initProperties);

	instance->autorelease();

	return instance;
}

Scarecrow::Scarecrow(ValueMap* initProperties) : PlatformerEnemy(initProperties,
	Resources::Entities_Platformer_Environment_Castle_Enemies_Scarecrow_Animations,
	PlatformerCollisionMapping::KeyCollisionTypeEnemy,
	Size(196.0f, 512.0f),
	0.8f,
	Vec2(0.0f, -128.0f))
{
}

Scarecrow::~Scarecrow()
{
}
