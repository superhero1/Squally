#pragma once
#include "cocos2d.h"
#include "Resources.h"
#include "Entities/Entity.h"
#include "Objects/Collision/CategoryGroup.h"

using namespace cocos2d;

class SmallBotSpider : public Entity
{
public:
	static SmallBotSpider * create();

protected:
	bool contactBegin(CollisionData data) override;
	bool contactUpdate(CollisionData data) override;
	bool contactEnd(CollisionData data) override;

private:
	SmallBotSpider();
	~SmallBotSpider();

	void update(float) override;

	Animation* walkAnimation;
	Animation* deathAnimation;

	Sprite* sprite;
};
