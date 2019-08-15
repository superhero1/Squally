#pragma once
#include <string>

#include "Entities/Platformer/PlatformerFriendly.h"

class HexusOpponentData;

class Scaldor : public PlatformerFriendly
{
public:
	static Scaldor* deserialize(cocos2d::ValueMap& initProperties);

	cocos2d::Vec2 getAvatarFrameOffset() override;
	static HexusOpponentData* getHexusOpponentData();

	static const std::string MapKeyScaldor;

private:
	typedef PlatformerFriendly super;
	Scaldor(cocos2d::ValueMap& initProperties);
	~Scaldor();

	static HexusOpponentData* HexusOpponentDataInstance;
	static const std::string HexusSaveKey;
};
