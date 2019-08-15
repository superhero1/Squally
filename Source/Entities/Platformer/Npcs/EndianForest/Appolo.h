#pragma once
#include <string>

#include "Entities/Platformer/PlatformerFriendly.h"

class HexusOpponentData;

class Appolo : public PlatformerFriendly
{
public:
	static Appolo* deserialize(cocos2d::ValueMap& initProperties);

	cocos2d::Vec2 getAvatarFrameOffset() override;
	static HexusOpponentData* getHexusOpponentData();

	static const std::string MapKeyAppolo;

private:
	typedef PlatformerFriendly super;
	Appolo(cocos2d::ValueMap& initProperties);
	~Appolo();

	static HexusOpponentData* HexusOpponentDataInstance;
	static const std::string HexusSaveKey;
};
