#pragma once
#include <string>

#include "Entities/Platformer/PlatformerFriendly.h"

class HexusOpponentData;

class Nessie : public PlatformerFriendly
{
public:
	static Nessie* deserialize(cocos2d::ValueMap& initProperties);

	cocos2d::Vec2 getAvatarFrameOffset() override;
	static HexusOpponentData* getHexusOpponentData();

	static const std::string MapKeyNessie;

private:
	typedef PlatformerFriendly super;
	Nessie(cocos2d::ValueMap& initProperties);
	~Nessie();

	static HexusOpponentData* HexusOpponentDataInstance;
	static const std::string HexusSaveKey;
};
