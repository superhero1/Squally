#pragma once
#include "cocos2d.h"

#include "Resources.h"
#include "Scenes/Hexus/Opponents/HexusOpponentData.h"

using namespace cocos2d;

class HexusOpponentMummyPriest : public HexusOpponentData
{
public:
	static HexusOpponentMummyPriest* getInstance();

private:
	HexusOpponentMummyPriest();
	~HexusOpponentMummyPriest();

	static const std::string OpponentSaveKey;
	static HexusOpponentMummyPriest* instance;
};