#pragma once
#include "cocos2d.h"

#include "Entities/Platformer/PlatformerEnemy.h"
#include "Resources.h"

using namespace cocos2d;

class SkeletalNecromancer : public PlatformerEnemy
{
public:
	static SkeletalNecromancer * deserialize(ValueMap* initProperties);

	static const std::string KeyEnemySkeletalNecromancer;

private:
	SkeletalNecromancer(ValueMap* initProperties);
	~SkeletalNecromancer();
};
