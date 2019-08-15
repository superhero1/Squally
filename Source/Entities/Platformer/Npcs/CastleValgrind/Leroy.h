#pragma once
#include <string>

#include "Entities/Platformer/PlatformerFriendly.h"

class HexusOpponentData;

class Leroy : public PlatformerFriendly
{
public:
	static Leroy* deserialize(cocos2d::ValueMap& initProperties);

	cocos2d::Vec2 getAvatarFrameOffset() override;
	static HexusOpponentData* getHexusOpponentData();

	static const std::string MapKeyLeroy;

private:
	typedef PlatformerFriendly super;
	Leroy(cocos2d::ValueMap& initProperties);
	~Leroy();

	static HexusOpponentData* HexusOpponentDataInstance;
	static const std::string HexusSaveKey;
};
