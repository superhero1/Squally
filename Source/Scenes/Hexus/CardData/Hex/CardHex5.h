#pragma once

#include "cocos2d.h"

#include "Resources.h"
#include "Scenes/Hexus/CardData/CardData.h"

using namespace cocos2d;

class CardHex5 : public CardData
{
public:
	CardHex5();
	~CardHex5();

private:
	static const std::string StringKeyCardName;
};
