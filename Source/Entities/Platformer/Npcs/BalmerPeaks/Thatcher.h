#pragma once
#include <string>

#include "Entities/Platformer/PlatformerFriendly.h"

class HexusOpponentData;

class Thatcher : public PlatformerFriendly
{
public:
	static Thatcher* deserialize(cocos2d::ValueMap& initProperties);

	cocos2d::Vec2 getAvatarFrameOffset() override;
	static HexusOpponentData* getHexusOpponentData();

	static const std::string MapKeyThatcher;

private:
	typedef PlatformerFriendly super;
	Thatcher(cocos2d::ValueMap& initProperties);
	~Thatcher();

	static HexusOpponentData* HexusOpponentDataInstance;
	static const std::string HexusSaveKey;
};
