#include "VampireLord.h"

const std::string VampireLord::KeyEnemyVampireLord = "vampire_lord";

VampireLord* VampireLord::deserialize(ValueMap* initProperties)
{
	VampireLord* instance = new VampireLord(initProperties);

	instance->autorelease();

	return instance;
}

VampireLord::VampireLord(ValueMap* initProperties) : PlatformerEnemy(initProperties,
	Resources::Entities_Platformer_Environment_Castle_VampireLord_Animations,
	PlatformerCollisionMapping::KeyCollisionTypeEnemy,
	Size(240.0f, 312.0f),
	1.0f,
	Vec2(0.0f, 0.0f))
{
}

VampireLord::~VampireLord()
{
}
