#pragma once
#include <set>

#include "Engine/Maps/GameObject.h"

namespace cocos2d
{
	class ParticleSystem;
}

class CollisionObject;
class HackableData;

class EdgePortal : public GameObject
{
public:
	static EdgePortal* create(cocos2d::ValueMap& initProperties);

	static const std::string MapKeyEdgePortal;

protected:
	EdgePortal(cocos2d::ValueMap& initProperties);
	~EdgePortal();
	void onEnter() override;
	void initializePositions() override;
	void initializeListeners() override;

private:
	typedef GameObject super;

	CollisionObject* edgePortalCollision;
	CollisionObject* edgePortalHintCollision;

	bool wasTripped;
	bool isLocked;
	std::string mapFile;
	std::vector<std::string> mapArgs;

	static const std::string MapKeyEdgePortalArgs;
	static const std::string MapKeyEdgePortalDirection;
	static const std::string MapKeyEdgePortalMap;
};
